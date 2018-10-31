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

bool MemoryCookieJar::deleteCookie(const QNetworkCookie &cookie) {
    bool deleted = false;
    QList<QNetworkCookie> cookis = allCookies();
    for(int i = 0; i < cookis.size(); i ++) {
        QNetworkCookie networkCookie = cookis.at(i);
        if(networkCookie.name() == cookie.name() &&
                networkCookie.domain() == cookie.domain() &&
                networkCookie.path() == cookie.path()) {
            cookis.removeAt(i);
            deleted = true;
        }
    }
    if(deleted) {
        setAllCookies(cookis);
        emit cookieChanged();
    }
}

bool MemoryCookieJar::addCookie(const QNetworkCookie &cookie, const QUrl &url) {
    QUrl convertedUrl = !url.isEmpty() ?
                url :
                QUrl(QString((cookie.isSecure() ? "https://" : "http://")
                             + QString((cookie.domain().startsWith('.') ? "www" : ""))
                             + cookie.domain()
                             + (cookie.path().isEmpty() ? "/" : cookie.path())));
    return setCookiesFromUrl(QList<QNetworkCookie>() << cookie, convertedUrl);
}

bool MemoryCookieJar::setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url) {
    bool flag = QNetworkCookieJar::setCookiesFromUrl(cookieList, url);
    if(flag) {
        emit cookieChanged();
    }
    return flag;
}

void MemoryCookieJar::clearCookies() {
    setAllCookies(QList<QNetworkCookie>());
    emit cookieChanged();
}
