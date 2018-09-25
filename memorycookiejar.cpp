#include "memorycookiejar.h"

static MemoryCookieJar* memoryCookieJar = NULL;

MemoryCookieJar::MemoryCookieJar(QObject* parent): QNetworkCookieJar(parent)
{

}

MemoryCookieJar::~MemoryCookieJar()
{

}

MemoryCookieJar* MemoryCookieJar::instance(QObject* parent) {
    if(memoryCookieJar == NULL) {
        memoryCookieJar = new MemoryCookieJar(parent);
    }
    return memoryCookieJar;
}

QList<QNetworkCookie> MemoryCookieJar::cookies(const QString &url) const {
    if(url.isEmpty()) {
        return allCookies();
    } else {
        return QNetworkCookieJar::cookiesForUrl(url);
    }
}
