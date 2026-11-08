#pragma once
#include <QObject>
#include <QTest>
#include <QRegularExpression>
#include "utils/GuidTimestampUtils.h"

class TestGuidTimestamp : public QObject {
    Q_OBJECT

private slots:
    void testGuidGeneration() const {
        const QString guid = GuidTimestampUtils::generateGuid();
        const QRegularExpression regex("^[0-9a-f]{8}-[0-9a-f]{4}-4[0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}$",
                                       QRegularExpression::CaseInsensitiveOption);
        QVERIFY(regex.match(guid).hasMatch());
    }

    void testIsoTimestamp() const {
        const QString iso = GuidTimestampUtils::generateTimestamp(GuidTimestampUtils::TimestampFormat::Iso8601Local);
        QVERIFY(!iso.isEmpty());
        QVERIFY(iso.contains('T') || iso.contains('-'));
    }
};
