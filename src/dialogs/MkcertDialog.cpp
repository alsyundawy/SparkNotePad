/*
 * This file is part of SparkNotePad.
 * Copyright 2026 Harry DS Alsyundawy (SparkNotePad Maintainer)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "MkcertDialog.h"
#include "ModernMessageBox.h"
#include "../features/ThemeManager.h"
#include "../features/ThemePaletteData.h"

#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QProcess>
#include <QRegularExpression>
#include <QFontDatabase>
#include <QFont>
#include <QPushButton>
#include <QStandardPaths>
#include <QTextEdit>
#include <QVBoxLayout>

MkcertDialog::MkcertDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle(tr("Generate Development SSL Certificate (mkcert / OpenSSL)"));
    resize(580, 480);
    setModal(true);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(16);

    // Title & Description
    QLabel *titleLabel = new QLabel(tr("Local Development SSL / TLS Certificate Generator"), this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(titleFont.pointSize() + 2);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);

    QLabel *descLabel = new QLabel(tr("Generate locally trusted certificates using <b>mkcert</b> (automatic root CA) "
                                      "or fallback to <b>OpenSSL</b> self-signed certificates for local development."),
                                   this);
    descLabel->setWordWrap(true);
    mainLayout->addWidget(descLabel);

    // Status Banner
    m_statusLabel = new QLabel(this);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setStyleSheet("padding: 8px 12px; border-radius: 6px;");
    mainLayout->addWidget(m_statusLabel);

    // Form Section
    QGridLayout *formLayout = new QGridLayout();
    formLayout->setSpacing(10);

    formLayout->addWidget(new QLabel(tr("Domains / IPs:"), this), 0, 0);
    m_domainsEdit = new QLineEdit(this);
    m_domainsEdit->setText(QStringLiteral("localhost 127.0.0.1 ::1"));
    m_domainsEdit->setPlaceholderText(tr("e.g. localhost 127.0.0.1 dev.local *.test"));
    formLayout->addWidget(m_domainsEdit, 0, 1, 1, 2);

    formLayout->addWidget(new QLabel(tr("Output Directory:"), this), 1, 0);
    m_dirEdit = new QLineEdit(this);
    QString defaultDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    if (defaultDir.isEmpty()) {
        defaultDir = QDir::homePath();
    }
    m_dirEdit->setText(defaultDir);
    formLayout->addWidget(m_dirEdit, 1, 1);

    m_browseDirBtn = new QPushButton(tr("Browse..."), this);
    connect(m_browseDirBtn, &QPushButton::clicked, this, &MkcertDialog::onBrowseDirClicked);
    formLayout->addWidget(m_browseDirBtn, 1, 2);

    mainLayout->addLayout(formLayout);

    // Output view
    QLabel *outLabel = new QLabel(tr("Command Output / Certificate Details:"), this);
    mainLayout->addWidget(outLabel);

    m_outputView = new QTextEdit(this);
    m_outputView->setReadOnly(true);
    m_outputView->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    m_outputView->setPlaceholderText(tr("Output logs and certificate paths will appear here..."));
    mainLayout->addWidget(m_outputView, 1);

    // Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    m_installCaBtn = new QPushButton(tr("Install Root CA (mkcert -install)"), this);
    connect(m_installCaBtn, &QPushButton::clicked, this, &MkcertDialog::onInstallCaClicked);
    btnLayout->addWidget(m_installCaBtn);

    btnLayout->addStretch();

    m_generateBtn = new QPushButton(tr("Generate Certificates"), this);
    m_generateBtn->setDefault(true);
    connect(m_generateBtn, &QPushButton::clicked, this, &MkcertDialog::onGenerateClicked);
    btnLayout->addWidget(m_generateBtn);

    m_closeBtn = new QPushButton(tr("Close"), this);
    connect(m_closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    btnLayout->addWidget(m_closeBtn);

    mainLayout->addLayout(btnLayout);

    checkToolsStatus();
    applyTheme();
}

void MkcertDialog::applyTheme() {
    const ThemePalette *palette = ThemeManager::instance()->currentPalette();
    bool dark = ThemeManager::instance()->isDarkActive();
    QString bgColor =
        palette ? QString::fromLatin1(palette->win) : (dark ? QStringLiteral("#1e1e1e") : QStringLiteral("#f6f8fa"));
    QString fgColor =
        palette ? QString::fromLatin1(palette->txtPri) : (dark ? QStringLiteral("#e0e0e0") : QStringLiteral("#24292f"));
    QString cardBg =
        palette ? QString::fromLatin1(palette->card) : (dark ? QStringLiteral("#252526") : QStringLiteral("#ffffff"));
    QString borderColor =
        palette ? QString::fromLatin1(palette->border) : (dark ? QStringLiteral("#33373e") : QStringLiteral("#d0d7de"));

    setStyleSheet(QString("QDialog { background-color: %1; color: %2; }"
                          "QLineEdit { background-color: %3; color: %2; border: 1px solid %4; border-radius: 6px; "
                          "padding: 6px 10px; }"
                          "QTextEdit { background-color: %3; color: %2; border: 1px solid %4; border-radius: 6px; }"
                          "QPushButton { background-color: %3; color: %2; border: 1px solid %4; border-radius: 6px; "
                          "padding: 6px 14px; font-weight: 500; }"
                          "QPushButton:hover { background-color: %4; }")
                      .arg(bgColor, fgColor, cardBg, borderColor));
}

void MkcertDialog::checkToolsStatus() {
    m_mkcertPath = QStandardPaths::findExecutable("mkcert");
    if (m_mkcertPath.isEmpty()) {
        static const QStringList extraPaths = {"/usr/local/bin/mkcert", "/opt/homebrew/bin/mkcert"};
        const auto it =
            std::find_if(extraPaths.cbegin(), extraPaths.cend(), [](const QString &p) { return QFileInfo::exists(p); });
        if (it != extraPaths.cend()) {
            m_mkcertPath = *it;
        }
    }

    m_opensslPath = QStandardPaths::findExecutable("openssl");
    if (m_opensslPath.isEmpty()) {
        static const QStringList extraPaths = {"/usr/bin/openssl", "/usr/local/bin/openssl",
                                               "/opt/homebrew/bin/openssl"};
        const auto it =
            std::find_if(extraPaths.cbegin(), extraPaths.cend(), [](const QString &p) { return QFileInfo::exists(p); });
        if (it != extraPaths.cend()) {
            m_opensslPath = *it;
        }
    }

    if (!m_mkcertPath.isEmpty()) {
        m_statusLabel->setText(
            tr("<b>mkcert detected:</b> %1<br>Locally trusted CA certificates are supported.").arg(m_mkcertPath));
        m_statusLabel->setStyleSheet(
            "background-color: rgba(46, 125, 50, 0.15); color: #2e7d32; border: 1px solid #2e7d32; border-radius: 6px; "
            "padding: 8px;");
        m_installCaBtn->setEnabled(true);
    } else if (!m_opensslPath.isEmpty()) {
        m_statusLabel->setText(
            tr("<b>mkcert not found, using OpenSSL:</b> %1<br>Self-signed certificates will be generated.")
                .arg(m_opensslPath));
        m_statusLabel->setStyleSheet(
            "background-color: rgba(239, 108, 0, 0.15); color: #ef6c00; border: 1px solid #ef6c00; border-radius: 6px; "
            "padding: 8px;");
        m_installCaBtn->setEnabled(false);
    } else {
        m_statusLabel->setText(
            tr("<b>Neither mkcert nor openssl was found in PATH.</b> Please install mkcert (e.g. <code>brew install "
               "mkcert</code>) or openssl."));
        m_statusLabel->setStyleSheet(
            "background-color: rgba(198, 40, 40, 0.15); color: #c62828; border: 1px solid #c62828; border-radius: 6px; "
            "padding: 8px;");
        m_installCaBtn->setEnabled(false);
        m_generateBtn->setEnabled(false);
    }
}

void MkcertDialog::onBrowseDirClicked() {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Output Directory"), m_dirEdit->text());
    if (!dir.isEmpty()) {
        m_dirEdit->setText(dir);
    }
}

void MkcertDialog::onInstallCaClicked() {
    if (m_mkcertPath.isEmpty()) {
        return;
    }

    m_outputView->append(tr("Running: %1 -install ...\n").arg(m_mkcertPath));
    QProcess proc;
    proc.start(m_mkcertPath, QStringList() << "-install");
    if (proc.waitForFinished(10000)) {
        QString out = QString::fromUtf8(proc.readAllStandardOutput());
        QString err = QString::fromUtf8(proc.readAllStandardError());
        m_outputView->append(out);
        if (!err.isEmpty()) {
            m_outputView->append(err);
        }
        if (proc.exitCode() == 0) {
            ModernMessageBox::information(
                this, tr("mkcert CA Installed"),
                tr("The local root Certificate Authority (CA) was installed successfully into system trust stores!"));
        } else {
            ModernMessageBox::warning(this, tr("mkcert Installation Warning"),
                                      tr("mkcert finished with status code: %1").arg(proc.exitCode()));
        }
    } else {
        m_outputView->append(tr("mkcert installation timed out.\n"));
    }
}

void MkcertDialog::onGenerateClicked() {
    QString domainsStr = m_domainsEdit->text().trimmed();
    if (domainsStr.isEmpty()) {
        ModernMessageBox::warning(this, tr("Missing Domains"),
                                  tr("Please specify at least one domain name or IP address (e.g. localhost)."));
        return;
    }

    QString targetDir = m_dirEdit->text().trimmed();
    if (targetDir.isEmpty() || !QDir(targetDir).exists()) {
        ModernMessageBox::warning(this, tr("Invalid Directory"), tr("Please select a valid output directory."));
        return;
    }

    QStringList domainList = domainsStr.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    QString primaryDomain = domainList.first();
    primaryDomain.remove('*');
    primaryDomain.remove('.');
    if (primaryDomain.isEmpty()) {
        primaryDomain = QStringLiteral("cert");
    }

    QString certFile = QDir(targetDir).filePath(primaryDomain + ".pem");
    QString keyFile = QDir(targetDir).filePath(primaryDomain + "-key.pem");

    m_outputView->clear();

    if (!m_mkcertPath.isEmpty()) {
        m_outputView->append(tr("Generating certificate with mkcert for: %1\n").arg(domainList.join(" ")));
        QProcess proc;
        proc.setWorkingDirectory(targetDir);
        QStringList args;
        args << "-cert-file" << certFile << "-key-file" << keyFile;
        args.append(domainList);

        proc.start(m_mkcertPath, args);
        if (proc.waitForFinished(15000)) {
            QString out = QString::fromUtf8(proc.readAllStandardOutput());
            QString err = QString::fromUtf8(proc.readAllStandardError());
            m_outputView->append(out);
            if (!err.isEmpty()) {
                m_outputView->append(err);
            }

            if (proc.exitCode() == 0 && QFileInfo::exists(certFile)) {
                m_outputView->append(tr("\nSUCCESS!\nCertificate: %1\nPrivate Key: %2\n").arg(certFile, keyFile));
                ModernMessageBox::information(
                    this, tr("Certificate Created"),
                    tr("Successfully generated locally trusted SSL certificate!\n\nCertificate: %1\nKey: %2")
                        .arg(certFile, keyFile));
            } else {
                ModernMessageBox::warning(this, tr("Generation Warning"),
                                          tr("mkcert completed with error code: %1").arg(proc.exitCode()));
            }
        }
    } else if (!m_opensslPath.isEmpty()) {
        m_outputView->append(tr("Generating self-signed certificate with OpenSSL for: %1\n").arg(domainList.join(" ")));
        QProcess proc;
        proc.setWorkingDirectory(targetDir);
        QStringList args;
        args << "req" << "-x509" << "-newkey" << "rsa:2048" << "-nodes" << "-sha256"
             << "-days" << "365"
             << "-keyout" << keyFile << "-out" << certFile << "-subj"
             << QStringLiteral("/CN=%1/O=SparkNotePad Dev").arg(domainList.first());

        proc.start(m_opensslPath, args);
        if (proc.waitForFinished(15000)) {
            QString out = QString::fromUtf8(proc.readAllStandardOutput());
            QString err = QString::fromUtf8(proc.readAllStandardError());
            m_outputView->append(out);
            if (!err.isEmpty()) {
                m_outputView->append(err);
            }

            if (proc.exitCode() == 0 && QFileInfo::exists(certFile)) {
                m_outputView->append(
                    tr("\nSUCCESS (OpenSSL)!\nCertificate: %1\nPrivate Key: %2\n").arg(certFile, keyFile));
                ModernMessageBox::information(
                    this, tr("Certificate Created"),
                    tr("Successfully generated self-signed SSL certificate with OpenSSL!\n\nCertificate: %1\nKey: %2")
                        .arg(certFile, keyFile));
            } else {
                ModernMessageBox::warning(this, tr("OpenSSL Error"),
                                          tr("OpenSSL exited with error code: %1").arg(proc.exitCode()));
            }
        }
    }
}
