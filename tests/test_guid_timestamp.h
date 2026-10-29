#pragma once
#include <QObject>
#include <QTest>
#include <QRegularExpression>
#include "utils/GuidTimestampUtils.h"

class TestGuidTimestamp : public QObject {
    Q_OBJECT

private slots:
    void testGuidGeneration() {
        QString guid = GuidTimestampUtils::generateGuid();
        QRegularExpression regex("^[0-9a-f]{8}-[0-9a-f]{4}-4[0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}$",
                                 QRegularExpression::CaseInsensitiveOption);
        QVERIFY(regex.match(guid).hasMatch());
    }

    void testIsoTimestamp() {
        QString iso = GuidTimestampUtils::generateTimestamp(GuidTimestampUtils::Iso8601Local);
        QVERIFY(!iso.isEmpty());
        QVERIFY(iso.contains('T') || iso.contains('-'));
    }
};
