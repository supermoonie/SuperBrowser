#ifndef SUPERBROWSER_H
#define SUPERBROWSER_H

#include <QObject>
#include "webpage.h"
#include "memorycookiejar.h"
#include "receivethread.h"

class SuperBrowser: public QObject
{
public:
    SuperBrowser(QObject* parent = NULL);
    ~SuperBrowser();

private:
    ReceiveThread* receiveThread;
    MemoryCookieJar* cookieJar;
    WebPage* webPage;

private slots:
    void onCommandReceived(const QString &rawCommand);
};

#endif // SUPERBROWSER_H
