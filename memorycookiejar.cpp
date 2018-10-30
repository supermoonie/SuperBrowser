#include "memorycookiejar.h"
#include <QDebug>

MemoryCookieJar::MemoryCookieJar(QObject* parent):
    QNetworkCookieJar(parent)
{

}

MemoryCookieJar::~MemoryCookieJar()
{

}

QList<QNetworkCookie> MemoryCookieJar::cookies(const QUrl &url) const {
    if(url.isEmpty() || !url.isValid()) {
        return allCookies();
    } else {
        return cookiesForUrl(url);
    }
}

bool MemoryCookieJar::setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url) {
    bool flag = QNetworkCookieJar::setCookiesFromUrl(cookieList, url);
    if(flag) {
        emit cookieChanged();
    }
    return flag;
}
