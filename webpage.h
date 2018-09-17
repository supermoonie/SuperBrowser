#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <QWebPage>

class WebPage: public QWebPage
{
public:
    WebPage(QObject* parent = NULL);
    ~WebPage();
};

#endif // WEBPAGE_H
