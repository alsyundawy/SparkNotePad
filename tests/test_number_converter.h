#pragma once
#include <QObject>
#include <QTest>
#include "utils/NumberConverter.h"

class TestNumberConverter : public QObject {
    Q_OBJECT

private slots:
    void testConversions() const {
        using enum NumberConverter::TargetBase;
        const auto rHex = NumberConverter::convertString("255", ToHex);
        QVERIFY(rHex.success);
        QCOMPARE(rHex.converted, QString("0xFF"));

        const auto rDec = NumberConverter::convertString("0xFF", ToDec);
        QVERIFY(rDec.success);
        QCOMPARE(rDec.converted, QString("255"));

        const auto rBin = NumberConverter::convertString("0b1010", ToDec);
        QVERIFY(rBin.success);
        QCOMPARE(rBin.converted, QString("10"));

        const auto rOct = NumberConverter::convertString("0o77", ToDec);
        QVERIFY(rOct.success);
        QCOMPARE(rOct.converted, QString("63"));

        const auto rToBin = NumberConverter::convertString("10", ToBin);
        QVERIFY(rToBin.success);
        QCOMPARE(rToBin.converted, QString("0b1010"));

        const auto rToOct = NumberConverter::convertString("63", ToOct);
        QVERIFY(rToOct.success);
        QCOMPARE(rToOct.converted, QString("0o77"));
    }
};
