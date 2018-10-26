#include "websocketserver.h"

WebSocketServer::WebSocketServer(QObject *parent) :
    QWebSocketServer("BrowserServer", QWebSocketServer::NonSecureMode, parent)
{
    connect(this, &WebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
}

WebSocketServer::~WebSocketServer()
{
    this->close();
    qDeleteAll(clients.begin(), clients.end());
}

void WebSocketServer::onNewConnection() {
    QWebSocket* webSocket = this->nextPendingConnection();
    connect(webSocket, &QWebSocket::textMessageReceived, this, &WebSocketServer::onTextMessageReceived);
    connect(webSocket, &QWebSocket::disconnected, this, &WebSocketServer::onDisconnect);
}

void WebSocketServer::onTextMessageReceived(QString message) {
    QWebSocket* client = qobject_cast<QWebSocket*>(sender());
    if(client) {
        emit commandReceived(client, message);
    }
}

void WebSocketServer::onDisconnect() {
    QWebSocket* client = qobject_cast<QWebSocket*>(sender());
    if(client) {
        clients.removeAll(client);
        client->deleteLater();
    }
}
