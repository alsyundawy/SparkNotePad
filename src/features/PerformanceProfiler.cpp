/*
 * This file is part of NoteSciPad.
 *
 * Performance Profiler, Memory Diagnostics, and Hardware Benchmark Engine.
 * Tailored for Apple Silicon (ARM64) and Linux (x86_64/ARM64).
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "PerformanceProfiler.h"
#include "ScintillaNext.h"

#include <QSysInfo>
#include <QThread>
#include <QElapsedTimer>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>

#if defined(__APPLE__)
#include <mach/mach.h>
#include <sys/sysctl.h>
#elif defined(__linux__)
#include <unistd.h>
#endif

PerformanceProfiler::SystemInfo PerformanceProfiler::getSystemInfo() {
    SystemInfo info;
    info.osName = QSysInfo::prettyProductName();
    info.cpuArchitecture = QSysInfo::currentCpuArchitecture();
    info.logicalCores = QThread::idealThreadCount();

#if defined(__arm64__) || defined(__aarch64__)
    info.isArm64 = true;
#endif

#if defined(__APPLE__)
    char brand[256] = {0};
    size_t sz = sizeof(brand);
    if (sysctlbyname("machdep.cpu.brand_string", brand, &sz, nullptr, 0) == 0) {
        info.cpuModel = QString::fromUtf8(brand);
    } else {
        info.cpuModel = QStringLiteral("Apple Silicon (ARM64)");
    }

    if (info.isArm64 || info.cpuModel.contains(QStringLiteral("Apple"), Qt::CaseInsensitive)) {
        info.isAppleSilicon = true;
    }
#elif defined(__linux__)
    QFile cpuInfo(QStringLiteral("/proc/cpuinfo"));
    if (cpuInfo.open(QIODevice::ReadOnly)) {
        QTextStream in(&cpuInfo);
        while (!in.atEnd()) {
            const QString line = in.readLine();
            if (line.startsWith(QStringLiteral("model name")) || line.startsWith(QStringLiteral("Hardware"))) {
                const int idx = line.indexOf(QLatin1Char(':'));
                if (idx != -1) {
                    info.cpuModel = line.mid(idx + 1).trimmed();
                    break;
                }
            }
        }
    }
    if (info.cpuModel.isEmpty()) {
        info.cpuModel = info.cpuArchitecture;
    }
#else
    info.cpuModel = info.cpuArchitecture;
#endif

    return info;
}

PerformanceProfiler::MemoryMetrics PerformanceProfiler::getMemoryMetrics() {
    MemoryMetrics m;

#if defined(__APPLE__)
    struct mach_task_basic_info info;
    mach_msg_type_number_t count = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, reinterpret_cast<task_info_t>(&info), &count) ==
        KERN_SUCCESS) {
        m.rssBytes = static_cast<qint64>(info.resident_size);
        m.virtualBytes = static_cast<qint64>(info.virtual_size);
    }
#elif defined(__linux__)
    QFile statm(QStringLiteral("/proc/self/statm"));
    if (statm.open(QIODevice::ReadOnly)) {
        QTextStream in(&statm);
        qint64 vmsPages = 0, rssPages = 0;
        in >> vmsPages >> rssPages;
        const long pageSize = sysconf(_SC_PAGESIZE);
        if (pageSize > 0) {
            m.rssBytes = rssPages * pageSize;
            m.virtualBytes = vmsPages * pageSize;
        }
    }
#endif

    m.rssMegabytes = m.rssBytes / (1024.0 * 1024.0);
    m.virtualMegabytes = m.virtualBytes / (1024.0 * 1024.0);
    return m;
}

PerformanceProfiler::DocumentMetrics PerformanceProfiler::getDocumentMetrics(ScintillaNext *editor) {
    DocumentMetrics doc;
    if (!editor)
        return doc;

    doc.fileName = editor->getFileName();
    doc.lineCount = static_cast<int>(editor->lineCount());
    doc.byteSize = editor->length();
    doc.language = editor->languageName;
    doc.isHighPerformanceMode = isHighPerformanceMode(editor);

    // Consider files larger than 5 MB or 50,000 lines as large files
    if (doc.byteSize > 5 * 1024 * 1024 || doc.lineCount > 50000) {
        doc.isLargeFile = true;
    }

    return doc;
}

void PerformanceProfiler::setHighPerformanceMode(ScintillaNext *editor, bool enable) {
    if (!editor)
        return;

    if (enable) {
        // 1. Disable word wrapping (wrap calculation is O(N) on long documents)
        editor->setWrapMode(0);  // SC_WRAP_NONE
        // 2. Enable double buffered drawing
        editor->setBufferedDraw(true);
        // 3. Mark property
        editor->QObject::setProperty("HighPerformanceMode", true);
    } else {
        editor->QObject::setProperty("HighPerformanceMode", false);
    }
}

bool PerformanceProfiler::isHighPerformanceMode(ScintillaNext *editor) {
    if (!editor)
        return false;
    return editor->QObject::property("HighPerformanceMode").toBool();
}

PerformanceProfiler::BenchmarkResult PerformanceProfiler::runScintillaInsertBenchmark(int lineCount) {
    BenchmarkResult res;
    res.testName = QStringLiteral("Scintilla Text Ingestion & Line Indexing");
    res.iterations = lineCount;

    ScintillaNext testEditor(QStringLiteral("benchmark_tab"));
    testEditor.setWrapMode(0);
    testEditor.setBufferedDraw(true);

    const QByteArray sampleLine = "const size_t benchmark_buffer_index = 0x4A1F; // NoteSciPad Benchmark Line Stream\n";
    const qint64 totalBytes = sampleLine.length() * static_cast<qint64>(lineCount);

    QElapsedTimer timer;
    timer.start();

    const int batchSize = 5000;
    QByteArray batch;
    batch.reserve(sampleLine.length() * batchSize);
    for (int i = 0; i < batchSize; ++i) {
        batch.append(sampleLine);
    }

    testEditor.beginUndoAction();
    for (int i = 0; i < lineCount; i += batchSize) {
        testEditor.appendText(batch.length(), batch.constData());
    }
    testEditor.endUndoAction();

    res.elapsedMs = timer.nsecsElapsed() / 1000000.0;
    res.throughputMBps = (totalBytes / (1024.0 * 1024.0)) / (res.elapsedMs / 1000.0);
    res.details = QStringLiteral("Ingested %1 lines (%2 MB) in %3 ms (%4 lines/sec, %5 MB/sec)")
                      .arg(lineCount)
                      .arg(QString::number(totalBytes / (1024.0 * 1024.0), 'f', 2))
                      .arg(QString::number(res.elapsedMs, 'f', 2))
                      .arg(QString::number(lineCount / (res.elapsedMs / 1000.0), 'f', 0))
                      .arg(QString::number(res.throughputMBps, 'f', 2));

    return res;
}

QString PerformanceProfiler::generateDiagnosticReport(ScintillaNext *currentEditor) {
    const auto sys = getSystemInfo();
    const auto mem = getMemoryMetrics();
    const auto doc = getDocumentMetrics(currentEditor);

    QString r;
    r += QStringLiteral("========================================\n");
    r += QStringLiteral("NoteSciPad Performance & Hardware Report\n");
    r += QStringLiteral("========================================\n\n");

    r += QStringLiteral("[System Architecture]\n");
    r += QStringLiteral("OS:            %1\n").arg(sys.osName);
    r += QStringLiteral("CPU Model:     %1\n").arg(sys.cpuModel);
    r += QStringLiteral("Architecture:  %1%2\n")
             .arg(sys.cpuArchitecture)
             .arg(sys.isAppleSilicon ? QStringLiteral(" (Apple Silicon Native)")
                                     : (sys.isArm64 ? QStringLiteral(" (ARM64 Native)") : QString()));
    r += QStringLiteral("Logical Cores: %1\n\n").arg(sys.logicalCores);

    r += QStringLiteral("[Memory Footprint]\n");
    r += QStringLiteral("Resident (RSS): %1 MB\n").arg(QString::number(mem.rssMegabytes, 'f', 2));
    r += QStringLiteral("Virtual (VMS):  %1 MB\n\n").arg(QString::number(mem.virtualMegabytes, 'f', 2));

    if (currentEditor) {
        r += QStringLiteral("[Active Document Metrics]\n");
        r += QStringLiteral("File:          %1\n").arg(doc.fileName);
        r += QStringLiteral("Line Count:    %1 lines\n").arg(doc.lineCount);
        r += QStringLiteral("Byte Size:     %1 bytes (%2 MB)\n")
                 .arg(doc.byteSize)
                 .arg(QString::number(doc.byteSize / (1024.0 * 1024.0), 'f', 2));
        r += QStringLiteral("Language:      %1\n").arg(doc.language);
        r += QStringLiteral("High-Perf Mode: %1\n\n")
                 .arg(doc.isHighPerformanceMode ? QStringLiteral("ACTIVE") : QStringLiteral("Standard"));
    }

    r += QStringLiteral("[Engine Invariant]\n");
    r += QStringLiteral("Scintilla:     ScintillaNext + thirdparty/scintilla\n");
    r += QStringLiteral("Lexilla:       thirdparty/lexilla (91 registered languages)\n");

    return r;
}
