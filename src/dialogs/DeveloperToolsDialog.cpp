/*
 * This file is part of SparkNotePad.
 *
 * Copyright 2026 Harry Dertin Sutisna Alsyundawy <alsyundawy@gmail.com>
 * https://github.com/alsyundawy/SparkNotePad
 * ALSYUNDAWY IT SOLUTION <https://alsyundawy.com>
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "DeveloperToolsDialog.h"
#include <QCryptographicHash>
#include <QFileDialog>
#include <QFileInfo>
#include <QGuiApplication>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QSpinBox>
#include <QSplitter>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QClipboard>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QFontDatabase>
#include <QFontMetrics>
#include "MainWindow.h"
#include "ScintillaNext.h"
#include "../features/DeveloperToolsEngine.h"

DeveloperToolsDialog::DeveloperToolsDialog(MainWindow *mainWindow, int initialTab, QWidget *parent)
    : QDialog(parent ? parent : mainWindow), m_mainWindow(mainWindow) {
    setWindowTitle(tr("Developer Tools & Utilities Hub"));
    setMinimumSize(920, 680);
    resize(980, 720);
    setupUi();
    if (m_tabWidget && initialTab >= 0 && initialTab < m_tabWidget->count()) {
        m_tabWidget->setCurrentIndex(initialTab);
    }
    onLoadFromEditor();
}

void DeveloperToolsDialog::setCurrentTab(int index) {
    if (m_tabWidget && index >= 0 && index < m_tabWidget->count()) {
        m_tabWidget->setCurrentIndex(index);
    }
}

void DeveloperToolsDialog::setupUi() {
    QFont dlgFont = font();
    dlgFont.setPointSizeF(10.5);
    setFont(dlgFont);

    setStyleSheet(QStringLiteral(
        "QDialog { font-size: 11px; }"
        "QTabBar::tab { font-weight: 600; font-size: 11px; min-height: 26px; padding: 4px 14px; }"
        "QPushButton { font-size: 11px; font-weight: 500; padding: 4px 10px; min-height: 26px; border-radius: 4px; }"
        "QComboBox { font-size: 11px; padding: 3px 8px; min-height: 26px; }"
        "QLabel { font-size: 11px; }"
        "QGroupBox { font-weight: 600; font-size: 11.5px; }"
        "QCheckBox { font-size: 11px; }"
    ));

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(10);

    // Tab Widget with modern styling and padding
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->addTab(createHashTab(), tr("Hash Tools"));
    m_tabWidget->addTab(createMimeTab(), tr("MIME Tools"));
    m_tabWidget->addTab(createConverterTab(), tr("Converter"));
    m_tabWidget->addTab(createCryptoTab(), tr("Encryption"));
    m_tabWidget->addTab(createDevToolsTab(), tr("Developer Tools"));
    mainLayout->addWidget(m_tabWidget, 0);

    // Monospace font for code and cryptographic data
    QFont monoFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    monoFont.setPointSizeF(10.5);

    // Shared Splitter for Input and Output
    auto *splitter = new QSplitter(Qt::Vertical, this);

    auto *inputBox = new QGroupBox(tr("Input Text:"), this);
    inputBox->setStyleSheet(QStringLiteral("QGroupBox { font-weight: 600; font-size: 11.5px; }"));
    auto *inputLayout = new QVBoxLayout(inputBox);
    inputLayout->setContentsMargins(8, 8, 8, 8);
    m_inputEdit = new QPlainTextEdit(inputBox);
    m_inputEdit->setPlaceholderText(tr("Enter or paste input text here... (Loaded automatically from active document selection)"));
    m_inputEdit->setFont(monoFont);
    m_inputEdit->setToolTip(tr("Source text to be processed, converted, hashed, or formatted"));
    const int tabStop = 4 * QFontMetrics(monoFont).horizontalAdvance(QLatin1Char(' '));
    m_inputEdit->setTabStopDistance(tabStop);
    inputLayout->addWidget(m_inputEdit);
    splitter->addWidget(inputBox);

    auto *outputBox = new QGroupBox(tr("Processed Output:"), this);
    outputBox->setStyleSheet(QStringLiteral("QGroupBox { font-weight: 600; font-size: 11.5px; }"));
    auto *outputLayout = new QVBoxLayout(outputBox);
    outputLayout->setContentsMargins(8, 8, 8, 8);
    m_outputEdit = new QPlainTextEdit(outputBox);
    m_outputEdit->setPlaceholderText(tr("Processed result will appear here..."));
    m_outputEdit->setFont(monoFont);
    m_outputEdit->setToolTip(tr("Processed result from tools, hashes, or converters (ready to copy or apply)"));
    m_outputEdit->setTabStopDistance(tabStop);
    outputLayout->addWidget(m_outputEdit);
    splitter->addWidget(outputBox);

    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);
    mainLayout->addWidget(splitter, 1);

    // Bottom Action Bar
    auto *bottomLayout = new QHBoxLayout();
    bottomLayout->setSpacing(8);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setStyleSheet(QStringLiteral("color: #3fb950; font-weight: 600; font-size: 11px; padding: 2px 6px;"));
    bottomLayout->addWidget(m_statusLabel, 1);

    auto *btnFromEditor = new QPushButton(tr("📥 From Active Document"), this);
    btnFromEditor->setMinimumHeight(28);
    btnFromEditor->setCursor(Qt::PointingHandCursor);
    btnFromEditor->setToolTip(tr("Load selected text or entire content from currently active document in editor"));
    connect(btnFromEditor, &QPushButton::clicked, this, &DeveloperToolsDialog::onLoadFromEditor);
    bottomLayout->addWidget(btnFromEditor);

    auto *btnCopy = new QPushButton(tr("📋 Copy Output"), this);
    btnCopy->setMinimumHeight(28);
    btnCopy->setCursor(Qt::PointingHandCursor);
    btnCopy->setToolTip(tr("Copy processed output text directly to system clipboard"));
    connect(btnCopy, &QPushButton::clicked, this, &DeveloperToolsDialog::onCopyOutput);
    bottomLayout->addWidget(btnCopy);

    auto *btnApply = new QPushButton(tr("✨ Apply to Active Document"), this);
    btnApply->setMinimumHeight(28);
    btnApply->setCursor(Qt::PointingHandCursor);
    btnApply->setToolTip(tr("Replace current selection or insert processed output into active document"));
    btnApply->setStyleSheet(QStringLiteral("QPushButton { font-weight: 600; }"));
    connect(btnApply, &QPushButton::clicked, this, &DeveloperToolsDialog::onApplyToEditor);
    bottomLayout->addWidget(btnApply);

    auto *btnClose = new QPushButton(tr("Close"), this);
    btnClose->setMinimumHeight(28);
    btnClose->setCursor(Qt::PointingHandCursor);
    btnClose->setToolTip(tr("Close Developer Tools Hub dialog"));
    connect(btnClose, &QPushButton::clicked, this, &QDialog::accept);
    bottomLayout->addWidget(btnClose);

    mainLayout->addLayout(bottomLayout);
}

QWidget *DeveloperToolsDialog::createHashTab() {
    auto *tab = new QWidget(this);
    auto *layout = new QGridLayout(tab);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setHorizontalSpacing(12);
    layout->setVerticalSpacing(10);

    // Row 0: Algorithm & Generate
    auto *lblAlgo = new QLabel(tr("Algorithm:"), tab);
    lblAlgo->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    lblAlgo->setStyleSheet(QStringLiteral("font-weight: 500; font-size: 12px;"));
    layout->addWidget(lblAlgo, 0, 0);

    m_comboHashAlgo = new QComboBox(tab);
    m_comboHashAlgo->setMinimumHeight(28);
    m_comboHashAlgo->setToolTip(tr("Select cryptographic hash algorithm"));
    m_comboHashAlgo->addItems({
        QStringLiteral("SHA-256"), QStringLiteral("SHA-512"),
        QStringLiteral("SHA-1"), QStringLiteral("MD5"),
        QStringLiteral("SHA3-256"), QStringLiteral("SHA3-512"),
        QStringLiteral("Keccak-256"), QStringLiteral("Keccak-512")
    });
    layout->addWidget(m_comboHashAlgo, 0, 1);

    auto *btnCompute = new QPushButton(tr("⚡ Generate Text Hash"), tab);
    btnCompute->setMinimumHeight(28);
    btnCompute->setMinimumWidth(160);
    btnCompute->setCursor(Qt::PointingHandCursor);
    btnCompute->setToolTip(tr("Compute selected cryptographic hash from input text"));
    connect(btnCompute, &QPushButton::clicked, this, &DeveloperToolsDialog::onComputeHash);
    layout->addWidget(btnCompute, 0, 2);

    // Row 1: Compare Hash
    auto *lblCompare = new QLabel(tr("Compare Hash:"), tab);
    lblCompare->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    lblCompare->setStyleSheet(QStringLiteral("font-weight: 500; font-size: 11px;"));
    layout->addWidget(lblCompare, 1, 0);

    m_compareHashEdit = new QLineEdit(tab);
    m_compareHashEdit->setMinimumHeight(28);
    m_compareHashEdit->setPlaceholderText(tr("Paste expected hash to compare against output..."));
    m_compareHashEdit->setToolTip(tr("Paste target hash here to compare and verify integrity"));
    m_compareHashEdit->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    connect(m_compareHashEdit, &QLineEdit::textChanged, this, &DeveloperToolsDialog::onCompareHash);
    layout->addWidget(m_compareHashEdit, 1, 1);

    m_hashCompareResult = new QLabel(tab);
    m_hashCompareResult->setMinimumWidth(110);
    m_hashCompareResult->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_hashCompareResult, 1, 2);

    // Row 2: File Checksum
    auto *lblFile = new QLabel(tr("File Checksum:"), tab);
    lblFile->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    lblFile->setStyleSheet(QStringLiteral("font-weight: 500; font-size: 11px;"));
    layout->addWidget(lblFile, 2, 0);

    m_filePathEdit = new QLineEdit(tab);
    m_filePathEdit->setMinimumHeight(28);
    m_filePathEdit->setPlaceholderText(tr("Select file to calculate cryptographic hash..."));
    m_filePathEdit->setToolTip(tr("Path to target file for checksum calculation"));
    layout->addWidget(m_filePathEdit, 2, 1);

    auto *btnBrowse = new QPushButton(tr("📁 Browse & Compute..."), tab);
    btnBrowse->setMinimumHeight(28);
    btnBrowse->setMinimumWidth(160);
    btnBrowse->setCursor(Qt::PointingHandCursor);
    btnBrowse->setToolTip(tr("Browse file from disk to compute or verify cryptographic hash"));
    connect(btnBrowse, &QPushButton::clicked, this, &DeveloperToolsDialog::onBrowseFileForHash);
    layout->addWidget(btnBrowse, 2, 2);

    layout->setColumnStretch(1, 1);
    return tab;
}

QWidget *DeveloperToolsDialog::createMimeTab() {
    auto *tab = new QWidget(this);
    auto *layout = new QGridLayout(tab);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setHorizontalSpacing(10);
    layout->setVerticalSpacing(10);

    // Row 0: Base64 & URL Encoding
    auto *btnB64Enc = new QPushButton(tr("Base64 Encode"), tab);
    btnB64Enc->setMinimumHeight(28);
    btnB64Enc->setCursor(Qt::PointingHandCursor);
    btnB64Enc->setToolTip(tr("Encode input text to standard RFC 4648 Base64"));
    connect(btnB64Enc, &QPushButton::clicked, this, &DeveloperToolsDialog::onBase64Encode);
    layout->addWidget(btnB64Enc, 0, 0);

    auto *btnB64Dec = new QPushButton(tr("Base64 Decode"), tab);
    btnB64Dec->setMinimumHeight(28);
    btnB64Dec->setCursor(Qt::PointingHandCursor);
    btnB64Dec->setToolTip(tr("Decode Base64 string back to plain text"));
    connect(btnB64Dec, &QPushButton::clicked, this, &DeveloperToolsDialog::onBase64Decode);
    layout->addWidget(btnB64Dec, 0, 1);

    auto *btnUrlEnc = new QPushButton(tr("URL Encode"), tab);
    btnUrlEnc->setMinimumHeight(28);
    btnUrlEnc->setCursor(Qt::PointingHandCursor);
    btnUrlEnc->setToolTip(tr("Percent-encode URL parameters (RFC 3986)"));
    connect(btnUrlEnc, &QPushButton::clicked, this, &DeveloperToolsDialog::onUrlEncode);
    layout->addWidget(btnUrlEnc, 0, 2);

    auto *btnUrlDec = new QPushButton(tr("URL Decode"), tab);
    btnUrlDec->setMinimumHeight(28);
    btnUrlDec->setCursor(Qt::PointingHandCursor);
    btnUrlDec->setToolTip(tr("Decode percent-encoded URL string"));
    connect(btnUrlDec, &QPushButton::clicked, this, &DeveloperToolsDialog::onUrlDecode);
    layout->addWidget(btnUrlDec, 0, 3);

    // Row 1: HTML Entities & Quoted-Printable Encoding
    auto *btnEntityEnc = new QPushButton(tr("HTML Entities Encode"), tab);
    btnEntityEnc->setMinimumHeight(28);
    btnEntityEnc->setCursor(Qt::PointingHandCursor);
    btnEntityEnc->setToolTip(tr("Escape special characters to HTML entities (&lt;, &gt;, &amp;)"));
    connect(btnEntityEnc, &QPushButton::clicked, this, &DeveloperToolsDialog::onHtmlEntityEncode);
    layout->addWidget(btnEntityEnc, 1, 0);

    auto *btnEntityDec = new QPushButton(tr("HTML Entities Decode"), tab);
    btnEntityDec->setMinimumHeight(28);
    btnEntityDec->setCursor(Qt::PointingHandCursor);
    btnEntityDec->setToolTip(tr("Unescape HTML entities to raw text"));
    connect(btnEntityDec, &QPushButton::clicked, this, &DeveloperToolsDialog::onHtmlEntityDecode);
    layout->addWidget(btnEntityDec, 1, 1);

    auto *btnQpEnc = new QPushButton(tr("Quoted-Printable Encode"), tab);
    btnQpEnc->setMinimumHeight(28);
    btnQpEnc->setCursor(Qt::PointingHandCursor);
    btnQpEnc->setToolTip(tr("Encode text to Quoted-Printable (RFC 2045)"));
    connect(btnQpEnc, &QPushButton::clicked, this, &DeveloperToolsDialog::onQuotedPrintableEncode);
    layout->addWidget(btnQpEnc, 1, 2);

    auto *btnQpDec = new QPushButton(tr("Quoted-Printable Decode"), tab);
    btnQpDec->setMinimumHeight(28);
    btnQpDec->setCursor(Qt::PointingHandCursor);
    btnQpDec->setToolTip(tr("Decode Quoted-Printable content to plain text"));
    connect(btnQpDec, &QPushButton::clicked, this, &DeveloperToolsDialog::onQuotedPrintableDecode);
    layout->addWidget(btnQpDec, 1, 3);

    for (int col = 0; col < 4; ++col) {
        layout->setColumnStretch(col, 1);
    }
    return tab;
}

QWidget *DeveloperToolsDialog::createConverterTab() {
    auto *tab = new QWidget(this);
    auto *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(10);

    // Row 0: Radix Base Conversion
    auto *rowRadix = new QHBoxLayout();
    rowRadix->setSpacing(10);

    auto *lblFrom = new QLabel(tr("From Base:"), tab);
    lblFrom->setStyleSheet(QStringLiteral("font-weight: 500; font-size: 12px;"));
    rowRadix->addWidget(lblFrom);

    m_comboFromRadix = new QComboBox(tab);
    m_comboFromRadix->setMinimumHeight(28);
    m_comboFromRadix->setToolTip(tr("Source numeral base representation"));
    m_comboFromRadix->addItem(tr("Decimal (10)"), 10);
    m_comboFromRadix->addItem(tr("Hexadecimal (16)"), 16);
    m_comboFromRadix->addItem(tr("Binary (2)"), 2);
    m_comboFromRadix->addItem(tr("Octal (8)"), 8);
    rowRadix->addWidget(m_comboFromRadix);

    auto *lblTo = new QLabel(tr("To Base:"), tab);
    lblTo->setStyleSheet(QStringLiteral("font-weight: 500; font-size: 11px;"));
    rowRadix->addWidget(lblTo);

    m_comboToRadix = new QComboBox(tab);
    m_comboToRadix->setMinimumHeight(28);
    m_comboToRadix->setToolTip(tr("Target numeral base representation"));
    m_comboToRadix->addItem(tr("Hexadecimal (16)"), 16);
    m_comboToRadix->addItem(tr("Decimal (10)"), 10);
    m_comboToRadix->addItem(tr("Binary (2)"), 2);
    m_comboToRadix->addItem(tr("Octal (8)"), 8);
    rowRadix->addWidget(m_comboToRadix);

    m_chkUppercaseHex = new QCheckBox(tr("Uppercase Hex"), tab);
    m_chkUppercaseHex->setChecked(true);
    m_chkUppercaseHex->setToolTip(tr("Format hexadecimal digits with uppercase characters (A-F)"));
    rowRadix->addWidget(m_chkUppercaseHex);

    auto *btnConvertRadix = new QPushButton(tr("🔄 Convert Radix"), tab);
    btnConvertRadix->setMinimumHeight(28);
    btnConvertRadix->setMinimumWidth(130);
    btnConvertRadix->setCursor(Qt::PointingHandCursor);
    btnConvertRadix->setToolTip(tr("Convert numbers between selected numeral bases"));
    connect(btnConvertRadix, &QPushButton::clicked, this, &DeveloperToolsDialog::onConvertRadix);
    rowRadix->addWidget(btnConvertRadix);
    layout->addLayout(rowRadix);

    // Row 1: Text & Case Transformations
    auto *rowText = new QHBoxLayout();
    rowText->setSpacing(8);

    auto *btnAsciiHex = new QPushButton(tr("ASCII → Hex"), tab);
    btnAsciiHex->setMinimumHeight(28);
    btnAsciiHex->setCursor(Qt::PointingHandCursor);
    btnAsciiHex->setToolTip(tr("Convert ASCII characters to hexadecimal byte representation"));
    connect(btnAsciiHex, &QPushButton::clicked, this, &DeveloperToolsDialog::onAsciiToHex);
    rowText->addWidget(btnAsciiHex);

    auto *btnHexAscii = new QPushButton(tr("Hex → ASCII"), tab);
    btnHexAscii->setMinimumHeight(28);
    btnHexAscii->setCursor(Qt::PointingHandCursor);
    btnHexAscii->setToolTip(tr("Convert hexadecimal byte stream back to ASCII characters"));
    connect(btnHexAscii, &QPushButton::clicked, this, &DeveloperToolsDialog::onHexToAscii);
    rowText->addWidget(btnHexAscii);

    auto *btnTitle = new QPushButton(tr("Title Case"), tab);
    btnTitle->setMinimumHeight(28);
    btnTitle->setCursor(Qt::PointingHandCursor);
    btnTitle->setToolTip(tr("Convert words to Title Case"));
    connect(btnTitle, &QPushButton::clicked, this, &DeveloperToolsDialog::onCaseTitle);
    rowText->addWidget(btnTitle);

    auto *btnInvert = new QPushButton(tr("Invert Case"), tab);
    btnInvert->setMinimumHeight(28);
    btnInvert->setCursor(Qt::PointingHandCursor);
    btnInvert->setToolTip(tr("Invert character casing (upper <-> lower)"));
    connect(btnInvert, &QPushButton::clicked, this, &DeveloperToolsDialog::onCaseInvert);
    rowText->addWidget(btnInvert);

    auto *btnSentence = new QPushButton(tr("Sentence Case"), tab);
    btnSentence->setMinimumHeight(28);
    btnSentence->setCursor(Qt::PointingHandCursor);
    btnSentence->setToolTip(tr("Capitalize first character of each sentence"));
    connect(btnSentence, &QPushButton::clicked, this, &DeveloperToolsDialog::onCaseSentence);
    rowText->addWidget(btnSentence);

    layout->addLayout(rowText);
    return tab;
}

QWidget *DeveloperToolsDialog::createCryptoTab() {
    auto *tab = new QWidget(this);
    auto *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(10);

    // Row 0: Passphrase & Strength
    auto *rowPass = new QHBoxLayout();
    rowPass->setSpacing(8);

    auto *lblPass = new QLabel(tr("Passphrase:"), tab);
    lblPass->setStyleSheet(QStringLiteral("font-weight: 500; font-size: 11px;"));
    rowPass->addWidget(lblPass);

    m_cryptoPassphrase = new QLineEdit(tab);
    m_cryptoPassphrase->setMinimumHeight(28);
    m_cryptoPassphrase->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    m_cryptoPassphrase->setPlaceholderText(tr("Enter encryption passphrase..."));
    m_cryptoPassphrase->setToolTip(tr("Passphrase for symmetric encryption, decryption, and strength analysis"));
    connect(m_cryptoPassphrase, &QLineEdit::textChanged, this, &DeveloperToolsDialog::onEvaluatePassword);
    rowPass->addWidget(m_cryptoPassphrase, 1);

    m_passwordScoreBar = new QProgressBar(tab);
    m_passwordScoreBar->setRange(0, 4);
    m_passwordScoreBar->setValue(0);
    m_passwordScoreBar->setFixedWidth(110);
    m_passwordScoreBar->setFixedHeight(20);
    m_passwordScoreBar->setTextVisible(false);
    m_passwordScoreBar->setToolTip(tr("Password entropy score (0 to 4)"));
    rowPass->addWidget(m_passwordScoreBar);

    m_passwordStrengthLabel = new QLabel(tr("Empty"), tab);
    m_passwordStrengthLabel->setMinimumWidth(100);
    m_passwordStrengthLabel->setStyleSheet(QStringLiteral("font-weight: 600; font-size: 11px;"));
    rowPass->addWidget(m_passwordStrengthLabel);

    layout->addLayout(rowPass);

    // Row 1: HTML Protection
    auto *rowHtml = new QHBoxLayout();
    rowHtml->setSpacing(8);

    auto *lblHtml = new QLabel(tr("Protection Mode:"), tab);
    lblHtml->setStyleSheet(QStringLiteral("font-weight: 500; font-size: 11px;"));
    rowHtml->addWidget(lblHtml);

    m_comboHtmlEncryptMethod = new QComboBox(tab);
    m_comboHtmlEncryptMethod->setMinimumHeight(28);
    m_comboHtmlEncryptMethod->setToolTip(tr("Select HTML protection algorithm or password encryption"));
    m_comboHtmlEncryptMethod->addItem(tr("JavaScript unescape"), static_cast<int>(HtmlEncryptMethod::JavascriptEscape));
    m_comboHtmlEncryptMethod->addItem(tr("Base64 Obfuscation"), static_cast<int>(HtmlEncryptMethod::Base64));
    m_comboHtmlEncryptMethod->addItem(tr("Password (AES-256-GCM)"), static_cast<int>(HtmlEncryptMethod::PasswordProtected));
    rowHtml->addWidget(m_comboHtmlEncryptMethod, 1);

    auto *btnHtmlEncrypt = new QPushButton(tr("🔒 Encrypt HTML"), tab);
    btnHtmlEncrypt->setMinimumHeight(28);
    btnHtmlEncrypt->setMinimumWidth(130);
    btnHtmlEncrypt->setCursor(Qt::PointingHandCursor);
    btnHtmlEncrypt->setToolTip(tr("Protect HTML file using selected obfuscation or encryption"));
    connect(btnHtmlEncrypt, &QPushButton::clicked, this, &DeveloperToolsDialog::onHtmlEncrypt);
    rowHtml->addWidget(btnHtmlEncrypt);

    auto *btnHtmlDecrypt = new QPushButton(tr("🔓 Decrypt HTML"), tab);
    btnHtmlDecrypt->setMinimumHeight(28);
    btnHtmlDecrypt->setMinimumWidth(130);
    btnHtmlDecrypt->setCursor(Qt::PointingHandCursor);
    btnHtmlDecrypt->setToolTip(tr("Decrypt and restore protected HTML content"));
    connect(btnHtmlDecrypt, &QPushButton::clicked, this, &DeveloperToolsDialog::onHtmlDecrypt);
    rowHtml->addWidget(btnHtmlDecrypt);

    layout->addLayout(rowHtml);
    return tab;
}

QWidget *DeveloperToolsDialog::createDevToolsTab() {
    auto *tab = new QWidget(this);
    auto *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(10);

    // Grid 0: Code Formatters & Minifiers (2 rows x 5 cols)
    auto *gridFmt = new QGridLayout();
    gridFmt->setHorizontalSpacing(8);
    gridFmt->setVerticalSpacing(6);

    auto *btnFmtJson = new QPushButton(tr("Format JSON"), tab);
    btnFmtJson->setMinimumHeight(28);
    btnFmtJson->setCursor(Qt::PointingHandCursor);
    btnFmtJson->setToolTip(tr("Format and beautify JSON with indentation"));
    connect(btnFmtJson, &QPushButton::clicked, this, &DeveloperToolsDialog::onFormatJson);
    gridFmt->addWidget(btnFmtJson, 0, 0);

    auto *btnMinJson = new QPushButton(tr("Minify JSON"), tab);
    btnMinJson->setMinimumHeight(28);
    btnMinJson->setCursor(Qt::PointingHandCursor);
    btnMinJson->setToolTip(tr("Minify JSON by removing whitespace and linebreaks"));
    connect(btnMinJson, &QPushButton::clicked, this, &DeveloperToolsDialog::onMinifyJson);
    gridFmt->addWidget(btnMinJson, 1, 0);

    auto *btnFmtXml = new QPushButton(tr("Format XML"), tab);
    btnFmtXml->setMinimumHeight(28);
    btnFmtXml->setCursor(Qt::PointingHandCursor);
    btnFmtXml->setToolTip(tr("Format and indent XML document"));
    connect(btnFmtXml, &QPushButton::clicked, this, &DeveloperToolsDialog::onFormatXml);
    gridFmt->addWidget(btnFmtXml, 0, 1);

    auto *btnMinXml = new QPushButton(tr("Minify XML"), tab);
    btnMinXml->setMinimumHeight(28);
    btnMinXml->setCursor(Qt::PointingHandCursor);
    btnMinXml->setToolTip(tr("Minify XML by stripping extra whitespace"));
    connect(btnMinXml, &QPushButton::clicked, this, &DeveloperToolsDialog::onMinifyXml);
    gridFmt->addWidget(btnMinXml, 1, 1);

    auto *btnFmtHtml = new QPushButton(tr("Format HTML"), tab);
    btnFmtHtml->setMinimumHeight(28);
    btnFmtHtml->setCursor(Qt::PointingHandCursor);
    btnFmtHtml->setToolTip(tr("Format and beautify HTML document"));
    connect(btnFmtHtml, &QPushButton::clicked, this, &DeveloperToolsDialog::onFormatHtml);
    gridFmt->addWidget(btnFmtHtml, 0, 2);

    auto *btnMinHtml = new QPushButton(tr("Minify HTML"), tab);
    btnMinHtml->setMinimumHeight(28);
    btnMinHtml->setCursor(Qt::PointingHandCursor);
    btnMinHtml->setToolTip(tr("Minify HTML markup"));
    connect(btnMinHtml, &QPushButton::clicked, this, &DeveloperToolsDialog::onMinifyHtml);
    gridFmt->addWidget(btnMinHtml, 1, 2);

    auto *btnFmtCss = new QPushButton(tr("Format CSS"), tab);
    btnFmtCss->setMinimumHeight(28);
    btnFmtCss->setCursor(Qt::PointingHandCursor);
    btnFmtCss->setToolTip(tr("Format CSS stylesheets with clean rules and indentation"));
    connect(btnFmtCss, &QPushButton::clicked, this, &DeveloperToolsDialog::onFormatCss);
    gridFmt->addWidget(btnFmtCss, 0, 3);

    auto *btnMinCss = new QPushButton(tr("Minify CSS"), tab);
    btnMinCss->setMinimumHeight(28);
    btnMinCss->setCursor(Qt::PointingHandCursor);
    btnMinCss->setToolTip(tr("Minify CSS stylesheets by compressing rules"));
    connect(btnMinCss, &QPushButton::clicked, this, &DeveloperToolsDialog::onMinifyCss);
    gridFmt->addWidget(btnMinCss, 1, 3);

    auto *btnFmtJs = new QPushButton(tr("Format JS"), tab);
    btnFmtJs->setMinimumHeight(28);
    btnFmtJs->setCursor(Qt::PointingHandCursor);
    btnFmtJs->setToolTip(tr("Format and beautify JavaScript / TypeScript source code"));
    connect(btnFmtJs, &QPushButton::clicked, this, &DeveloperToolsDialog::onFormatJs);
    gridFmt->addWidget(btnFmtJs, 0, 4);

    auto *btnMinJs = new QPushButton(tr("Minify JS"), tab);
    btnMinJs->setMinimumHeight(28);
    btnMinJs->setCursor(Qt::PointingHandCursor);
    btnMinJs->setToolTip(tr("Minify JavaScript by stripping comments and whitespace"));
    connect(btnMinJs, &QPushButton::clicked, this, &DeveloperToolsDialog::onMinifyJs);
    gridFmt->addWidget(btnMinJs, 1, 4);

    for (int col = 0; col < 5; ++col) {
        gridFmt->setColumnStretch(col, 1);
    }
    layout->addLayout(gridFmt);

    // Row 1: Utilities & Generators
    auto *rowMisc = new QHBoxLayout();
    rowMisc->setSpacing(8);

    auto *btnJwt = new QPushButton(tr("🔑 Decode JWT Token"), tab);
    btnJwt->setMinimumHeight(28);
    btnJwt->setCursor(Qt::PointingHandCursor);
    btnJwt->setToolTip(tr("Decode JSON Web Token (JWT) header, claims, and payload"));
    connect(btnJwt, &QPushButton::clicked, this, &DeveloperToolsDialog::onParseJwt);
    rowMisc->addWidget(btnJwt);

    auto *lblIndent = new QLabel(tr("JSON Indent:"), tab);
    lblIndent->setStyleSheet(QStringLiteral("font-weight: 500; font-size: 11px;"));
    rowMisc->addWidget(lblIndent);

    m_spinJsonIndent = new QSpinBox(tab);
    m_spinJsonIndent->setRange(1, 8);
    m_spinJsonIndent->setValue(2);
    m_spinJsonIndent->setMinimumHeight(28);
    m_spinJsonIndent->setToolTip(tr("Indentation width in spaces for JSON formatting (1 to 8)"));
    rowMisc->addWidget(m_spinJsonIndent);

    rowMisc->addSpacing(4);

    auto *btnUuid = new QPushButton(tr("🎲 Generate UUIDv4"), tab);
    btnUuid->setMinimumHeight(28);
    btnUuid->setCursor(Qt::PointingHandCursor);
    btnUuid->setToolTip(tr("Generate random RFC 4122 Version 4 UUID"));
    connect(btnUuid, &QPushButton::clicked, this, &DeveloperToolsDialog::onGenerateUuid);
    rowMisc->addWidget(btnUuid);

    m_chkUuidUpper = new QCheckBox(tr("Upper"), tab);
    m_chkUuidUpper->setToolTip(tr("Format UUID with uppercase hex characters"));
    rowMisc->addWidget(m_chkUuidUpper);
    m_chkUuidBraces = new QCheckBox(tr("{ }"), tab);
    m_chkUuidBraces->setToolTip(tr("Wrap UUID in curly braces { ... }"));
    rowMisc->addWidget(m_chkUuidBraces);
    m_chkUuidDashes = new QCheckBox(tr("Dashes"), tab);
    m_chkUuidDashes->setChecked(true);
    m_chkUuidDashes->setToolTip(tr("Include hyphens in UUID representation"));
    rowMisc->addWidget(m_chkUuidDashes);

    rowMisc->addSpacing(4);

    auto *btnEpoch = new QPushButton(tr("⏱️ Current Epoch & ISO"), tab);
    btnEpoch->setMinimumHeight(28);
    btnEpoch->setCursor(Qt::PointingHandCursor);
    btnEpoch->setToolTip(tr("Display current Unix timestamp in seconds, milliseconds, and ISO 8601 string"));
    connect(btnEpoch, &QPushButton::clicked, this, &DeveloperToolsDialog::onCurrentEpoch);
    rowMisc->addWidget(btnEpoch);

    layout->addLayout(rowMisc);
    return tab;
}

void DeveloperToolsDialog::onLoadFromEditor() {
    if (!m_mainWindow) {
        return;
    }
    auto *editor = m_mainWindow->currentEditor();
    if (!editor) {
        return;
    }

    const QByteArray selBytes = editor->getSelText();
    if (!selBytes.isEmpty()) {
        m_inputEdit->setPlainText(QString::fromUtf8(selBytes));
        m_statusLabel->setText(tr("Loaded selection from editor (%1 chars)").arg(selBytes.size()));
    } else {
        const QString fullText = QString::fromUtf8(editor->getText(editor->length() + 1));
        m_inputEdit->setPlainText(fullText);
        m_statusLabel->setText(tr("Loaded active document (%1 chars)").arg(fullText.size()));
    }
}

void DeveloperToolsDialog::onApplyToEditor() {
    if (!m_mainWindow) {
        return;
    }
    auto *editor = m_mainWindow->currentEditor();
    if (!editor) {
        return;
    }

    const QString outputText = m_outputEdit->toPlainText();
    if (outputText.isEmpty()) {
        return;
    }

    const QByteArray utf8 = outputText.toUtf8();
    if (!editor->selectionEmpty()) {
        editor->replaceSel(utf8.constData());
    } else {
        editor->setText(utf8.constData());
    }
    m_statusLabel->setText(tr("Applied to editor successfully."));
}

void DeveloperToolsDialog::onCopyOutput() {
    const QString text = m_outputEdit->toPlainText();
    if (!text.isEmpty()) {
        QGuiApplication::clipboard()->setText(text);
        m_statusLabel->setText(tr("Copied output to clipboard."));
    }
}

namespace {

QCryptographicHash::Algorithm parseHashAlgorithm(const QString &algoStr) {
    if (algoStr == QStringLiteral("SHA-512")) {
        return QCryptographicHash::Sha512;
    }
    if (algoStr == QStringLiteral("SHA-1")) {
        return QCryptographicHash::Sha1;
    }
    if (algoStr == QStringLiteral("MD5")) {
        return QCryptographicHash::Md5;
    }
    if (algoStr == QStringLiteral("SHA3-256")) {
        return QCryptographicHash::Sha3_256;
    }
    if (algoStr == QStringLiteral("SHA3-512")) {
        return QCryptographicHash::Sha3_512;
    }
    if (algoStr == QStringLiteral("Keccak-256")) {
        return QCryptographicHash::Keccak_256;
    }
    if (algoStr == QStringLiteral("Keccak-512")) {
        return QCryptographicHash::Keccak_512;
    }
    return QCryptographicHash::Sha256;
}

} // namespace

void DeveloperToolsDialog::onComputeHash() {
    const QString input = m_inputEdit->toPlainText();
    const QString algoStr = m_comboHashAlgo->currentText();
    const QCryptographicHash::Algorithm algo = parseHashAlgorithm(algoStr);

    const QByteArray hash = QCryptographicHash::hash(input.toUtf8(), algo);
    m_outputEdit->setPlainText(QString::fromLatin1(hash.toHex()));
    m_statusLabel->setText(tr("Generated %1 hash").arg(algoStr));
    onCompareHash();
}

void DeveloperToolsDialog::onCompareHash() {
    const QString expected = m_compareHashEdit->text().trimmed();
    const QString computed = m_outputEdit->toPlainText().trimmed();
    if (expected.isEmpty() || computed.isEmpty()) {
        m_hashCompareResult->setText(QString());
        return;
    }

    if (QString::compare(expected, computed, Qt::CaseInsensitive) == 0) {
        m_hashCompareResult->setStyleSheet(QStringLiteral("color: #3fb950; font-weight: bold; padding: 2px 6px; border: 1px solid #3fb950; border-radius: 4px;"));
        m_hashCompareResult->setText(tr("✓ MATCH"));
    } else {
        m_hashCompareResult->setStyleSheet(QStringLiteral("color: #f85149; font-weight: bold; padding: 2px 6px; border: 1px solid #f85149; border-radius: 4px;"));
        m_hashCompareResult->setText(tr("✗ MISMATCH"));
    }
}

void DeveloperToolsDialog::onBrowseFileForHash() {
    const QString path = QFileDialog::getOpenFileName(this, tr("Select File to Hash"));
    if (path.isEmpty()) {
        return;
    }

    m_filePathEdit->setText(path);
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        m_statusLabel->setText(tr("Cannot open file: %1").arg(file.errorString()));
        return;
    }

    const QString algoStr = m_comboHashAlgo->currentText();
    const QCryptographicHash::Algorithm algo = parseHashAlgorithm(algoStr);

    QCryptographicHash hasher(algo);
    hasher.addData(&file);
    const QByteArray hash = hasher.result();
    m_outputEdit->setPlainText(QString::fromLatin1(hash.toHex()));
    m_statusLabel->setText(tr("Calculated %1 for %2 (%3 bytes)").arg(algoStr, QFileInfo(path).fileName()).arg(file.size()));
    onCompareHash();
}

void DeveloperToolsDialog::onBase64Encode() {
    m_outputEdit->setPlainText(DeveloperToolsEngine::base64EncodeLines(m_inputEdit->toPlainText()));
    m_statusLabel->setText(tr("Base64 encoded."));
}

void DeveloperToolsDialog::onBase64Decode() {
    m_outputEdit->setPlainText(DeveloperToolsEngine::base64DecodeLines(m_inputEdit->toPlainText()));
    m_statusLabel->setText(tr("Base64 decoded."));
}

void DeveloperToolsDialog::onUrlEncode() {
    m_outputEdit->setPlainText(DeveloperToolsEngine::urlEncode(m_inputEdit->toPlainText()));
    m_statusLabel->setText(tr("URL encoded."));
}

void DeveloperToolsDialog::onUrlDecode() {
    m_outputEdit->setPlainText(DeveloperToolsEngine::urlDecode(m_inputEdit->toPlainText()));
    m_statusLabel->setText(tr("URL decoded."));
}

void DeveloperToolsDialog::onHtmlEntityEncode() {
    m_outputEdit->setPlainText(DeveloperToolsEngine::htmlEntityEncode(m_inputEdit->toPlainText()));
    m_statusLabel->setText(tr("HTML entities encoded."));
}

void DeveloperToolsDialog::onHtmlEntityDecode() {
    m_outputEdit->setPlainText(DeveloperToolsEngine::htmlEntityDecode(m_inputEdit->toPlainText()));
    m_statusLabel->setText(tr("HTML entities decoded."));
}

void DeveloperToolsDialog::onQuotedPrintableEncode() {
    m_outputEdit->setPlainText(DeveloperToolsEngine::quotedPrintableEncode(m_inputEdit->toPlainText()));
    m_statusLabel->setText(tr("Quoted-Printable encoded."));
}

void DeveloperToolsDialog::onQuotedPrintableDecode() {
    m_outputEdit->setPlainText(DeveloperToolsEngine::quotedPrintableDecode(m_inputEdit->toPlainText()));
    m_statusLabel->setText(tr("Quoted-Printable decoded."));
}

void DeveloperToolsDialog::onConvertRadix() {
    QString err;
    const int fromBase = m_comboFromRadix->currentData().toInt();
    const int toBase = m_comboToRadix->currentData().toInt();
    const bool upper = m_chkUppercaseHex->isChecked();
    const QString result = DeveloperToolsEngine::convertRadix(m_inputEdit->toPlainText(), fromBase, toBase, upper, &err);
    if (!err.isEmpty()) {
        m_outputEdit->setPlainText(tr("Error: %1").arg(err));
        m_statusLabel->setText(err);
    } else {
        m_outputEdit->setPlainText(result);
        m_statusLabel->setText(tr("Converted from Base %1 to Base %2").arg(fromBase).arg(toBase));
    }
}

void DeveloperToolsDialog::onAsciiToHex() {
    m_outputEdit->setPlainText(DeveloperToolsEngine::asciiToHex(m_inputEdit->toPlainText()));
    m_statusLabel->setText(tr("Converted ASCII to Hex."));
}

void DeveloperToolsDialog::onHexToAscii() {
    QString err;
    const QString res = DeveloperToolsEngine::hexToAscii(m_inputEdit->toPlainText(), &err);
    if (!err.isEmpty()) {
        m_outputEdit->setPlainText(tr("Hex Error: %1").arg(err));
        m_statusLabel->setText(err);
    } else {
        m_outputEdit->setPlainText(res);
        m_statusLabel->setText(tr("Converted Hex to ASCII."));
    }
}

void DeveloperToolsDialog::onCaseTitle() {
    m_outputEdit->setPlainText(DeveloperToolsEngine::toTitleCase(m_inputEdit->toPlainText()));
    m_statusLabel->setText(tr("Converted to Title Case."));
}

void DeveloperToolsDialog::onCaseInvert() {
    m_outputEdit->setPlainText(DeveloperToolsEngine::toInvertCase(m_inputEdit->toPlainText()));
    m_statusLabel->setText(tr("Inverted character cases."));
}

void DeveloperToolsDialog::onCaseSentence() {
    m_outputEdit->setPlainText(DeveloperToolsEngine::toSentenceCase(m_inputEdit->toPlainText()));
    m_statusLabel->setText(tr("Converted to Sentence Case."));
}

void DeveloperToolsDialog::onEvaluatePassword() {
    const QString pwd = m_cryptoPassphrase->text();
    const auto info = DeveloperToolsEngine::evaluatePasswordStrength(pwd);
    m_passwordScoreBar->setValue(info.score);
    m_passwordStrengthLabel->setText(tr("%1 (%2 bits)").arg(info.label).arg(info.entropy));
    if (info.score >= 3) {
        m_passwordStrengthLabel->setStyleSheet(QStringLiteral("color: #3fb950; font-weight: bold; font-size: 11px;"));
    } else if (info.score == 2) {
        m_passwordStrengthLabel->setStyleSheet(QStringLiteral("color: #d29922; font-weight: bold; font-size: 11px;"));
    } else {
        m_passwordStrengthLabel->setStyleSheet(QStringLiteral("color: #f85149; font-weight: bold; font-size: 11px;"));
    }
}

void DeveloperToolsDialog::onHtmlEncrypt() {
    QString err;
    const auto method = static_cast<HtmlEncryptMethod>(m_comboHtmlEncryptMethod->currentData().toInt());
    const QString pass = m_cryptoPassphrase->text();
    const QString res = DeveloperToolsEngine::encryptHtml(m_inputEdit->toPlainText(), method, pass, &err);
    if (!err.isEmpty()) {
        m_outputEdit->setPlainText(tr("Encryption Error: %1").arg(err));
        m_statusLabel->setText(err);
    } else {
        m_outputEdit->setPlainText(res);
        m_statusLabel->setText(tr("HTML successfully encrypted."));
    }
}

void DeveloperToolsDialog::onHtmlDecrypt() {
    QString err;
    const QString res = DeveloperToolsEngine::decryptHtml(m_inputEdit->toPlainText(), &err);
    if (!err.isEmpty()) {
        m_outputEdit->setPlainText(tr("Decryption Error: %1").arg(err));
        m_statusLabel->setText(err);
    } else {
        m_outputEdit->setPlainText(res);
        m_statusLabel->setText(tr("HTML successfully decrypted."));
    }
}

void DeveloperToolsDialog::onFormatJson() {
    QString err;
    const int indent = m_spinJsonIndent ? m_spinJsonIndent->value() : 2;
    const QString res = DeveloperToolsEngine::formatJson(m_inputEdit->toPlainText(), indent, &err);
    if (!err.isEmpty()) {
        m_outputEdit->setPlainText(tr("JSON Error: %1").arg(err));
        m_statusLabel->setText(err);
    } else {
        m_outputEdit->setPlainText(res);
        m_statusLabel->setText(tr("JSON formatted."));
    }
}

void DeveloperToolsDialog::onMinifyJson() {
    QString err;
    const QString res = DeveloperToolsEngine::minifyJson(m_inputEdit->toPlainText(), &err);
    if (!err.isEmpty()) {
        m_outputEdit->setPlainText(tr("JSON Error: %1").arg(err));
        m_statusLabel->setText(err);
    } else {
        m_outputEdit->setPlainText(res);
        m_statusLabel->setText(tr("JSON minified."));
    }
}

void DeveloperToolsDialog::onFormatXml() {
    QString err;
    const int indent = m_spinJsonIndent ? m_spinJsonIndent->value() : 2;
    const QString res = DeveloperToolsEngine::formatXml(m_inputEdit->toPlainText(), indent, &err);
    if (!err.isEmpty()) {
        m_outputEdit->setPlainText(tr("XML Error: %1").arg(err));
        m_statusLabel->setText(err);
    } else {
        m_outputEdit->setPlainText(res);
        m_statusLabel->setText(tr("XML formatted."));
    }
}

void DeveloperToolsDialog::onMinifyXml() {
    QString err;
    const QString res = DeveloperToolsEngine::minifyXml(m_inputEdit->toPlainText(), &err);
    if (!err.isEmpty()) {
        m_outputEdit->setPlainText(tr("XML Error: %1").arg(err));
        m_statusLabel->setText(err);
    } else {
        m_outputEdit->setPlainText(res);
        m_statusLabel->setText(tr("XML minified."));
    }
}

void DeveloperToolsDialog::onFormatHtml() {
    QString err;
    const int indent = m_spinJsonIndent ? m_spinJsonIndent->value() : 2;
    const QString res = DeveloperToolsEngine::formatHtml(m_inputEdit->toPlainText(), indent, &err);
    if (!err.isEmpty()) {
        m_outputEdit->setPlainText(tr("HTML Error: %1").arg(err));
        m_statusLabel->setText(err);
    } else {
        m_outputEdit->setPlainText(res);
        m_statusLabel->setText(tr("HTML formatted."));
    }
}

void DeveloperToolsDialog::onMinifyHtml() {
    QString err;
    const QString res = DeveloperToolsEngine::minifyHtml(m_inputEdit->toPlainText(), &err);
    if (!err.isEmpty()) {
        m_outputEdit->setPlainText(tr("HTML Error: %1").arg(err));
        m_statusLabel->setText(err);
    } else {
        m_outputEdit->setPlainText(res);
        m_statusLabel->setText(tr("HTML minified."));
    }
}

void DeveloperToolsDialog::onFormatCss() {
    QString err;
    const QString res = DeveloperToolsEngine::formatCss(m_inputEdit->toPlainText(), &err);
    if (!err.isEmpty()) {
        m_outputEdit->setPlainText(tr("CSS Error: %1").arg(err));
        m_statusLabel->setText(err);
    } else {
        m_outputEdit->setPlainText(res);
        m_statusLabel->setText(tr("CSS formatted."));
    }
}

void DeveloperToolsDialog::onMinifyCss() {
    QString err;
    const QString res = DeveloperToolsEngine::minifyCss(m_inputEdit->toPlainText(), &err);
    if (!err.isEmpty()) {
        m_outputEdit->setPlainText(tr("CSS Error: %1").arg(err));
        m_statusLabel->setText(err);
    } else {
        m_outputEdit->setPlainText(res);
        m_statusLabel->setText(tr("CSS minified."));
    }
}

void DeveloperToolsDialog::onFormatJs() {
    QString err;
    const QString res = DeveloperToolsEngine::formatJs(m_inputEdit->toPlainText(), &err);
    if (!err.isEmpty()) {
        m_outputEdit->setPlainText(tr("JS Error: %1").arg(err));
        m_statusLabel->setText(err);
    } else {
        m_outputEdit->setPlainText(res);
        m_statusLabel->setText(tr("JS formatted."));
    }
}

void DeveloperToolsDialog::onMinifyJs() {
    QString err;
    const QString res = DeveloperToolsEngine::minifyJs(m_inputEdit->toPlainText(), &err);
    if (!err.isEmpty()) {
        m_outputEdit->setPlainText(tr("JS Error: %1").arg(err));
        m_statusLabel->setText(err);
    } else {
        m_outputEdit->setPlainText(res);
        m_statusLabel->setText(tr("JS minified."));
    }
}

void DeveloperToolsDialog::onParseJwt() {
    const auto info = DeveloperToolsEngine::parseJwt(m_inputEdit->toPlainText());
    if (!info.isValid) {
        m_outputEdit->setPlainText(tr("JWT Error: %1").arg(info.error));
        m_statusLabel->setText(info.error);
    } else {
        QString text = QStringLiteral("=== JWT HEADER ===\n%1\n\n=== JWT PAYLOAD ===\n%2\n\n=== SIGNATURE ===\n%3")
                           .arg(info.headerJson, info.payloadJson, info.signature);
        if (info.expirationDate.isValid()) {
            text += tr("\n\nExpiration Date: %1 (UTC)").arg(info.expirationDate.toString(Qt::ISODate));
        }
        if (info.issuedAtDate.isValid()) {
            text += tr("\nIssued At Date: %1 (UTC)").arg(info.issuedAtDate.toString(Qt::ISODate));
        }
        m_outputEdit->setPlainText(text);
        m_statusLabel->setText(tr("JWT decoded successfully."));
    }
}

void DeveloperToolsDialog::onGenerateUuid() {
    const bool upper = m_chkUuidUpper && m_chkUuidUpper->isChecked();
    const bool braces = m_chkUuidBraces && m_chkUuidBraces->isChecked();
    const bool dashes = m_chkUuidDashes && m_chkUuidDashes->isChecked();
    const QString uuid = DeveloperToolsEngine::generateUuidV4(upper, braces, dashes);
    m_outputEdit->setPlainText(uuid);
    m_statusLabel->setText(tr("Generated UUIDv4."));
}

void DeveloperToolsDialog::onCurrentEpoch() {
    const qint64 now = QDateTime::currentSecsSinceEpoch();
    const QString iso = DeveloperToolsEngine::epochToIso(now);
    m_outputEdit->setPlainText(tr("Epoch: %1\nISO 8601: %2").arg(now).arg(iso));
    m_statusLabel->setText(tr("Epoch generated."));
}
