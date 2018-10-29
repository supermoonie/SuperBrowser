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

signals:
    void cookieChanged();

public:
    QList<QNetworkCookie> cookies(const QString& url = QString()) const;
    virtual bool insertCookie(const QNetworkCookie &cookie) Q_DECL_OVERRIDE;
    virtual bool deleteCookie(const QNetworkCookie &cookie) Q_DECL_OVERRIDE;
    virtual bool updateCookie(const QNetworkCookie &cookie) Q_DECL_OVERRIDE;
};

#endif // MEMORYCOOKIEJAR_H
