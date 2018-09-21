#ifndef SUPERBROWSER_H
#define SUPERBROWSER_H

#include <QObject>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMap>
#include "webpage.h"
#include "memorycookiejar.h"
#include "receivethread.h"

class SuperBrowser: public QObject
{
    typedef QJsonObject(SuperBrowser::*FUN)(QJsonArray&);

public:
    SuperBrowser(QObject* parent = NULL);
    ~SuperBrowser();

private:
    ReceiveThread* receiveThread;
    MemoryCookieJar* cookieJar;
    WebPage* webPage;
    QMap<QString, FUN>* commandMap;

    QJsonObject navigate(QJsonArray &parameters);

private slots:
    void onCommandReceived(const QString &rawCommand);
};

#endif // SUPERBROWSER_H
