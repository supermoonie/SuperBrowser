#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QList>

class WebSocketServer : public QWebSocketServer
{
    Q_OBJECT
public:
    explicit WebSocketServer(QObject *parent = 0);
    ~WebSocketServer();

private:
    QList<QWebSocket*> clients;

private slots:
    void onNewConnection();
    void onTextMessageReceived(QString message);
    void onDisconnect();

signals:
    void commandReceived(QWebSocket* client, const QString &command);

public slots:

};

#endif // WEBSOCKETSERVER_H
