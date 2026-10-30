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
#if !defined(UNIT_TESTING)
#include "ScintillaNext.h"
#endif

#include <algorithm>
#include <cmath>
#include <QList>
#include <QtMath>

namespace {

class Parser {
public:
    explicit Parser(const QString &text) : m_text(text), m_pos(0), m_len(text.length()) {}

    double parse() {
        skipWhitespace();
        if (m_pos >= m_len) {
            throw QStringLiteral("Empty expression");
        }
        double val = parseExpression();
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
                double denom = parsePower();
                if (denom == 0.0) {
                    throw QStringLiteral("Division by zero");
                }
                val /= denom;
            } else if (m_pos < m_len && m_text[m_pos] == '%') {
                m_pos++;
                double denom = parsePower();
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
            double exp = parsePower();  // right-associative
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

    double parsePrimary() {
        skipWhitespace();
        if (m_pos >= m_len) {
            throw QStringLiteral("Unexpected end of expression");
        }

        const QChar ch = m_text[m_pos];
        if (ch == '(') {
            m_pos++;
            double val = parseExpression();
            skipWhitespace();
            if (m_pos >= m_len || m_text[m_pos] != ')') {
                throw QStringLiteral("Missing closing parenthesis ')'");
            }
            m_pos++;
            return val;
        }

        // Number parsing
        if (ch.isDigit() || ch == '.') {
            const int start = m_pos;
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
            double num = m_text.mid(start, m_pos - start).toDouble(&ok);
            if (!ok) {
                throw QStringLiteral("Invalid numeric format: %1").arg(m_text.mid(start, m_pos - start));
            }
            return num;
        }

        // Identifier (function or constant)
        if (ch.isLetter() || ch == '_') {
            const int start = m_pos;
            while (m_pos < m_len && (m_text[m_pos].isLetterOrNumber() || m_text[m_pos] == '_')) {
                m_pos++;
            }
            const QString ident = m_text.mid(start, m_pos - start).toLower();

            skipWhitespace();
            // Function call
            if (m_pos < m_len && m_text[m_pos] == '(') {
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
                    throw QStringLiteral("Missing closing parenthesis for %1()").arg(ident);
                }
                m_pos++;

                return evaluateFunction(ident, args);
            }

            // Constants
            if (ident == QStringLiteral("pi"))
                return M_PI;
            if (ident == QStringLiteral("e"))
                return M_E;
            if (ident == QStringLiteral("tau"))
                return 2.0 * M_PI;
            if (ident == QStringLiteral("phi"))
                return (1.0 + std::sqrt(5.0)) / 2.0;

            throw QStringLiteral("Unknown constant: %1").arg(ident);
        }

        throw QStringLiteral("Unexpected token '%1'").arg(ch);
    }

    static double evaluateFunction(const QString &fn, const QList<double> &args) {
        if (args.isEmpty()) {
            throw QStringLiteral("Function %1() requires arguments").arg(fn);
        }

        if (fn == QStringLiteral("sqrt"))
            return std::sqrt(args[0]);
        if (fn == QStringLiteral("cbrt"))
            return std::cbrt(args[0]);
        if (fn == QStringLiteral("abs"))
            return std::fabs(args[0]);
        if (fn == QStringLiteral("sin"))
            return std::sin(args[0]);
        if (fn == QStringLiteral("cos"))
            return std::cos(args[0]);
        if (fn == QStringLiteral("tan"))
            return std::tan(args[0]);
        if (fn == QStringLiteral("asin"))
            return std::asin(args[0]);
        if (fn == QStringLiteral("acos"))
            return std::acos(args[0]);
        if (fn == QStringLiteral("atan"))
            return std::atan(args[0]);
        if (fn == QStringLiteral("sinh"))
            return std::sinh(args[0]);
        if (fn == QStringLiteral("cosh"))
            return std::cosh(args[0]);
        if (fn == QStringLiteral("tanh"))
            return std::tanh(args[0]);
        if (fn == QStringLiteral("log") || fn == QStringLiteral("ln"))
            return std::log(args[0]);
        if (fn == QStringLiteral("log10"))
            return std::log10(args[0]);
        if (fn == QStringLiteral("log2"))
            return std::log2(args[0]);
        if (fn == QStringLiteral("exp"))
            return std::exp(args[0]);
        if (fn == QStringLiteral("floor"))
            return std::floor(args[0]);
        if (fn == QStringLiteral("ceil"))
            return std::ceil(args[0]);
        if (fn == QStringLiteral("round"))
            return std::round(args[0]);
        if (fn == QStringLiteral("trunc"))
            return std::trunc(args[0]);

        if (fn == QStringLiteral("pow")) {
            if (args.size() < 2)
                throw QStringLiteral("pow(base, exp) requires 2 arguments");
            return std::pow(args[0], args[1]);
        }
        if (fn == QStringLiteral("min")) {
            return *std::min_element(args.cbegin(), args.cend());
        }
        if (fn == QStringLiteral("max")) {
            return *std::max_element(args.cbegin(), args.cend());
        }

        throw QStringLiteral("Unknown math function: %1()").arg(fn);
    }

