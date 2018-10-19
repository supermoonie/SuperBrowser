#include "tcpserver.h"

static const QString BOUNDARY = "boundary-----------";

TcpServer::TcpServer(int port, QObject *parent) : QTcpServer(parent)
{
    if(!this->listen(QHostAddress::Any, port))
    {
        emit listenError();
        return;
    } else {
        connect(this, &TcpServer::newConnection, this, &TcpServer::onNewConnection);
    }
}

TcpServer::~TcpServer()
{

}

void TcpServer::onNewConnection() {
    socket = this->nextPendingConnection();
    message.clear();
    connect(socket, &QTcpSocket::readyRead, this, &TcpServer::onSocketReadyRead);
}

void TcpServer::onSocketReadyRead() {
    QByteArray data = socket->readAll();
    message.append(data);
    int index = message.indexOf(BOUNDARY);
    if(index > 0) {
        QString command = message.left(index);
        emit commandReceived(command);
    }
}
