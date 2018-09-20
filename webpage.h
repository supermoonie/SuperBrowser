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
    static WebPage* instance();
    QImage renderImage();

private:

};

#endif // WEBPAGE_H
