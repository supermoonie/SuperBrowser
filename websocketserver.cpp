#include "websocketserver.h"

static WebSocketServer* INSTANCE = NULL;

WebSocketServer::WebSocketServer(QObject *parent) :
    QWebSocketServer("BrowserServer", QWebSocketServer::NonSecureMode, parent)
{
    INSTANCE = this;
    connect(this, &WebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
}

WebSocketServer::~WebSocketServer()
{
    this->close();
    qDeleteAll(clients.begin(), clients.end());
}

WebSocketServer* WebSocketServer::instance() {
    return INSTANCE;
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

void WebSocketServer::exit() {
    for(QWebSocket* client: clients) {
        client->disconnect();
        client->close();
    }
}
