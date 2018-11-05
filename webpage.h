#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <QWebPage>
#include <QWebFrame>
#include <QImage>
#include <QPainter>
#include <QWebSocket>
#include <QList>
#include <QJsonArray>
#include <QMessageBox>
#include "networkaccessmanager.h"
#include "memorycookiejar.h"

class WebPage: public QWebPage
{
    Q_OBJECT

    typedef void (WebPage::*FUN)(QJsonObject&,QJsonObject&);

public:
    explicit WebPage(QObject* parent = NULL);
    ~WebPage();
    static WebPage* instance();

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
    void javaScriptAlert(QWebFrame* frame, const QString &msg);
    bool javaScriptConfirm(QWebFrame *originatingFrame, const QString &msg);

private:
    MemoryCookieJar* cookieJar;
    NetworkAccessManager* networkAccessManager;
    QString userAgent;
    QMessageBox* alertBox;
    QMessageBox* confirmBox;
    QMap<QString, FUN> commandMap;

private:
    // Window
    void getVersion(QJsonObject &in, QJsonObject &out);
    void setProxy(QJsonObject &in, QJsonObject &out);
    void setInterceptors(QJsonObject &in, QJsonObject &out);
    void getWindowBounds(QJsonObject &in, QJsonObject &out);
    void setWindowBounds(QJsonObject &in, QJsonObject &out);
    void getWindowState(QJsonObject &in, QJsonObject &out);
    void setWindowState(QJsonObject &in, QJsonObject &out);
    void close(QJsonObject &in, QJsonObject &out);
    // Page
    void navigate(QJsonObject &in, QJsonObject &out);
    void currentUrl(QJsonObject &in, QJsonObject &out);
    void getUserAgent(QJsonObject &in, QJsonObject &out);
    void setUserAgent(QJsonObject &in, QJsonObject &out);
    void hasAlert(QJsonObject &in, QJsonObject &out);
    void alertText(QJsonObject &in, QJsonObject &out);
    void closeAlert(QJsonObject &in, QJsonObject &out);
    void hasConfirm(QJsonObject &in, QJsonObject &out);
    void confirmText(QJsonObject &in, QJsonObject &out);
    void acceptConfirm(QJsonObject &in, QJsonObject &out);

signals:
    void commandProcessed(QByteArray &data);
    void cookieChanged(const QList<QNetworkCookie> &cookieList);

public slots:
    void onCommandReceived(QWebSocket* client, const QString &command);
    void onCookieChanged();
    void onCookieEdited(const QNetworkCookie &cookie);

};

#endif // WEBPAGE_H
