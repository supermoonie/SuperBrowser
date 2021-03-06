#ifndef SUPERBROWSER_H
#define SUPERBROWSER_H

#include <QObject>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMap>
#include <QApplication>
#include <QDateTime>
#include <QNetworkProxy>
#include <QNetworkProxyFactory>
#include "webpage.h"
#include "memorycookiejar.h"
#include "networkaccessmanager.h"

class SuperBrowser: public QObject
{
    typedef void (SuperBrowser::*FUN)(QJsonObject&,QJsonObject*);

public:
    SuperBrowser(QObject* parent = NULL);
    ~SuperBrowser();

private:
    MemoryCookieJar* cookieJar;
    NetworkAccessManager* networkAccessManager;
    WebPage* webPage;
    QMap<QString, FUN> commandMap;

private:
    void navigate(QJsonObject &in, QJsonObject* out);
    void close();
    void setProxy(QJsonObject &in, QJsonObject* out);
    void setUserAgent(QJsonObject &in, QJsonObject* out);
    void setInterceptor(QJsonObject &in, QJsonObject* out);
    // cookie operation
    void getAllCookies(QJsonObject &in, QJsonObject* out);
    void getCookies(QJsonObject &in, QJsonObject* out);
    bool insertCookie(QJsonObject &cookieJson);
    void setCookie(QJsonObject &in, QJsonObject* out);
    void setCookies(QJsonObject &in, QJsonObject* out);
    bool deleteCookie(QJsonObject &cookieJson);
    void deleteCookie(QJsonObject &in, QJsonObject* out);
    void deleteCookies(QJsonObject &in, QJsonObject* out);
    // cookie operation

    // extractor operation
    bool addExtractor(const QString &extractor);
    void addExtractor(QJsonObject &in, QJsonObject* out);
    void addExtractors(QJsonObject &in, QJsonObject* out);
    QByteArray getResponse(const QString &extractor);
    void getResponse(QJsonObject &in, QJsonObject* out);
    void getResponses(QJsonObject &in, QJsonObject* out);
    // extractor operation

private slots:

};

#endif // SUPERBROWSER_H
