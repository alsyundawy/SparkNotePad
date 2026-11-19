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
    static void testJsonPrettifyAndMinify() {
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

    static void testUrlEncodeAndDecode() {
        const QString plain = QStringLiteral("https://example.com/search?q=spark notepad & key=val#sec");
        const QString encoded = DeveloperToolsEngine::urlEncode(plain);
        QVERIFY(!encoded.contains(QStringLiteral(" ")));
        QVERIFY(encoded.contains(QStringLiteral("%20")));

        const QString decoded = DeveloperToolsEngine::urlDecode(encoded);
        QCOMPARE(decoded, plain);
    }

    static void testUnicodeEscapeAndUnescape() {
        const QString original = QStringLiteral("Café & résumé: 日本語");
        const QString escaped = DeveloperToolsEngine::unicodeEscape(original);
        QVERIFY(escaped.contains(QStringLiteral("\\u")));

        const QString unescaped = DeveloperToolsEngine::unicodeUnescape(escaped);
        QCOMPARE(unescaped, original);
    }

    static void testUuidV4Generation() {
        const QString uuid = DeveloperToolsEngine::generateUuidV4(false, false, true);
        static const QRegularExpression uuidRegex(
            QStringLiteral("^[0-9a-f]{8}-[0-9a-f]{4}-4[0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}$"),
            QRegularExpression::CaseInsensitiveOption);
        QVERIFY2(uuidRegex.match(uuid).hasMatch(), qPrintable(QStringLiteral("Generated invalid UUID: %1").arg(uuid)));

        const QString uuidUpper = DeveloperToolsEngine::generateUuidV4(true, true, true);
        QVERIFY(uuidUpper.startsWith(QStringLiteral("{")));
        QVERIFY(uuidUpper.endsWith(QStringLiteral("}")));
    }

    static void testTimestampConversion() {
        const qint64 epoch = 1767225600;  // 2026-01-01 00:00:00 UTC
        const QString iso = DeveloperToolsEngine::epochToIso(epoch);
        QVERIFY(iso.startsWith(QStringLiteral("2026-01-01T00:00:00")));

        bool ok = false;
        const qint64 roundtrip = DeveloperToolsEngine::isoToEpoch(iso, &ok);
        QVERIFY(ok);
        QCOMPARE(roundtrip, epoch);
    }

    static void testXmlFormatAndMinify() {
        const QString rawXml = QStringLiteral("<root><item id=\"1\"><name>Spark</name></item></root>");
        QString errorMsg;
        const QString formatted = DeveloperToolsEngine::formatXml(rawXml, 2, &errorMsg);
        QVERIFY(errorMsg.isEmpty());
        QVERIFY(formatted.contains(QStringLiteral("\n")));
        QVERIFY(formatted.contains(QStringLiteral("  <item id=\"1\">")));

        const QString minified = DeveloperToolsEngine::minifyXml(formatted, &errorMsg);
        QVERIFY(errorMsg.isEmpty());
        QVERIFY(!minified.contains(QStringLiteral("\n")));
        QVERIFY(minified.contains(QStringLiteral("<root><item id=\"1\">")));
    }

    static void testHtmlFormatAndMinify() {
        const QString rawHtml = QStringLiteral("<div><p>Hello <span>World</span></p></div>");
        QString errorMsg;
        const QString formatted = DeveloperToolsEngine::formatHtml(rawHtml, 4, &errorMsg);
        QVERIFY(errorMsg.isEmpty());
        QVERIFY(formatted.contains(QStringLiteral("\n")));

        const QString minified = DeveloperToolsEngine::minifyHtml(formatted, &errorMsg);
        QVERIFY(errorMsg.isEmpty());
        QVERIFY(!minified.contains(QStringLiteral("\n")));
    }

    static void testCssFormatAndMinify() {
        const QString rawCss = QStringLiteral("body { color: red; background: blue; } h1 { font-size: 14px; }");
        QString errorMsg;
        const QString formatted = DeveloperToolsEngine::formatCss(rawCss, &errorMsg);
        QVERIFY(errorMsg.isEmpty());
        QVERIFY(formatted.contains(QStringLiteral("\n")));

        const QString minified = DeveloperToolsEngine::minifyCss(formatted, &errorMsg);
        QVERIFY(errorMsg.isEmpty());
        QCOMPARE(minified, QStringLiteral("body{color:red;background:blue;}h1{font-size:14px;}"));
    }

    static void testJsFormatAndMinify() {
        const QString rawJs = QStringLiteral("function test() { var x = 1; return x + 2; }");
        QString errorMsg;
        const QString formatted = DeveloperToolsEngine::formatJs(rawJs, &errorMsg);
        QVERIFY(errorMsg.isEmpty());
        QVERIFY(formatted.contains(QStringLiteral("\n")));

        const QString minified = DeveloperToolsEngine::minifyJs(rawJs, &errorMsg);
        QVERIFY(errorMsg.isEmpty());
        QVERIFY(!minified.contains(QStringLiteral("//")));
    }

    static void testHtmlEncryptionModes() {
        const QString originalHtml = QStringLiteral("<h1>Confidential Document</h1><p>Secret content</p>");
        QString errorMsg;

        // Mode 1: JS Unescape
        const QString encJs = DeveloperToolsEngine::encryptHtml(originalHtml, HtmlEncryptMethod::JavascriptEscape, QString(), &errorMsg);
        QVERIFY(errorMsg.isEmpty());
        QVERIFY(encJs.contains(QStringLiteral("unescape")));

        const QString decJs = DeveloperToolsEngine::decryptHtml(encJs, &errorMsg);
        QVERIFY(errorMsg.isEmpty());
        QCOMPARE(decJs, originalHtml);

        // Mode 2: Base64
        const QString encB64 = DeveloperToolsEngine::encryptHtml(originalHtml, HtmlEncryptMethod::Base64, QString(), &errorMsg);
        QVERIFY(errorMsg.isEmpty());
        QVERIFY(encB64.contains(QStringLiteral("atob")));

        const QString decB64 = DeveloperToolsEngine::decryptHtml(encB64, &errorMsg);
        QVERIFY(errorMsg.isEmpty());
        QCOMPARE(decB64, originalHtml);

        // Mode 3: Password AES-256-GCM
        const QString password = QStringLiteral("SuperSecretPass123!@#");
        const QString encPass = DeveloperToolsEngine::encryptHtml(originalHtml, HtmlEncryptMethod::PasswordProtected, password, &errorMsg);
        QVERIFY(errorMsg.isEmpty());
        QVERIFY(encPass.contains(QStringLiteral("crypto.subtle")));
        QVERIFY(encPass.contains(QStringLiteral("SPKENC01:")));
    }

    static void testJwtParsing() {
        // Sample valid JWT token
        const QString token = QStringLiteral(
            "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."
            "eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ."
            "SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c");
        const JwtInfo info = DeveloperToolsEngine::parseJwt(token);
        QVERIFY(info.isValid);
        QVERIFY(info.error.isEmpty());
        QVERIFY(info.headerJson.contains(QStringLiteral("HS256")));
        QVERIFY(info.payloadJson.contains(QStringLiteral("1234567890")));
        QVERIFY(info.payloadJson.contains(QStringLiteral("John Doe")));

        // Invalid token
        const JwtInfo invalidInfo = DeveloperToolsEngine::parseJwt(QStringLiteral("invalid.jwt"));
        QVERIFY(!invalidInfo.isValid);
        QVERIFY(!invalidInfo.error.isEmpty());
    }

    static void testHexConversions() {
        const QString ascii = QStringLiteral("SparkNotePad 1.5.0");
        const QString hex = DeveloperToolsEngine::asciiToHex(ascii, QStringLiteral(" "));
        QVERIFY(hex.startsWith(QStringLiteral("53 70 61 72 6B")));

        QString errorMsg;
        const QString roundtrip = DeveloperToolsEngine::hexToAscii(hex, &errorMsg);
        QVERIFY(errorMsg.isEmpty());
        QCOMPARE(roundtrip, ascii);
    }

    static void testRadixConversion() {
        QString errorMsg;
        const QString hexVal = DeveloperToolsEngine::convertRadix(QStringLiteral("255"), 10, 16, true, &errorMsg);
        QVERIFY(errorMsg.isEmpty());
        QCOMPARE(hexVal.toUpper(), QStringLiteral("FF"));

        const QString binVal = DeveloperToolsEngine::convertRadix(QStringLiteral("FF"), 16, 2, true, &errorMsg);
        QVERIFY(errorMsg.isEmpty());
        QCOMPARE(binVal, QStringLiteral("11111111"));
    }

    static void testCaseConversions() {
        const QString title = DeveloperToolsEngine::toTitleCase(QStringLiteral("hello world of text editors"));
        QCOMPARE(title, QStringLiteral("Hello World Of Text Editors"));

        const QString inverted = DeveloperToolsEngine::toInvertCase(QStringLiteral("SparkNotePad 1.5.0"));
        QCOMPARE(inverted, QStringLiteral("sPARKnOTEpAD 1.5.0"));

        const QString sentence = DeveloperToolsEngine::toSentenceCase(QStringLiteral("hello world. this is spark. cool!"));
        QCOMPARE(sentence, QStringLiteral("Hello world. This is spark. Cool!"));
    }

    static void testPasswordStrengthEvaluation() {
        const PasswordStrength weak = DeveloperToolsEngine::evaluatePasswordStrength(QStringLiteral("123456"));
        QVERIFY(weak.score <= 1);
        QVERIFY(!weak.isStrong);

        const PasswordStrength strong = DeveloperToolsEngine::evaluatePasswordStrength(QStringLiteral("K9#vP2$xL!mQ8@zW"));
        QVERIFY(strong.score >= 3);
        QVERIFY(strong.isStrong);
    }
};
