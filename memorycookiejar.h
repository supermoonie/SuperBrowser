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
    virtual bool setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url) Q_DECL_OVERRIDE;

};

#endif // MEMORYCOOKIEJAR_H
