#ifndef MEMORYCOOKIEJAR_H
#define MEMORYCOOKIEJAR_H

#include <QNetworkCookieJar>
#include <QNetworkCookie>

class MemoryCookieJar: public QNetworkCookieJar
{
    Q_OBJECT
public:
    MemoryCookieJar(QObject* parent = NULL);
    ~MemoryCookieJar();

signals:
    void cookieChanged();

public:
    QList<QNetworkCookie> cookies(const QUrl &url) const;
    bool addCookie(const QNetworkCookie &cookie, const QUrl &url);
    virtual bool setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url) Q_DECL_OVERRIDE;

public slots:
    void clearCookies();
    bool deleteCookie(const QNetworkCookie &cookie) Q_DECL_OVERRIDE;

};

#endif // MEMORYCOOKIEJAR_H
