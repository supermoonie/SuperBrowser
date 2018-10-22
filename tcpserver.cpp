#include "tcpserver.h"

static const QString BOUNDARY = "boundary-----------";

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{

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

void TcpServer::write(QByteArray &data) {
    data.append("\r\n").append(BOUNDARY).append("\r\n");
    socket->write(data);
    socket->flush();
}
