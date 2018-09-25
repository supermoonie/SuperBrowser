#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <QWebPage>
#include <QWebFrame>
#include <QImage>
#include <QPainter>
#include "terminal.h"

class WebPage: public QWebPage
{
public:
    WebPage(QObject* parent = NULL);
    ~WebPage();

public:
    static WebPage* instance(QObject* parent = NULL);
    void setUserAgent(const QString &ua);

protected:
    QImage renderImage();
    QString userAgentForUrl(const QUrl &url) const;

private:
    QString userAgent;

};

#endif // WEBPAGE_H