    const QString m_text;
    int m_pos;
    const int m_len;
};

}  // namespace

QString MathEvaluator::formatValue(double val) {
    if (std::isnan(val))
        return QStringLiteral("NaN");
    if (std::isinf(val))
        return (val > 0) ? QStringLiteral("Infinity") : QStringLiteral("-Infinity");

    // Clean integer check
    if (val == std::floor(val) && std::fabs(val) < 1e15) {
        return QString::number(static_cast<qint64>(val));
    }

    // High-precision clean float
    QString str = QString::number(val, 'g', 12);
    return str;
}

MathEvaluator::Result MathEvaluator::evaluateString(const QString &expr) {
    QString cleaned = expr.trimmed();
    // Strip trailing = if present
    if (cleaned.endsWith('=')) {
        cleaned.chop(1);
        cleaned = cleaned.trimmed();
    }

    if (cleaned.isEmpty()) {
        return {false, 0.0, expr, QString(), QStringLiteral("Empty expression")};
    }

    try {
        Parser parser(cleaned);
        const double val = parser.parse();
        const QString formatted = formatValue(val);
        return {true, val, cleaned, formatted, QString()};
    } catch (const QString &err) {
        return {false, 0.0, cleaned, QString(), err};
    } catch (...) {
        return {false, 0.0, cleaned, QString(), QStringLiteral("Invalid mathematical syntax")};
    }
}

#if !defined(UNIT_TESTING)
MathEvaluator::Result MathEvaluator::evaluateInEditor(ScintillaNext *editor, Action action) {
    if (!editor) {
        return {false, 0.0, QString(), QString(), QStringLiteral("No active editor")};
    }
    if (editor->readOnly()) {
        return {false, 0.0, QString(), QString(), QStringLiteral("Document is read-only")};
    }

    int startPos = editor->selectionStart();
    int endPos = editor->selectionEnd();
    QString exprText;

    if (startPos != endPos) {
        exprText = QString::fromUtf8(editor->get_text_range(startPos, endPos));
    } else {
        // Expand from line or cursor
        const int curPos = editor->currentPos();
        const int line = editor->lineFromPosition(curPos);
        const int lineStart = editor->positionFromLine(line);
        const QString lineText = QString::fromUtf8(editor->get_text_range(lineStart, curPos));

        // Look back for beginning of expression
        int exprStartInLine = lineText.lastIndexOf(QLatin1String("="));
        if (exprStartInLine != -1) {
            exprStartInLine++;
        } else {
            exprStartInLine = lineText.lastIndexOf(QLatin1String(";"));
            if (exprStartInLine != -1) {
                exprStartInLine++;
            } else {
                exprStartInLine = 0;
            }
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
    if (action == AppendResult) {
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
    editor->replaceTarget(utf8.length(), utf8.constData());
    const int newPos = startPos + utf8.length();
    editor->setSelection(newPos, newPos);
    editor->endUndoAction();

    return result;
}

#endif
