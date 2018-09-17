#ifndef MEMORYCOOKIEJAR_H
#define MEMORYCOOKIEJAR_H

#include <QNetworkCookieJar>
#include <QObject>

class MemoryCookieJar: public QNetworkCookieJar
{

public:
    MemoryCookieJar(QObject* parent = NULL);
    ~MemoryCookieJar();

    QList<QNetworkCookie> cookies(const QString& url = QString()) const;
};

#endif // MEMORYCOOKIEJAR_H
