#pragma once
#include <QObject>
#include <QTest>
#include "utils/MathEvaluator.h"

class TestMathEvaluator : public QObject {
    Q_OBJECT

private slots:
    void testEvaluation() {
        auto r1 = MathEvaluator::evaluateString("1 + 2 * 3");
        QVERIFY(r1.success);
        QCOMPARE(r1.value, 7.0);

        auto r2 = MathEvaluator::evaluateString("(10 - 2) / 4");
        QVERIFY(r2.success);
        QCOMPARE(r2.value, 2.0);

        auto r3 = MathEvaluator::evaluateString("sqrt(144)");
        QVERIFY(r3.success);
        QCOMPARE(r3.value, 12.0);
    }
};
