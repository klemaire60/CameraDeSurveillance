#pragma once

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include "ControleCamera.h"

class WebSocketServer : public QObject
{
    Q_OBJECT

public:
    explicit WebSocketServer(ControleCamera* controleCamera, QObject* parent = nullptr);
    ~WebSocketServer();

    bool startServer(quint16 port);

private slots:
    void onNewConnection();
    void onTextMessageReceived(const QString& message);
    void onClientDisconnected();

private:
    QWebSocketServer* server;
    QList<QWebSocket*> clients;
    ControleCamera* cameraControl;
};
