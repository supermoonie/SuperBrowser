#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
    ~TcpServer();
    void write(QByteArray &data);

private:
    QTcpSocket* socket;
    QString message;

signals:
    void listenError();
    void commandReceived(const QString &command);

private slots:
    void onSocketReadyRead();

public slots:
    void onNewConnection();

};

#endif // TCPSERVER_H
