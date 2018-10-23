#ifndef MEMORYCOOKIEJAR_H
#define MEMORYCOOKIEJAR_H

#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QObject>

class MemoryCookieJar: public QNetworkCookieJar
{
    Q_OBJECT
public:
    MemoryCookieJar(QObject* parent = NULL);
    ~MemoryCookieJar();

public:
    QList<QNetworkCookie> cookies(const QString& url = QString()) const;
};

#endif // MEMORYCOOKIEJAR_H
