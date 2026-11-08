#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * Copyright 2026 Harry Dertin Sutisna Alsyundawy <alsyundawy@gmail.com>
 * https://github.com/alsyundawy/SparkNotePad
 */

#include <QObject>
#include <QRegularExpression>
#include <QTest>
#include "features/DeveloperToolsEngine.h"

class TestDeveloperToolsEngine : public QObject {
    Q_OBJECT

private slots:
    void testJsonPrettifyAndMinify() const {
        const QString rawJson =
            QStringLiteral("{\"name\":\"SparkNotePad\",\"version\":\"1.4.5\",\"features\":[1,2,3]}");
        QString errorMsg;
        const QString pretty = DeveloperToolsEngine::formatJson(rawJson, 4, &errorMsg);
        QVERIFY(errorMsg.isEmpty());
        QVERIFY(pretty.contains(QStringLiteral("\n")));
        QVERIFY(pretty.contains(QStringLiteral("    \"name\": \"SparkNotePad\"")));

        const QString minified = DeveloperToolsEngine::minifyJson(pretty, &errorMsg);
        QVERIFY(errorMsg.isEmpty());
        QVERIFY(!minified.contains(QStringLiteral("\n")));
        QVERIFY(minified.contains(QStringLiteral("\"version\":\"1.4.5\"")));

        // Test invalid JSON error reporting
        QString invalidErr;
        const QString failResult = DeveloperToolsEngine::formatJson(QStringLiteral("{unquoted: 123"), 4, &invalidErr);
        QVERIFY(failResult.isEmpty());
        QVERIFY(!invalidErr.isEmpty());
    }

    void testUrlEncodeAndDecode() const {
        const QString plain = QStringLiteral("https://example.com/search?q=spark notepad & key=val#sec");
        const QString encoded = DeveloperToolsEngine::urlEncode(plain);
        QVERIFY(!encoded.contains(QStringLiteral(" ")));
        QVERIFY(encoded.contains(QStringLiteral("%20")));

        const QString decoded = DeveloperToolsEngine::urlDecode(encoded);
        QCOMPARE(decoded, plain);
    }

    void testUnicodeEscapeAndUnescape() const {
        const QString original = QStringLiteral("Café & résumé: 日本語");
        const QString escaped = DeveloperToolsEngine::unicodeEscape(original);
        QVERIFY(escaped.contains(QStringLiteral("\\u")));

        const QString unescaped = DeveloperToolsEngine::unicodeUnescape(escaped);
        QCOMPARE(unescaped, original);
    }

    void testUuidV4Generation() const {
        const QString uuid = DeveloperToolsEngine::generateUuidV4(false, false, true);
        static const QRegularExpression uuidRegex(
            QStringLiteral("^[0-9a-f]{8}-[0-9a-f]{4}-4[0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}$"),
            QRegularExpression::CaseInsensitiveOption);
        QVERIFY2(uuidRegex.match(uuid).hasMatch(), qPrintable(QStringLiteral("Generated invalid UUID: %1").arg(uuid)));

        const QString uuidUpper = DeveloperToolsEngine::generateUuidV4(true, true, true);
        QVERIFY(uuidUpper.startsWith(QStringLiteral("{")));
        QVERIFY(uuidUpper.endsWith(QStringLiteral("}")));
    }

    void testTimestampConversion() const {
        const qint64 epoch = 1767225600;  // 2026-01-01 00:00:00 UTC
        const QString iso = DeveloperToolsEngine::epochToIso(epoch);
        QVERIFY(iso.startsWith(QStringLiteral("2026-01-01T00:00:00")));

        bool ok = false;
        const qint64 roundtrip = DeveloperToolsEngine::isoToEpoch(iso, &ok);
        QVERIFY(ok);
        QCOMPARE(roundtrip, epoch);
    }
};
