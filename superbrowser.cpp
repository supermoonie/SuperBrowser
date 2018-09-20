#include "superbrowser.h"

//static SuperBrowser* superBrowser = NULL;

SuperBrowser::SuperBrowser(QObject* parent): QObject(parent)
{
    cookieJar = MemoryCookieJar::instance();
    receiveThread = new ReceiveThread(NULL);
    connect(receiveThread, &ReceiveThread::received, this, &SuperBrowser::onCommandReceived);
    receiveThread->start();
}

SuperBrowser::~SuperBrowser()
{
    receiveThread->stop();
    receiveThread->deleteLater();
}

void SuperBrowser::onCommandReceived(const QString &rawCommand) {
    Terminal::instance()->cout(rawCommand, true);
}
