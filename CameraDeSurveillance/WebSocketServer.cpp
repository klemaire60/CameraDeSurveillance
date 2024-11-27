#include "WebSocketServer.h"
#include <QDebug>

WebSocketServer::WebSocketServer(ControleCamera* controleCamera, QObject* parent)
    : QObject(parent), cameraControl(controleCamera)
{
    server = new QWebSocketServer("Camera Control Server", QWebSocketServer::NonSecureMode, this);
}

WebSocketServer::~WebSocketServer()
{
    if (server->isListening()) {
        server->close();
    }
    qDeleteAll(clients);
}

bool WebSocketServer::startServer(quint16 port)
{
    if (!server->listen(QHostAddress::Any, port)) {
        qDebug() << "WebSocket Server failed to start: " << server->errorString();
        return false;
    }
    connect(server, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
    qDebug() << "WebSocket Server started on port" << port;
    return true;
}

void WebSocketServer::onNewConnection()
{
    auto client = server->nextPendingConnection();
    clients.append(client);
    connect(client, &QWebSocket::textMessageReceived, this, &WebSocketServer::onTextMessageReceived);
    connect(client, &QWebSocket::disconnected, this, &WebSocketServer::onClientDisconnected);
    qDebug() << "New client connected.";
}

void WebSocketServer::onTextMessageReceived(const QString& message)
{
    qDebug() << "Message received:" << message;

    if (message == "MOVE_UP") cameraControl->MoveUp();
    else if (message == "MOVE_DOWN") cameraControl->MoveDown();
    else if (message == "MOVE_LEFT") cameraControl->MoveLeft();
    else if (message == "MOVE_RIGHT") cameraControl->MoveRight();
    else if (message == "ZOOM_IN") cameraControl->adjustZoom(500); // Exemple
    else if (message == "ZOOM_OUT") cameraControl->adjustZoom(0); // Exemple
}

void WebSocketServer::onClientDisconnected()
{
    auto client = qobject_cast<QWebSocket*>(sender());
    if (client) {
        clients.removeAll(client);
        client->deleteLater();
    }
    qDebug() << "Client disconnected.";
}
