#pragma once
#include <QObject>
#include <QTest>
#include "utils/MimeTools.h"

class TestMimeTools : public QObject {
    Q_OBJECT

private slots:
    void testBase64Suite() const {
        const QString plain = "Hello SparkNotePad!\nLine 2";
        const QString enc = MimeTools::base64Encode(plain);
        QCOMPARE(MimeTools::base64Decode(enc), plain);

        // Padding
        const QString unpadded = MimeTools::base64Encode("ab");
        QCOMPARE(unpadded, QString("YWI"));
        const QString padded = MimeTools::base64EncodeWithPadding("ab");
        QCOMPARE(padded, QString("YWI="));
        QVERIFY(padded.endsWith("="));

        // Unix EOL wrapping
        const QString longText = QString("A").repeated(100);
        const QString eolEnc = MimeTools::base64EncodeUnixEol(longText, 64);
        QVERIFY(eolEnc.contains('\n'));

        // By line
        const QString multiline = "Line1\nLine2\nLine3";
        const QString encByLine = MimeTools::base64EncodeByLine(multiline);
        const QString decByLine = MimeTools::base64DecodeByLine(encByLine);
        QCOMPARE(decByLine, multiline);

        // Strict
        bool ok = false;
        QCOMPARE(MimeTools::base64DecodeStrict(enc, &ok), plain);
        QVERIFY(ok);

        bool badOk = true;
        MimeTools::base64DecodeStrict("Invalid@@@Base64!", &badOk);
        QVERIFY(!badOk);
    }

    void testQuotedPrintableSuite() const {
        const QString plain = "Hello = World! Special: äöü & 123";
        const QString qp = MimeTools::quotedPrintableEncode(plain);
        QVERIFY(qp.contains("=3D"));
        const QString dec = MimeTools::quotedPrintableDecode(qp);
        QCOMPARE(dec, plain);
    }

    void testUrlEncodingSuite() const {
        const QString plain = "Hello World! @#$%";
        // RFC 1738 encodes space to +
        const QString rfc1738 = MimeTools::urlEncodeRfc1738(plain);
        QVERIFY(rfc1738.contains('+'));
        QCOMPARE(MimeTools::urlDecode(rfc1738), plain);

        // Extended (RFC 3986 percent-encoding, space to %20)
        const QString ext = MimeTools::urlEncodeExtended(plain);
        QVERIFY(ext.contains("%20"));
        QCOMPARE(MimeTools::urlDecode(ext), plain);

        // Full encoding (all characters become %XX)
        const QString full = MimeTools::urlEncodeFull("ABC");
        QCOMPARE(full, QString("%41%42%43"));
        QCOMPARE(MimeTools::urlDecode(full), QString("ABC"));

        // By line
        const QString multi = "One Two\nThree Four";
        const QString multiEnc = MimeTools::urlEncodeRfc1738ByLine(multi);
        QVERIFY(multiEnc.contains('\n'));
        const QString multiDec = MimeTools::urlDecode(multiEnc);
        QCOMPARE(multiDec, multi);
    }
};
