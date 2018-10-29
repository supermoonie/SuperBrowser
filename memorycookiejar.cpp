#include "memorycookiejar.h"
#include <QDebug>

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

bool MemoryCookieJar::insertCookie(const QNetworkCookie &cookie) {
    bool flag = QNetworkCookieJar::insertCookie(cookie);
    if(flag) {
        emit cookieChanged();
    }
    return flag;
}

bool MemoryCookieJar::deleteCookie(const QNetworkCookie &cookie) {
    bool flag = QNetworkCookieJar::deleteCookie(cookie);
    if(flag) {
        emit cookieChanged();
    }
    return flag;
}

bool MemoryCookieJar::updateCookie(const QNetworkCookie &cookie) {
    bool flag = QNetworkCookieJar::updateCookie(cookie);
    if(flag) {
        emit cookieChanged();
    }
    return flag;
}

bool MemoryCookieJar::setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url) {
    return QNetworkCookieJar::setCookiesFromUrl(cookieList, url);
}

QList<QNetworkCookie> MemoryCookieJar::cookiesForUrl(const QUrl &url) const {

}

bool MemoryCookieJar::validateCookie(const QNetworkCookie &cookie, const QUrl &url) const {

}
