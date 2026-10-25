/*
 * This file is part of SparkNotePad.
 * Copyright 2026 Harry DS Alsyundawy (SparkNotePad Maintainer)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "InlineMathEvaluator.h"
#include "ScintillaNext.h"

#include <cmath>
#include <cctype>
#include <stdexcept>
#include <QRegularExpression>

namespace {

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_E
#define M_E 2.71828182845904523536
#endif

class MathParser {
public:
    explicit MathParser(const std::string &expr) : m_s(expr), m_pos(0) {}

    double parse() {
        skipWhitespace();
        if (m_pos >= m_s.length()) {
            throw std::runtime_error("Empty expression");
        }
        double val = parseExpression();
        skipWhitespace();
        if (m_pos < m_s.length()) {
            throw std::runtime_error(std::string("Unexpected character: '") + m_s[m_pos] + "'");
        }
        return val;
    }

private:
    void skipWhitespace() {
        while (m_pos < m_s.length() && (std::isspace(static_cast<unsigned char>(m_s[m_pos])))) {
            m_pos++;
        }
    }

    char peek() {
        skipWhitespace();
        return (m_pos < m_s.length()) ? m_s[m_pos] : 0;
    }

    char get() {
        skipWhitespace();
        return (m_pos < m_s.length()) ? m_s[m_pos++] : 0;
    }

    double parseExpression() {
        double val = parseTerm();
        while (true) {
            char op = peek();
            if (op == '+') {
                get();
                val += parseTerm();
            } else if (op == '-') {
                get();
                val -= parseTerm();
            } else {
                break;
            }
        }
        return val;
    }

    double parseTerm() {
        double val = parseFactor();
        while (true) {
            char op = peek();
            if (op == '*') {
                get();
                val *= parseFactor();
            } else if (op == '/') {
                get();
                double denom = parseFactor();
                if (std::abs(denom) < 1e-15) {
                    throw std::runtime_error("Division by zero");
                }
                val /= denom;
            } else if (op == '%') {
                get();
                double denom = parseFactor();
                if (std::abs(denom) < 1e-15) {
                    throw std::runtime_error("Modulo by zero");
                }
                val = std::fmod(val, denom);
            } else {
                break;
            }
        }
        return val;
    }

    double parseFactor() {
        double base = parsePrimary();
        if (peek() == '^') {
            get();
            double exp = parseFactor();  // right-associative
            base = std::pow(base, exp);
        }
        return base;
    }

    double parsePrimary() {
        char c = peek();
        if (c == '+') {
            get();
            return parsePrimary();
        }
        if (c == '-') {
            get();
            return -parsePrimary();
        }
        if (c == '(') {
            get();
            double val = parseExpression();
            if (peek() != ')') {
                throw std::runtime_error("Missing closing parenthesis");
            }
            get();
            return val;
        }

        if (std::isdigit(static_cast<unsigned char>(c)) || c == '.') {
            size_t start = m_pos;
            while (m_pos < m_s.length() &&
                   (std::isdigit(static_cast<unsigned char>(m_s[m_pos])) || m_s[m_pos] == '.')) {
                m_pos++;
            }
            if (m_pos < m_s.length() && (m_s[m_pos] == 'e' || m_s[m_pos] == 'E')) {
                m_pos++;
                if (m_pos < m_s.length() && (m_s[m_pos] == '+' || m_s[m_pos] == '-')) {
                    m_pos++;
                }
                while (m_pos < m_s.length() && std::isdigit(static_cast<unsigned char>(m_s[m_pos]))) {
                    m_pos++;
                }
            }
            std::string numStr = m_s.substr(start, m_pos - start);
            return std::stod(numStr);
        }

        if (std::isalpha(static_cast<unsigned char>(c))) {
            size_t start = m_pos;
            while (m_pos < m_s.length() &&
                   (std::isalnum(static_cast<unsigned char>(m_s[m_pos])) || m_s[m_pos] == '_')) {
                m_pos++;
            }
            std::string ident = m_s.substr(start, m_pos - start);
            std::transform(ident.begin(), ident.end(), ident.begin(),
                           [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });

            if (ident == "pi")
                return M_PI;
            if (ident == "e")
                return M_E;

            if (peek() == '(') {
                get();
                double arg1 = parseExpression();
                double arg2 = 0.0;
                bool hasArg2 = false;
                if (peek() == ',') {
                    get();
                    arg2 = parseExpression();
                    hasArg2 = true;
                }
                if (peek() != ')') {
                    throw std::runtime_error("Missing closing parenthesis in function call");
                }
                get();

                if (ident == "sin")
                    return std::sin(arg1);
                if (ident == "cos")
                    return std::cos(arg1);
                if (ident == "tan")
                    return std::tan(arg1);
                if (ident == "asin")
                    return std::asin(arg1);
                if (ident == "acos")
                    return std::acos(arg1);
                if (ident == "atan")
                    return std::atan(arg1);
                if (ident == "sinh")
                    return std::sinh(arg1);
                if (ident == "cosh")
                    return std::cosh(arg1);
                if (ident == "tanh")
                    return std::tanh(arg1);
                if (ident == "sqrt") {
                    if (arg1 < 0.0)
                        throw std::runtime_error("Domain error in sqrt (negative argument)");
                    return std::sqrt(arg1);
                }
                if (ident == "cbrt")
                    return std::cbrt(arg1);
                if (ident == "exp")
                    return std::exp(arg1);
                if (ident == "log" || ident == "ln") {
                    if (arg1 <= 0.0)
                        throw std::runtime_error("Domain error in log (argument <= 0)");
                    return std::log(arg1);
                }
                if (ident == "log10") {
                    if (arg1 <= 0.0)
                        throw std::runtime_error("Domain error in log10 (argument <= 0)");
                    return std::log10(arg1);
                }
                if (ident == "log2") {
                    if (arg1 <= 0.0)
                        throw std::runtime_error("Domain error in log2 (argument <= 0)");
                    return std::log2(arg1);
                }
                if (ident == "abs" || ident == "fabs")
                    return std::abs(arg1);
                if (ident == "floor")
                    return std::floor(arg1);
                if (ident == "ceil")
                    return std::ceil(arg1);
                if (ident == "round")
                    return std::round(arg1);
                if (ident == "pow") {
                    if (!hasArg2)
                        throw std::runtime_error("pow() requires two arguments: pow(base, exp)");
                    return std::pow(arg1, arg2);
                }

                throw std::runtime_error("Unknown function: " + ident);
            }

            throw std::runtime_error("Unknown identifier: " + ident);
        }

        throw std::runtime_error(std::string("Syntax error at: '") + c + "'");
    }

    std::string m_s;
    size_t m_pos;
};

}  // namespace

MathResult InlineMathEvaluator::evaluate(const QString &expression) {
    MathResult res;
    QString clean = expression.trimmed();
    if (clean.isEmpty()) {
        res.errorMessage = QStringLiteral("Expression is empty");
        return res;
    }

    try {
        MathParser parser(clean.toStdString());
        res.value = parser.parse();
        res.success = true;

        // Clean formatting: if integer or near integer, format without trailing zeros
        if (std::abs(res.value - std::round(res.value)) < 1e-12 && std::abs(res.value) < 1e15) {
            res.formattedValue = QString::number(static_cast<qint64>(std::round(res.value)));
        } else {
            res.formattedValue = QString::number(res.value, 'g', 12);
        }
    } catch (const std::exception &e) {
        res.success = false;
        res.errorMessage = QString::fromStdString(e.what());
    }

    return res;
}

bool InlineMathEvaluator::evaluateInEditor(ScintillaNext *editor, bool appendResult, QString *outMessage) {
    if (!editor) {
        if (outMessage)
            *outMessage = QStringLiteral("No active editor");
        return false;
    }

    sptr_t selStart = editor->selectionStart();
    sptr_t selEnd = editor->selectionEnd();

    QString expr;
    bool hasSelection = (selStart != selEnd);

    if (hasSelection) {
        expr = QString::fromUtf8(editor->get_text_range(selStart, selEnd));
    } else {
        // Read current line
        sptr_t curPos = editor->currentPos();
        sptr_t line = editor->lineFromPosition(curPos);
        sptr_t lineStart = editor->positionFromLine(line);
        sptr_t lineEnd = editor->lineEndPosition(line);
        expr = QString::fromUtf8(editor->get_text_range(lineStart, lineEnd));
    }

    // Strip trailing equals if user wrote "2 + 2 ="
    QString cleanExpr = expr.trimmed();
    if (cleanExpr.endsWith(QLatin1Char('='))) {
        cleanExpr.chop(1);
        cleanExpr = cleanExpr.trimmed();
    }

    MathResult res = evaluate(cleanExpr);
    if (!res.success) {
        if (outMessage)
            *outMessage = QStringLiteral("Math Error: ") + res.errorMessage;
        return false;
    }

    if (hasSelection) {
        if (appendResult) {
            QString replacement = expr + QStringLiteral(" = ") + res.formattedValue;
            QByteArray utf8 = replacement.toUtf8();
            editor->replaceSel(utf8.constData());
        } else {
            QByteArray utf8 = res.formattedValue.toUtf8();
            editor->replaceSel(utf8.constData());
        }
    } else {
        sptr_t curPos = editor->currentPos();
        sptr_t line = editor->lineFromPosition(curPos);
        sptr_t lineEnd = editor->lineEndPosition(line);
        QString appendStr = QStringLiteral(" = ") + res.formattedValue;
        QByteArray utf8 = appendStr.toUtf8();
        editor->insertText(lineEnd, utf8.constData());
        editor->gotoPos(lineEnd + utf8.length());
    }

    if (outMessage) {
        *outMessage = QStringLiteral("Evaluated: ") + res.formattedValue;
    }
    return true;
}
