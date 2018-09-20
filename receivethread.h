#ifndef RECEIVETHREAD_H
#define RECEIVETHREAD_H

#include <QThread>
#include <iostream>

using namespace std;

class ReceiveThread : public QThread
{
    Q_OBJECT
public:
    explicit ReceiveThread(QObject *parent = 0);
    ~ReceiveThread();
    void stop();

private:
    bool stoped = false;

protected:
    void run();

signals:
    void received(const QString &data);

public slots:
};

#endif // RECEIVETHREAD_H
