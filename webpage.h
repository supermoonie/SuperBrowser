#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <QWebPage>
#include <QWebFrame>
#include <QImage>
#include <QPainter>
#include <QWebSocket>

class WebPage: public QWebPage
{
    Q_OBJECT

    typedef void (WebPage::*FUN)(QJsonObject&,QJsonObject&);

public:
    explicit WebPage(QObject* parent = NULL);
    ~WebPage();

public:
    void setUserAgent(const QString &ua);

protected:
    QImage renderImage();
    QString userAgentForUrl(const QUrl &url) const;

private:
    QString userAgent;
    QMap<QString, FUN> commandMap;

private:
    void navigate(QJsonObject &in, QJsonObject &out);
    void setProxy(QJsonObject &in, QJsonObject &out);
    void setUserAgent(QJsonObject &in, QJsonObject &out);

signals:
    void commandProcessed(QByteArray &data);

public slots:
    void onCommandReceived(QWebSocket* client, const QString &command);

};

#endif // WEBPAGE_H
