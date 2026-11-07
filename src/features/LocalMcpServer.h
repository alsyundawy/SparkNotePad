#pragma once
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

#ifndef LOCALMCPSERVER_H
#define LOCALMCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>

class EditorManager;

class LocalMcpServer : public QObject {
    Q_OBJECT

public:
    explicit LocalMcpServer(EditorManager *editorManager, QObject *parent = nullptr);
    ~LocalMcpServer() override;

    bool start(quint16 targetPort = 0);
    void stop();
    bool isRunning() const;
    quint16 port() const;
    const QString &authToken() const;

private slots:
    void onNewConnection();
    void onClientReadyRead();
    void onClientDisconnected();

private:
    QByteArray processJsonRpcRequest(const QByteArray &requestData);
    QJsonObject handleRpcMethod(const QString &method, const QJsonObject &params, const QJsonValue &id);
    static QJsonObject handleInitialize();
    static QJsonObject handleToolsList();
    QJsonObject handleToolCall(const QString &toolName, const QJsonObject &arguments);
    QJsonObject callListOpenTabs();
    QJsonObject callReadActiveTab(const QJsonObject &arguments);

    EditorManager *editorManager;
    QTcpServer *tcpServer;
    QString token;
};

#endif  // LOCALMCPSERVER_H
