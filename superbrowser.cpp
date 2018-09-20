#include "superbrowser.h"

SuperBrowser::SuperBrowser(QObject* parent): QObject(parent)
{
    cookieJar = MemoryCookieJar::instance();
    webPage = WebPage::instance();
    receiveThread = new ReceiveThread(NULL);
    connect(receiveThread, &ReceiveThread::received, this, &SuperBrowser::onCommandReceived);
    receiveThread->start();
}

SuperBrowser::~SuperBrowser()
{
    receiveThread->stop();
    receiveThread->deleteLater();
    cookieJar->deleteLater();
    webPage->deleteLater();
}

void SuperBrowser::onCommandReceived(const QString &rawCommand) {
    Terminal::instance()->cout(rawCommand, true);
}
