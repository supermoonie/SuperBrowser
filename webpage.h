#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <QWebPage>
#include <QWebFrame>
#include <QImage>
#include <QPainter>
#include <QWebSocket>
#include <QList>
#include <QJsonArray>
#include "networkaccessmanager.h"
#include "memorycookiejar.h"

class WebPage: public QWebPage
{
    Q_OBJECT

    typedef void (WebPage::*FUN)(QJsonObject&,QJsonObject&);

public:
    explicit WebPage(QObject* parent = NULL);
    ~WebPage();

public:
    NetworkAccessManager* getNetworkAccessManager();
    MemoryCookieJar* getCookieJar();
    void setUserAgent(const QString &ua);
    QString getUserAgent();
    void setInterceptors(const QList<QString> &interceptors);
    QList<QString> getInterceptors();

protected:
    QImage renderImage();
    QString userAgentForUrl(const QUrl &url) const;
    virtual QWebPage* createWindow(WebWindowType type) Q_DECL_OVERRIDE;

private:
    MemoryCookieJar* cookieJar;
    NetworkAccessManager* networkAccessManager;
    QString userAgent;
    QMap<QString, FUN> commandMap;

private:
    void navigate(QJsonObject &in, QJsonObject &out);
    void setProxy(QJsonObject &in, QJsonObject &out);
    void setUserAgent(QJsonObject &in, QJsonObject &out);
    void setInterceptors(QJsonObject &in, QJsonObject &out);

signals:
    void commandProcessed(QByteArray &data);
    void cookieChanged(const QList<QNetworkCookie> &cookieList);

public slots:
    void onCommandReceived(QWebSocket* client, const QString &command);
    void onCookieChanged();
    void onCookieEdited(const QNetworkCookie &cookie);

};

#endif // WEBPAGE_H
