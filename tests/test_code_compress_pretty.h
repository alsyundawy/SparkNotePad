#pragma once
#include <QObject>
#include <QTest>
#include "utils/CodeCompressPretty.h"

class TestCodeCompressPretty : public QObject {
    Q_OBJECT

private slots:
    void testJsonMinifyPrettify() {
        QString messy = "{   \"key\":   \"value\" ,  \"number\": 42 }";
        auto mini = CodeCompressPretty::compressJson(messy);
        QVERIFY(mini.success);
        QVERIFY(!mini.formatted.contains("  "));
        QVERIFY(mini.formatted.contains("\"key\":\"value\""));

        auto pretty = CodeCompressPretty::prettifyJson(mini.formatted);
        QVERIFY(pretty.success);
        QVERIFY(pretty.formatted.contains("\n"));
        QVERIFY(pretty.formatted.contains("    \"key\""));
    }

    void testCssMinify() {
        QString css = "/* comment */ body { color: red; margin: 0px; }";
        auto mini = CodeCompressPretty::compressCss(css);
        QVERIFY(mini.success);
        QVERIFY(!mini.formatted.contains("comment"));
        QVERIFY(mini.formatted.contains("body{color:red;margin:0px;}"));
    }
};
