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
    bool isSecure = url.scheme().startsWith("https") ? true : false;
    bool flag = false;
    qDebug() << "---------------------------------";
    for(QNetworkCookie cookie: cookieList) {
        cookie.setSecure(isSecure);
        if(cookie.domain().isEmpty()) {
            cookie.setDomain(url.host());
        }
        if(cookie.path().isEmpty()) {
            cookie.setPath("/");
        }
        qDebug() << cookie.toRawForm();
        QUrl convertedUrl = !url.isEmpty() ?
                    url :
                    QUrl(QString((cookie.isSecure() ? "https://" : "http://")
                                 + (cookie.domain().startsWith('.') ? "www" : "")
                                 + cookie.domain()
                                 + (cookie.path().isEmpty() ? "/" : cookie.path())));
        qDebug() << convertedUrl.toString();
        if(!flag) {
            flag = QNetworkCookieJar::setCookiesFromUrl(QList<QNetworkCookie>() << cookie, convertedUrl);
        } else {
            QNetworkCookieJar::setCookiesFromUrl(QList<QNetworkCookie>() << cookie, convertedUrl);
        }
        qDebug() << flag;
    }
    if(flag) {
        emit cookieChanged();
    }
    return flag;
}
