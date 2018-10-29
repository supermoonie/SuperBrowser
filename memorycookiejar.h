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
    virtual bool setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url) Q_DECL_OVERRIDE;
    virtual QList<QNetworkCookie> cookiesForUrl(const QUrl &url) const Q_DECL_OVERRIDE;

protected:
    virtual bool validateCookie(const QNetworkCookie &cookie, const QUrl &url) const Q_DECL_OVERRIDE;

private:
    QMap<QString, QNetworkCookie> cookies;
};

#endif // MEMORYCOOKIEJAR_H
