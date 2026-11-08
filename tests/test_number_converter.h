#pragma once
#include <QObject>
#include <QTest>
#include "utils/NumberConverter.h"

class TestNumberConverter : public QObject {
    Q_OBJECT

private slots:
    void testConversions() {
        const auto rHex = NumberConverter::convertString("255", NumberConverter::TargetBase::ToHex);
        QVERIFY(rHex.success);
        QCOMPARE(rHex.converted, QString("0xFF"));

        const auto rDec = NumberConverter::convertString("0xFF", NumberConverter::TargetBase::ToDec);
        QVERIFY(rDec.success);
        QCOMPARE(rDec.converted, QString("255"));

        const auto rBin = NumberConverter::convertString("0b1010", NumberConverter::TargetBase::ToDec);
        QVERIFY(rBin.success);
        QCOMPARE(rBin.converted, QString("10"));

        const auto rOct = NumberConverter::convertString("0o77", NumberConverter::TargetBase::ToDec);
        QVERIFY(rOct.success);
        QCOMPARE(rOct.converted, QString("63"));

        const auto rToBin = NumberConverter::convertString("10", NumberConverter::TargetBase::ToBin);
        QVERIFY(rToBin.success);
        QCOMPARE(rToBin.converted, QString("0b1010"));

        const auto rToOct = NumberConverter::convertString("63", NumberConverter::TargetBase::ToOct);
        QVERIFY(rToOct.success);
        QCOMPARE(rToOct.converted, QString("0o77"));
    }
};
