#pragma once
#include <QObject>
#include <QTest>
#include "utils/NumberConverter.h"

class TestNumberConverter : public QObject {
    Q_OBJECT

private slots:
    void testConversions() {
        auto rHex = NumberConverter::convertString("255", NumberConverter::ToHex);
        QVERIFY(rHex.success);
        QCOMPARE(rHex.converted, QString("0xFF"));

        auto rDec = NumberConverter::convertString("0xFF", NumberConverter::ToDec);
        QVERIFY(rDec.success);
        QCOMPARE(rDec.converted, QString("255"));

        auto rBin = NumberConverter::convertString("0b1010", NumberConverter::ToDec);
        QVERIFY(rBin.success);
        QCOMPARE(rBin.converted, QString("10"));

        auto rOct = NumberConverter::convertString("0o77", NumberConverter::ToDec);
        QVERIFY(rOct.success);
        QCOMPARE(rOct.converted, QString("63"));

        auto rToBin = NumberConverter::convertString("10", NumberConverter::ToBin);
        QVERIFY(rToBin.success);
        QCOMPARE(rToBin.converted, QString("0b1010"));

        auto rToOct = NumberConverter::convertString("63", NumberConverter::ToOct);
        QVERIFY(rToOct.success);
        QCOMPARE(rToOct.converted, QString("0o77"));
    }
};
