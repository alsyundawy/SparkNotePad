#pragma once
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

#ifndef PERFORMANCEPROFILER_H
#define PERFORMANCEPROFILER_H

#include <QString>
#include <QDateTime>

class ScintillaNext;

class PerformanceProfiler {
public:
    struct SystemInfo {
        QString osName;
        QString cpuArchitecture;
        QString cpuModel;
        int logicalCores{0};
        bool isAppleSilicon{false};
        bool isArm64{false};
    };

    struct MemoryMetrics {
        qint64 rssBytes{0};
        qint64 virtualBytes{0};
        double rssMegabytes{0.0};
        double virtualMegabytes{0.0};
    };

    struct DocumentMetrics {
        QString fileName;
        qint64 byteSize{0};
        int lineCount{0};
        QString language;
        bool isLargeFile{false};
        bool isHighPerformanceMode{false};
    };

    struct BenchmarkResult {
        QString testName;
        int iterations{0};
        double elapsedMs{0.0};
        double throughputMBps{0.0};
        QString details;
    };

    // System and hardware diagnostics
    static SystemInfo getSystemInfo();

    // Live resident memory diagnostics
    static MemoryMetrics getMemoryMetrics();

    // Active document metrics
    static DocumentMetrics getDocumentMetrics(ScintillaNext *editor);

    // High performance mode toggle for large files
    static void setHighPerformanceMode(ScintillaNext *editor, bool enable);
    static bool isHighPerformanceMode(ScintillaNext *editor);

    // Benchmarking suite
    static BenchmarkResult runScintillaInsertBenchmark(int lineCount = 50000);

    // Generate comprehensive text diagnostic report
    static QString generateDiagnosticReport(ScintillaNext *currentEditor = nullptr);
};

#endif  // PERFORMANCEPROFILER_H
