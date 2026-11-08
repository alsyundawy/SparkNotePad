/*
 * This file is part of SparkNotePad.
 *
 * Adapted from Notepad4 (zufuliu/notepad4) Calculate Expression feature.
 * Source: zufuliu/notepad4 (src/Edit.cpp, PR #1230)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "MathEvaluator.h"
#ifndef UNIT_TESTING
#include "ScintillaNext.h"
#endif

#include <algorithm>
#include <cmath>
#include <exception>
#include <limits>
#include <QList>
#include <QtMath>

namespace {

class Parser {
public:
    explicit Parser(const QString &text) : m_text(text), m_len(text.length()) {}

    double parse() {
        skipWhitespace();
        if (m_pos >= m_len) {
            throw QStringLiteral("Empty expression");
        }
        const double val = parseExpression();
        skipWhitespace();
        if (m_pos < m_len) {
            throw QStringLiteral("Unexpected character '%1' at position %2").arg(m_text[m_pos]).arg(m_pos + 1);
        }
        return val;
    }

private:
    void skipWhitespace() {
        while (m_pos < m_len && m_text[m_pos].isSpace()) {
            m_pos++;
        }
    }

    double parseExpression() {
        double val = parseTerm();
        while (true) {
            skipWhitespace();
            if (m_pos < m_len && m_text[m_pos] == '+') {
                m_pos++;
                val += parseTerm();
            } else if (m_pos < m_len && m_text[m_pos] == '-') {
                m_pos++;
                val -= parseTerm();
            } else {
                break;
            }
        }
        return val;
    }

    double parseTerm() {
        double val = parsePower();
        while (true) {
            skipWhitespace();
            if (m_pos < m_len && m_text[m_pos] == '*') {
                m_pos++;
                val *= parsePower();
            } else if (m_pos < m_len && m_text[m_pos] == '/') {
                m_pos++;
                const double denom = parsePower();
                if (denom == 0.0) {
                    throw QStringLiteral("Division by zero");
                }
                val /= denom;
            } else if (m_pos < m_len && m_text[m_pos] == '%') {
                m_pos++;
                const double denom = parsePower();
                if (denom == 0.0) {
                    throw QStringLiteral("Modulo by zero");
                }
                val = std::fmod(val, denom);
            } else {
                break;
            }
        }
        return val;
    }

    double parsePower() {
        double val = parseFactor();
        skipWhitespace();
        if (m_pos < m_len && m_text[m_pos] == '^') {
            m_pos++;
            const double exp = parsePower();  // right-associative
            val = std::pow(val, exp);
        }
        return val;
    }

    double parseFactor() {
        skipWhitespace();
        if (m_pos < m_len && m_text[m_pos] == '+') {
            m_pos++;
            return parseFactor();
        }
        if (m_pos < m_len && m_text[m_pos] == '-') {
            m_pos++;
            return -parseFactor();
        }
        return parsePrimary();
    }

    double parseParentheses() {
        m_pos++;
        const double val = parseExpression();
        skipWhitespace();
        if (m_pos >= m_len || m_text[m_pos] != ')') {
            throw QStringLiteral("Missing closing parenthesis ')'");
        }
        m_pos++;
        return val;
    }

    double parseNumber(QChar ch) {
        const qsizetype start = m_pos;
        bool hasDecimal = (ch == '.');
        bool hasExp = false;
        m_pos++;

        while (m_pos < m_len) {
            const QChar c = m_text[m_pos];
            if (c.isDigit()) {
                m_pos++;
            } else if (c == '.' && !hasDecimal && !hasExp) {
                hasDecimal = true;
                m_pos++;
            } else if ((c == 'e' || c == 'E') && !hasExp) {
                hasExp = true;
                m_pos++;
                if (m_pos < m_len && (m_text[m_pos] == '+' || m_text[m_pos] == '-')) {
                    m_pos++;
                }
            } else {
                break;
            }
        }
        bool ok = false;
        const double num = m_text.mid(start, m_pos - start).toDouble(&ok);
        if (!ok) {
            throw QStringLiteral("Invalid numeric format: %1").arg(m_text.mid(start, m_pos - start));
        }
        return num;
    }

    QList<double> parseArgumentList(const QString &fnName) {
        m_pos++;
        QList<double> args;
        skipWhitespace();
        if (m_pos < m_len && m_text[m_pos] != ')') {
            args.append(parseExpression());
            while (true) {
                skipWhitespace();
                if (m_pos < m_len && m_text[m_pos] == ',') {
                    m_pos++;
                    args.append(parseExpression());
                } else {
                    break;
                }
            }
        }
        skipWhitespace();
        if (m_pos >= m_len || m_text[m_pos] != ')') {
            throw QStringLiteral("Missing closing parenthesis for %1()").arg(fnName);
        }
        m_pos++;
        return args;
    }

    double parseIdentifier() {
        const qsizetype start = m_pos;
        while (m_pos < m_len && (m_text[m_pos].isLetterOrNumber() || m_text[m_pos] == '_')) {
            m_pos++;
        }
        const QString ident = m_text.mid(start, m_pos - start).toLower();

        skipWhitespace();
        if (m_pos < m_len && m_text[m_pos] == '(') {
            const QList<double> args = parseArgumentList(ident);
            return evaluateFunction(ident, args);
        }

        if (ident == QStringLiteral("pi")) {
            return M_PI;
        }
        if (ident == QStringLiteral("e")) {
            return M_E;
        }
        if (ident == QStringLiteral("tau")) {
            return 2.0 * M_PI;
        }
        if (ident == QStringLiteral("phi")) {
            return (1.0 + std::sqrt(5.0)) / 2.0;
        }

        throw QStringLiteral("Unknown constant: %1").arg(ident);
    }

    double parsePrimary() {
        skipWhitespace();
        if (m_pos >= m_len) {
            throw QStringLiteral("Unexpected end of expression");
        }

        const QChar ch = m_text[m_pos];
        if (ch == '(') {
            return parseParentheses();
        }
        if (ch.isDigit() || ch == '.') {
            return parseNumber(ch);
        }
        if (ch.isLetter() || ch == '_') {
            return parseIdentifier();
        }

        throw QStringLiteral("Unexpected token '%1'").arg(ch);
    }

    static double evaluateUnaryMath(const QString &fn, double x) {
        if (fn == QStringLiteral("sqrt")) { return std::sqrt(x); }
        if (fn == QStringLiteral("cbrt")) { return std::cbrt(x); }
        if (fn == QStringLiteral("abs")) { return std::fabs(x); }
        if (fn == QStringLiteral("sin")) { return std::sin(x); }
        if (fn == QStringLiteral("cos")) { return std::cos(x); }
        if (fn == QStringLiteral("tan")) { return std::tan(x); }
        if (fn == QStringLiteral("asin")) { return std::asin(x); }
        if (fn == QStringLiteral("acos")) { return std::acos(x); }
        if (fn == QStringLiteral("atan")) { return std::atan(x); }
        if (fn == QStringLiteral("sinh")) { return std::sinh(x); }
        if (fn == QStringLiteral("cosh")) { return std::cosh(x); }
        if (fn == QStringLiteral("tanh")) { return std::tanh(x); }
        if (fn == QStringLiteral("log") || fn == QStringLiteral("ln")) { return std::log(x); }
        if (fn == QStringLiteral("log10")) { return std::log10(x); }
        if (fn == QStringLiteral("log2")) { return std::log2(x); }
        if (fn == QStringLiteral("exp")) { return std::exp(x); }
        if (fn == QStringLiteral("floor")) { return std::floor(x); }
        if (fn == QStringLiteral("ceil")) { return std::ceil(x); }
        if (fn == QStringLiteral("round")) { return std::round(x); }
        if (fn == QStringLiteral("trunc")) { return std::trunc(x); }
        return std::numeric_limits<double>::quiet_NaN();
    }

    static double evaluateFunction(const QString &fn, const QList<double> &args) {
        if (args.isEmpty()) {
            throw QStringLiteral("Function %1() requires arguments").arg(fn);
        }

        const double unaryResult = evaluateUnaryMath(fn, args[0]);
        if (!std::isnan(unaryResult)) {
            return unaryResult;
        }

        if (fn == QStringLiteral("pow")) {
            if (args.size() < 2) {
                throw QStringLiteral("pow(base, exp) requires 2 arguments");
            }
            return std::pow(args[0], args[1]);
        }
        if (fn == QStringLiteral("min")) {
            return *std::ranges::min_element(args);
        }
        if (fn == QStringLiteral("max")) {
            return *std::ranges::max_element(args);
        }

        throw QStringLiteral("Unknown math function: %1()").arg(fn);
    }

    const QString m_text;
    qsizetype m_pos{0};
    const qsizetype m_len{0};
};

}  // namespace

QString MathEvaluator::formatValue(double val) {
    if (std::isnan(val)) {
        return QStringLiteral("NaN");
    }
    if (std::isinf(val)) {
        return (val > 0) ? QStringLiteral("Infinity") : QStringLiteral("-Infinity");
    }

    // Clean integer check
    if (val == std::floor(val) && std::fabs(val) < 1e15) {
        return QString::number(static_cast<qint64>(val));
    }

    // High-precision clean float
    return QString::number(val, 'g', 12);
}

MathEvaluator::Result MathEvaluator::evaluateString(const QString &expr) {
    QString cleaned = expr.trimmed();
    // Strip trailing = if present
    if (cleaned.endsWith('=')) {
        cleaned.chop(1);
        cleaned = cleaned.trimmed();
    }

    try {
        Parser parser(cleaned);
        const double val = parser.parse();
        const QString formatted = formatValue(val);
        return {true, val, cleaned, formatted, QString()};
    } catch (const QString &err) {
        return {false, 0.0, cleaned, QString(), err};
    } catch (const std::exception &ex) {
        return {false, 0.0, cleaned, QString(), QString::fromUtf8(ex.what())};
    }
}

#ifndef UNIT_TESTING
MathEvaluator::Result MathEvaluator::evaluateInEditor(ScintillaNext *editor, Action action) {
    if (!editor) {
        return {false, 0.0, QString(), QString(), QStringLiteral("No active editor")};
    }
    if (editor->readOnly()) {
        return {false, 0.0, QString(), QString(), QStringLiteral("Document is read-only")};
    }

    int startPos = static_cast<int>(editor->selectionStart());
    int endPos = static_cast<int>(editor->selectionEnd());
    QString exprText;

    if (startPos != endPos) {
        exprText = QString::fromUtf8(editor->get_text_range(startPos, endPos));
    } else {
        // Expand from line or cursor
        const int curPos = static_cast<int>(editor->currentPos());
        const int line = static_cast<int>(editor->lineFromPosition(curPos));
        const int lineStart = static_cast<int>(editor->positionFromLine(line));
        const QString lineText = QString::fromUtf8(editor->get_text_range(lineStart, curPos));

        // Look back for beginning of expression
        const qsizetype eqIdx = lineText.lastIndexOf(QLatin1Char('='));
        const qsizetype semiIdx = lineText.lastIndexOf(QLatin1Char(';'));
        int exprStartInLine = 0;
        if (eqIdx != -1) {
            exprStartInLine = static_cast<int>(eqIdx + 1);
        } else if (semiIdx != -1) {
            exprStartInLine = static_cast<int>(semiIdx + 1);
        }

        startPos = lineStart + exprStartInLine;
        endPos = curPos;
        exprText = QString::fromUtf8(editor->get_text_range(startPos, endPos));
    }

    auto result = evaluateString(exprText);
    if (!result.success) {
        return result;
    }

    editor->beginUndoAction();
    QString replacement;
    if (action == Action::AppendResult) {
        replacement = exprText.trimmed();
        if (!replacement.endsWith('=')) {
            replacement += QStringLiteral(" = ");
        } else {
            replacement += QStringLiteral(" ");
        }
        replacement += result.formattedResult;
    } else {
        replacement = result.formattedResult;
    }

    editor->setTargetRange(startPos, endPos);
    const QByteArray utf8 = replacement.toUtf8();
    editor->replaceTarget(static_cast<int>(utf8.length()), utf8.constData());
    const int newPos = startPos + static_cast<int>(utf8.length());
    editor->setSelection(newPos, newPos);
    editor->endUndoAction();

    return result;
}
#endif
