/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad4 (zufuliu/notepad4) Evaluate JS Expression feature.
 * Uses native JavaScriptCore on macOS and portable runtime fallback on Linux.
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "JsEvaluator.h"
#include "ScintillaNext.h"

#include <QElapsedTimer>
#include <QProcess>
#include <QStandardPaths>
#include <QFile>

#if defined(__APPLE__)
#include <JavaScriptCore/JavaScript.h>

namespace {

QString jsStringToQString(JSStringRef jsStr) {
    if (!jsStr)
        return QString();
    size_t maxBytes = JSStringGetMaximumUTF8CStringSize(jsStr);
    QByteArray bytes(static_cast<int>(maxBytes), '\0');
    size_t actualBytes = JSStringGetUTF8CString(jsStr, bytes.data(), maxBytes);
    if (actualBytes > 0) {
        bytes.truncate(static_cast<int>(actualBytes - 1));
    }
    return QString::fromUtf8(bytes);
}

QString formatJsValue(JSContextRef ctx, JSValueRef val) {
    if (JSValueIsNull(ctx, val))
        return QStringLiteral("null");
    if (JSValueIsUndefined(ctx, val))
        return QStringLiteral("undefined");
    if (JSValueIsBoolean(ctx, val))
        return JSValueToBoolean(ctx, val) ? QStringLiteral("true") : QStringLiteral("false");

    // If object or array, try formatting via JSON.stringify(val, null, 2)
    if (JSValueIsObject(ctx, val)) {
        JSStringRef jsonName = JSStringCreateWithUTF8CString("JSON");
        JSValueRef jsonVal = JSObjectGetProperty(ctx, JSContextGetGlobalObject(ctx), jsonName, nullptr);
        JSStringRelease(jsonName);
        if (jsonVal && JSValueIsObject(ctx, jsonVal)) {
            JSObjectRef jsonObj = (JSObjectRef)jsonVal;
            JSStringRef stringifyName = JSStringCreateWithUTF8CString("stringify");
            JSValueRef stringifyVal = JSObjectGetProperty(ctx, jsonObj, stringifyName, nullptr);
            JSStringRelease(stringifyName);
            if (stringifyVal && JSValueIsObject(ctx, stringifyVal)) {
                JSObjectRef fn = (JSObjectRef)stringifyVal;
                JSValueRef args[3] = {val, JSValueMakeNull(ctx), JSValueMakeNumber(ctx, 2)};
                JSValueRef jsonRes = JSObjectCallAsFunction(ctx, fn, jsonObj, 3, args, nullptr);
                if (jsonRes && !JSValueIsUndefined(ctx, jsonRes)) {
                    JSStringRef s = JSValueToStringCopy(ctx, jsonRes, nullptr);
                    QString out = jsStringToQString(s);
                    JSStringRelease(s);
                    return out;
                }
            }
        }
    }

    JSStringRef s = JSValueToStringCopy(ctx, val, nullptr);
    QString out = jsStringToQString(s);
    JSStringRelease(s);
    return out;
}

}  // namespace

#endif

JsEvaluator::Result JsEvaluator::evaluate(const QString &script) {
    const QString trimmed = script.trimmed();
    if (trimmed.isEmpty()) {
        return {false, script, QString(), QStringLiteral("Empty script expression"), 0.0};
    }

    QElapsedTimer timer;
    timer.start();

#if defined(__APPLE__)
    JSGlobalContextRef ctx = JSGlobalContextCreate(nullptr);

    JSStringRef scriptRef = JSStringCreateWithUTF8CString(trimmed.toUtf8().constData());
    JSValueRef exception = nullptr;
    JSValueRef jsResult = JSEvaluateScript(ctx, scriptRef, nullptr, nullptr, 1, &exception);

    const double elapsedMs = timer.nsecsElapsed() / 1000000.0;

    Result res;
    res.expression = trimmed;
    res.executionTimeMs = elapsedMs;

    if (exception) {
        res.success = false;
        res.errorMessage = formatJsValue(ctx, exception);
    } else {
        res.success = true;
        res.resultString = formatJsValue(ctx, jsResult);
    }

    JSStringRelease(scriptRef);
    JSGlobalContextRelease(ctx);
    return res;

#else
    // Linux / Portable Fallback: Check for node, bun, deno, or qjs
    QStringList runners = {QStringLiteral("node"), QStringLiteral("bun"), QStringLiteral("deno"),
                           QStringLiteral("qjs")};

    QString runtimePath;
    for (const auto &r : runners) {
        runtimePath = QStandardPaths::findExecutable(r);
        if (!runtimePath.isEmpty())
            break;
    }

    if (runtimePath.isEmpty()) {
        return {false, trimmed, QString(),
                QStringLiteral("No JavaScript runtime found. Please install node, bun, or deno on your system."), 0.0};
    }

    QProcess proc;
    QStringList args;
    if (runtimePath.endsWith(QStringLiteral("deno"))) {
        args << QStringLiteral("eval") << QStringLiteral("console.log(%1)").arg(trimmed);
    } else {
        args << QStringLiteral("-e") << QStringLiteral("console.log(eval(%1))").arg(trimmed);
    }

    proc.start(runtimePath, args);
    if (!proc.waitForFinished(3000)) {
        proc.kill();
        return {false, trimmed, QString(), QStringLiteral("JavaScript evaluation timed out (3s)"), 3000.0};
    }

    const double elapsedMs = timer.nsecsElapsed() / 1000000.0;
    const QString stdOut = QString::fromUtf8(proc.readAllStandardOutput()).trimmed();
    const QString stdErr = QString::fromUtf8(proc.readAllStandardError()).trimmed();

    if (proc.exitCode() != 0 || !stdErr.isEmpty()) {
        return {false, trimmed, QString(), stdErr.isEmpty() ? QStringLiteral("Runtime exited with error") : stdErr,
                elapsedMs};
    }

    return {true, trimmed, stdOut, QString(), elapsedMs};
#endif
}

JsEvaluator::Result JsEvaluator::evaluateInEditor(ScintillaNext *editor, Action action) {
    if (!editor) {
        return {false, QString(), QString(), QStringLiteral("No active editor"), 0.0};
    }
    if (editor->readOnly()) {
        return {false, QString(), QString(), QStringLiteral("Document is read-only"), 0.0};
    }

    int startPos = editor->selectionStart();
    int endPos = editor->selectionEnd();
    QString scriptText;

    if (startPos != endPos) {
        scriptText = QString::fromUtf8(editor->get_text_range(startPos, endPos));
    } else {
        // Expand from line
        const int curPos = editor->currentPos();
        const int line = editor->lineFromPosition(curPos);
        const int lineStart = editor->positionFromLine(line);
        const int lineEnd = editor->lineEndPosition(line);
        startPos = lineStart;
        endPos = lineEnd;
        scriptText = QString::fromUtf8(editor->get_text_range(startPos, endPos));
    }

    auto result = evaluate(scriptText);
    if (!result.success) {
        return result;
    }

    editor->beginUndoAction();
    QString replacement;
    if (action == AppendComment) {
        replacement = scriptText.trimmed() + QStringLiteral(" // -> ") + result.resultString;
    } else {
        replacement = result.resultString;
    }

    editor->setTargetRange(startPos, endPos);
    const QByteArray utf8 = replacement.toUtf8();
    editor->replaceTarget(utf8.length(), utf8.constData());
    const int newPos = startPos + utf8.length();
    editor->setSelection(newPos, newPos);
    editor->endUndoAction();

    return result;
}
