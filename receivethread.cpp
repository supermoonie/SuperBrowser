#include "receivethread.h"

static const string BOUNDARY = "boundary---------";

ReceiveThread::ReceiveThread(QObject *parent) : QThread(parent)
{

}

ReceiveThread::~ReceiveThread()
{

}

void ReceiveThread::run() {
    string data;
    string line;
    while(!stoped && cin >> line) {
        data.append(line);
        int index = data.find(BOUNDARY);
        if(index > 0) {
            emit received(QString::fromStdString(data.replace(index, BOUNDARY.length(), "")));
            data.clear();
        }
        line.clear();
    }
}

void ReceiveThread::stop() {
    this->stoped = true;
}
