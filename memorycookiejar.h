#ifndef MEMORYCOOKIEJAR_H
#define MEMORYCOOKIEJAR_H

#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QObject>

class MemoryCookieJar: public QNetworkCookieJar
{

public:
    static MemoryCookieJar* instance();

    MemoryCookieJar(QObject* parent = NULL);
    ~MemoryCookieJar();

    QList<QNetworkCookie> cookies(const QString& url = QString()) const;
};

#endif // MEMORYCOOKIEJAR_H
