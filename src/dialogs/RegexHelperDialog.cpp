/*
 * This file is part of NoteSciPad.
 *
 * PCRE2 Regular Expression Reference and Live Interactive Tester.
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "RegexHelperDialog.h"
#include "ui_RegexHelperDialog.h"

#include <QRegularExpression>
#include <QHeaderView>
#include <QTreeWidgetItem>

RegexHelperDialog::RegexHelperDialog(QWidget *parent, const QString &initialPattern)
    : QDialog(parent), ui(new Ui::RegexHelperDialog) {
    ui->setupUi(this);

    ui->treeRef->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeRef->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->treeRef->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    populateReferenceTree();

    if (!initialPattern.isEmpty()) {
        ui->editPattern->setText(initialPattern);
    } else {
        ui->editPattern->setText(QStringLiteral("(\\w+)\\s*=\\s*(\\d+)"));
        ui->editReplace->setText(QStringLiteral("$1: \\2"));
        ui->editSampleText->setPlainText(
            QStringLiteral("alpha = 100\nbeta = 250\ngamma = 42\n# Unmatched comment line"));
    }

    connect(ui->editPattern, &QLineEdit::textChanged, this, &RegexHelperDialog::updateRegexTest);
    connect(ui->editReplace, &QLineEdit::textChanged, this, &RegexHelperDialog::updateRegexTest);
    connect(ui->editSampleText, &QPlainTextEdit::textChanged, this, &RegexHelperDialog::updateRegexTest);
    connect(ui->chkCaseSensitive, &QCheckBox::toggled, this, &RegexHelperDialog::updateRegexTest);
    connect(ui->chkDotAll, &QCheckBox::toggled, this, &RegexHelperDialog::updateRegexTest);
    connect(ui->chkMultiline, &QCheckBox::toggled, this, &RegexHelperDialog::updateRegexTest);
    connect(ui->btnClose, &QPushButton::clicked, this, &QDialog::accept);

    updateRegexTest();
}

RegexHelperDialog::~RegexHelperDialog() {
    delete ui;
}

void RegexHelperDialog::updateRegexTest() {
    const QString pattern = ui->editPattern->text();
    if (pattern.isEmpty()) {
        ui->labelStatus->setText(tr("Pattern status: Enter a regular expression to test"));
        ui->labelStatus->setStyleSheet(QStringLiteral("color: palette(text); font-weight: bold;"));
        ui->editReplacePreview->clear();
        return;
    }

    QRegularExpression::PatternOptions options = QRegularExpression::UseUnicodePropertiesOption;
    if (!ui->chkCaseSensitive->isChecked()) {
        options |= QRegularExpression::CaseInsensitiveOption;
    }
    if (ui->chkDotAll->isChecked()) {
        options |= QRegularExpression::DotMatchesEverythingOption;
    }
    if (ui->chkMultiline->isChecked()) {
        options |= QRegularExpression::MultilineOption;
    }

    QRegularExpression re(pattern, options);
    if (!re.isValid()) {
        ui->labelStatus->setText(
            tr("✗ Pattern Error at offset %1: %2").arg(re.patternErrorOffset()).arg(re.errorString()));
        ui->labelStatus->setStyleSheet(QStringLiteral("color: #E11D48; font-weight: bold;"));
        ui->editReplacePreview->clear();
        return;
    }

    const QString sample = ui->editSampleText->toPlainText();
    auto it = re.globalMatch(sample);
    int matchCount = 0;
    while (it.hasNext()) {
        it.next();
        matchCount++;
    }

    ui->labelStatus->setText(tr("✓ Valid PCRE2 Pattern — %1 match(es) found").arg(matchCount));
    ui->labelStatus->setStyleSheet(QStringLiteral("color: #16A34A; font-weight: bold;"));

    // Preview substitution
    const QString repTemplate = ui->editReplace->text();
    if (repTemplate.isEmpty() || matchCount == 0) {
        ui->editReplacePreview->setPlainText(sample);
        return;
    }

    QString substitutedOutput;
    int lastPos = 0;
    auto repIt = re.globalMatch(sample);
    while (repIt.hasNext()) {
        QRegularExpressionMatch m = repIt.next();
        substitutedOutput += sample.mid(lastPos, m.capturedStart(0) - lastPos);

        // Substitute replacement tokens
        QString replacedPart;
        for (int i = 0; i < repTemplate.length();) {
            const QChar ch = repTemplate.at(i);
            if ((ch == QLatin1Char('\\') || ch == QLatin1Char('$')) && i + 1 < repTemplate.length()) {
                const QChar nextCh = repTemplate.at(i + 1);
                if (nextCh.isDigit()) {
                    int groupNum = nextCh.digitValue();
                    replacedPart += m.captured(groupNum);
                    i += 2;
                    continue;
                } else if (nextCh == QLatin1Char('&')) {
                    replacedPart += m.captured(0);
                    i += 2;
                    continue;
                } else if (ch == QLatin1Char('\\')) {
                    if (nextCh == QLatin1Char('n')) {
                        replacedPart += QLatin1Char('\n');
                        i += 2;
                        continue;
                    }
                    if (nextCh == QLatin1Char('t')) {
                        replacedPart += QLatin1Char('\t');
                        i += 2;
                        continue;
                    }
                    if (nextCh == QLatin1Char('r')) {
                        replacedPart += QLatin1Char('\r');
                        i += 2;
                        continue;
                    }
                    if (nextCh == QLatin1Char('\\')) {
                        replacedPart += QLatin1Char('\\');
                        i += 2;
                        continue;
                    }
                }
            }
            replacedPart += ch;
            i++;
        }

        substitutedOutput += replacedPart;
        lastPos = m.capturedEnd(0);
    }
    substitutedOutput += sample.mid(lastPos);
    ui->editReplacePreview->setPlainText(substitutedOutput);
}

void RegexHelperDialog::populateReferenceTree() {
    ui->treeRef->clear();

    const struct RefGroup {
        const char *category;
        struct Item {
            const char *token;
            const char *desc;
            const char *example;
        } items[6];
    } groups[] = {{"Character Classes",
                   {{"\\d", "Any digit [0-9]", "\\d+ matches 123"},
                    {"\\w", "Word character [a-zA-Z0-9_]", "\\w+ matches var_1"},
                    {"\\s", "Whitespace (space, tab, newline)", "\\s+ matches spaces"},
                    {"\\D, \\W, \\S", "Negated classes (not digit, word, space)", "\\D+ matches non-digits"},
                    {"\\p{L}", "Any Unicode letter (all languages)", "\\p{L}+ matches café, 日本語"},
                    {"[a-z0-9_]", "Custom character set / range", "[A-Z]{3} matches USA"}}},
                  {"Anchors & Boundaries",
                   {{"^", "Start of string / start of line", "^# matches header line"},
                    {"$", "End of string / end of line", ";$ matches line ending with ;"},
                    {"\\b", "Word boundary", "\\bcat\\b matches cat (not catch)"},
                    {"\\B", "Non-word boundary", "\\Bcat matches scatter"},
                    {"\\A", "Beginning of entire document", "\\A<?xml matches file start"},
                    {"\\Z", "End of entire document", "\\Z matches EOF"}}},
                  {"Quantifiers",
                   {{"*", "0 or more occurrences (greedy)", "a* matches '', 'a', 'aaa'"},
                    {"+", "1 or more occurrences (greedy)", "a+ matches 'a', 'aaa'"},
                    {"?", "0 or 1 occurrence (optional)", "colou?r matches color, colour"},
                    {"{n,m}", "Between n and m occurrences", "\\d{2,4} matches 1999, 24"},
                    {"*?, +?", "Lazy / non-greedy matching", "<.+?> matches <b> not <b>c</b>"},
                    {"+", "Possessive quantifier (no backtrack)", "\\d++ avoids catastrophic backtrack"}}},
                  {"Groups & Lookarounds",
                   {{"(...)", "Capturing group (creates \\1, $1)", "(\\w+): (\\d+)"},
                    {"(?:...)", "Non-capturing group", "(?:http|https)://"},
                    {"(?=...)", "Positive lookahead (must follow)", "\\d+(?=px) matches 100 in 100px"},
                    {"(?!...)", "Negative lookahead (must not follow)", "foo(?!bar) matches foo not foobar"},
                    {"(?<=...)", "Positive lookbehind (must precede)", "(?<=\\$)\\d+ matches 50 in $50"},
                    {"(?<!...)", "Negative lookbehind (must not precede)", "(?<!\\\\)test matches non-escaped"}}},
                  {"Replacement Tokens",
                   {{"\\1 .. \\9, $1 .. $9", "Captured group replacement", "$2 = $1"},
                    {"\\0, $0, &", "Entire matched text", "[&] wraps match in brackets"},
                    {"\\n, \\r, \\t", "Newline, carriage return, tab", "\\1\\n\\2 splits into two lines"},
                    {"\\\\", "Literal backslash in replacement", "path\\\\file"},
                    {"\\$", "Literal dollar sign in replacement", "\\$100"},
                    {"&", "Synonym for \\0 (entire match)", "[&]"}}}};
    for (const auto &g : groups) {
        QTreeWidgetItem *groupItem = new QTreeWidgetItem(ui->treeRef);
        groupItem->setText(0, QString::fromLatin1(g.category));
        groupItem->setExpanded(true);

        for (const auto &item : g.items) {
            if (!item.token || item.token[0] == '\0')
                continue;
            QTreeWidgetItem *child = new QTreeWidgetItem(groupItem);
            child->setText(0, QString::fromLatin1(item.token));
            child->setText(1, QString::fromLatin1(item.desc));
            child->setText(2, QString::fromLatin1(item.example));
        }
    }
}
