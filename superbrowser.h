#ifndef SUPERBROWSER_H
#define SUPERBROWSER_H

#include <QObject>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMap>
#include <QApplication>
#include "webpage.h"
#include "memorycookiejar.h"
#include "receivethread.h"
#include "networkaccessmanager.h"

class SuperBrowser: public QObject
{
    typedef QJsonObject(SuperBrowser::*FUN)(QJsonArray&);

public:
    SuperBrowser(QObject* parent = NULL);
    ~SuperBrowser();

private:
    ReceiveThread* receiveThread;
    MemoryCookieJar* cookieJar;
    NetworkAccessManager* networkAccessManager;
    WebPage* webPage;
    QMap<QString, FUN>* commandMap;

private:
    QJsonObject navigate(QJsonArray &parameters);
    void close();
    QJsonObject getAllCookies(QJsonArray &parameters);
    QJsonObject getCookies(QJsonArray &parameters);

private slots:
    void onCommandReceived(const QString &rawCommand);
};

#endif // SUPERBROWSER_H
