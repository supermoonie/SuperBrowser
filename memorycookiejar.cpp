#include "memorycookiejar.h"

MemoryCookieJar::MemoryCookieJar(QObject* parent): QNetworkCookieJar(parent)
{

}

MemoryCookieJar::~MemoryCookieJar()
{

}

QList<QNetworkCookie> MemoryCookieJar::cookies(const QString &url) const {
    if(url.isEmpty()) {
        return allCookies();
    } else {
        return QNetworkCookieJar::cookiesForUrl(url);
    }
}
