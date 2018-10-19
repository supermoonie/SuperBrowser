#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(int port, QObject *parent = 0);
    ~TcpServer();

private:
    QTcpSocket* socket;
    QString message;

signals:
    void listenError();
    void commandReceived(const QString &command);

private slots:
    void onNewConnection();
    void onSocketReadyRead();

public slots:
};

#endif // TCPSERVER_H
