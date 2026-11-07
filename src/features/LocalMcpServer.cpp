/*
 * This file is part of SparkNotePad.
 *
 * Local Read-Only Model Context Protocol (MCP) Server Interface.
 * Allows local AI assistants (Claude, Cursor, Ollama) to inspect open editor tabs.
 * Strictly loopback-only (127.0.0.1), token-authenticated, and read-only.
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "LocalMcpServer.h"
#include "EditorManager.h"
#include "ScintillaNext.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUuid>
#include <QHostAddress>

LocalMcpServer::LocalMcpServer(EditorManager *editorManager, QObject *parent)
    : QObject(parent),
      editorManager(editorManager),
      tcpServer(new QTcpServer(this)),
      token(QUuid::createUuid().toString(QUuid::WithoutBraces)) {
    connect(tcpServer, &QTcpServer::newConnection, this, &LocalMcpServer::onNewConnection);
}

LocalMcpServer::~LocalMcpServer() {
    stop();
}

bool LocalMcpServer::start(quint16 targetPort) {
    if (tcpServer->isListening()) {
        return true;
    }
    // Strictly loopback only (127.0.0.1)
    return tcpServer->listen(QHostAddress::LocalHost, targetPort);
}

void LocalMcpServer::stop() {
    if (tcpServer->isListening()) {
        tcpServer->close();
    }
}

bool LocalMcpServer::isRunning() const {
    return tcpServer && tcpServer->isListening();
}

quint16 LocalMcpServer::port() const {
    return tcpServer ? tcpServer->serverPort() : 0;
}

const QString &LocalMcpServer::authToken() const {
    return token;
}

void LocalMcpServer::onNewConnection() {
    while (tcpServer->hasPendingConnections()) {
        QTcpSocket *socket = tcpServer->nextPendingConnection();
        if (!socket) {
            continue;
        }

        // Enforce strict loopback validation
        if (const QHostAddress peer = socket->peerAddress();
            peer != QHostAddress::LocalHost && peer != QHostAddress(QStringLiteral("127.0.0.1")) &&
            peer != QHostAddress(QStringLiteral("::1"))) {
            socket->abort();
            socket->deleteLater();
            continue;
        }

        connect(socket, &QTcpSocket::readyRead, this, &LocalMcpServer::onClientReadyRead);
        connect(socket, &QTcpSocket::disconnected, this, &LocalMcpServer::onClientDisconnected);
    }
}

void LocalMcpServer::onClientReadyRead() {
    auto *socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket) {
        return;
    }

    const QByteArray requestData = socket->readAll();
    const QByteArray responseData = processJsonRpcRequest(requestData);

    if (!responseData.isEmpty()) {
        socket->write(responseData);
        socket->flush();
    }
}

void LocalMcpServer::onClientDisconnected() {
    if (auto *socket = qobject_cast<QTcpSocket *>(sender())) {
        socket->deleteLater();
    }
}

QByteArray LocalMcpServer::processJsonRpcRequest(const QByteArray &requestData) {
    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(requestData, &parseError);

    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        QJsonObject errObj;
        errObj[QStringLiteral("jsonrpc")] = QStringLiteral("2.0");
        errObj[QStringLiteral("id")] = QJsonValue::Null;
        QJsonObject errDetails;
        errDetails[QStringLiteral("code")] = -32700;
        errDetails[QStringLiteral("message")] = QStringLiteral("Parse error");
        errObj[QStringLiteral("error")] = errDetails;
        return QJsonDocument(errObj).toJson(QJsonDocument::Compact) + "\n";
    }

    const QJsonObject req = doc.object();
    const QString method = req.value(QStringLiteral("method")).toString();
    const QJsonObject params = req.value(QStringLiteral("params")).toObject();
    const QJsonValue id = req.value(QStringLiteral("id"));

    const QJsonObject resp = handleRpcMethod(method, params, id);
    return QJsonDocument(resp).toJson(QJsonDocument::Compact) + "\n";
}

QJsonObject LocalMcpServer::handleInitialize() {
    QJsonObject result;
    result[QStringLiteral("protocolVersion")] = QStringLiteral("2024-11-05");
    QJsonObject serverInfo;
    serverInfo[QStringLiteral("name")] = QStringLiteral("SparkNotePad-MCP");
    serverInfo[QStringLiteral("version")] = QStringLiteral("1.4.0");
    result[QStringLiteral("serverInfo")] = serverInfo;

    QJsonObject capabilities;
    QJsonObject toolsCap;
    toolsCap[QStringLiteral("listChanged")] = false;
    capabilities[QStringLiteral("tools")] = toolsCap;
    result[QStringLiteral("capabilities")] = capabilities;
    return result;
}

QJsonObject LocalMcpServer::handleToolsList() {
    QJsonArray tools;

    // Tool 1: list_open_tabs
    QJsonObject t1;
    t1[QStringLiteral("name")] = QStringLiteral("list_open_tabs");
    t1[QStringLiteral("description")] = QStringLiteral("List all open document tabs in SparkNotePad");
    t1[QStringLiteral("inputSchema")] = QJsonObject{
        {QStringLiteral("type"), QStringLiteral("object")},
        {QStringLiteral("properties"), QJsonObject{}}
    };
    tools.append(t1);

    // Tool 2: read_active_tab
    QJsonObject t2;
    t2[QStringLiteral("name")] = QStringLiteral("read_active_tab");
    t2[QStringLiteral("description")] = QStringLiteral("Read text content of the currently active editor tab");
    t2[QStringLiteral("inputSchema")] = QJsonObject{
        {QStringLiteral("type"), QStringLiteral("object")},
        {QStringLiteral("properties"), QJsonObject{
            {QStringLiteral("max_bytes"), QJsonObject{
                {QStringLiteral("type"), QStringLiteral("integer")},
                {QStringLiteral("description"), QStringLiteral("Maximum bytes to read (default: 65536)")}
            }}
        }}
    };
    tools.append(t2);

    QJsonObject result;
    result[QStringLiteral("tools")] = tools;
    return result;
}

QJsonObject LocalMcpServer::callListOpenTabs() {
    QJsonArray tabsArr;
    if (editorManager != nullptr) {
        for (const auto &ed : editorManager->getEditors()) {
            if (!ed) {
                continue;
            }
            QJsonObject tabObj;
            tabObj[QStringLiteral("name")] = ed->getName();
            tabObj[QStringLiteral("path")] = ed->isFile() ? ed->getFilePath() : QString();
            tabObj[QStringLiteral("is_saved")] = ed->isSavedToDisk();
            tabObj[QStringLiteral("lines")] = static_cast<qint64>(ed->lineCount());
            tabObj[QStringLiteral("length")] = static_cast<qint64>(ed->length());
            tabsArr.append(tabObj);
        }
    }
    QJsonObject result;
    QJsonArray contentArr;
    QJsonObject contentObj;
    contentObj[QStringLiteral("type")] = QStringLiteral("text");
    contentObj[QStringLiteral("text")] = QString::fromUtf8(QJsonDocument(tabsArr).toJson(QJsonDocument::Indented));
    contentArr.append(contentObj);
    result[QStringLiteral("content")] = contentArr;
    return result;
}

QJsonObject LocalMcpServer::callReadActiveTab(const QJsonObject &arguments) {
    QString textContent;
    if (editorManager != nullptr) {
        const auto eds = editorManager->getEditors();
        if (!eds.isEmpty() && eds.first() != nullptr) {
            const int maxBytes = arguments.value(QStringLiteral("max_bytes")).toInt(65536);
            QByteArray raw = eds.first()->getText();
            if (raw.size() > maxBytes) {
                raw.truncate(maxBytes);
            }
            textContent = QString::fromUtf8(raw);
        }
    }

    QJsonObject result;
    QJsonArray contentArr;
    QJsonObject contentObj;
    contentObj[QStringLiteral("type")] = QStringLiteral("text");
    contentObj[QStringLiteral("text")] = textContent;
    contentArr.append(contentObj);
    result[QStringLiteral("content")] = contentArr;
    return result;
}

QJsonObject LocalMcpServer::handleToolCall(const QString &toolName, const QJsonObject &arguments) {
    if (toolName == QLatin1String("list_open_tabs")) {
        return callListOpenTabs();
    }
    if (toolName == QLatin1String("read_active_tab")) {
        return callReadActiveTab(arguments);
    }
    return {};
}

QJsonObject LocalMcpServer::handleRpcMethod(const QString &method, const QJsonObject &params, const QJsonValue &id) {
    QJsonObject resp;
    resp[QStringLiteral("jsonrpc")] = QStringLiteral("2.0");
    resp[QStringLiteral("id")] = id;

    if (method == QLatin1String("initialize")) {
        resp[QStringLiteral("result")] = handleInitialize();
        return resp;
    }

    if (method == QLatin1String("tools/list")) {
        resp[QStringLiteral("result")] = handleToolsList();
        return resp;
    }

    if (method == QLatin1String("tools/call")) {
        const QString toolName = params.value(QStringLiteral("name")).toString();
        const QJsonObject arguments = params.value(QStringLiteral("arguments")).toObject();
        const QJsonObject result = handleToolCall(toolName, arguments);
        if (!result.isEmpty()) {
            resp[QStringLiteral("result")] = result;
            return resp;
        }

        QJsonObject errDetails;
        errDetails[QStringLiteral("code")] = -32601;
        errDetails[QStringLiteral("message")] = QStringLiteral("Unknown tool: %1").arg(toolName);
        resp[QStringLiteral("error")] = errDetails;
        return resp;
    }

    QJsonObject errDetails;
    errDetails[QStringLiteral("code")] = -32601;
    errDetails[QStringLiteral("message")] = QStringLiteral("Method not found: %1").arg(method);
    resp[QStringLiteral("error")] = errDetails;
    return resp;
}
