#include "superbrowser.h"

SuperBrowser::SuperBrowser(QObject* parent): QObject(parent)
{
    cookieJar = new MemoryCookieJar(this);
    networkAccessManager = new NetworkAccessManager(cookieJar, this);
    webPage = new WebPage(this);
    webPage->setNetworkAccessManager(networkAccessManager);
    commandMap.insert("navigate", &SuperBrowser::navigate);
    commandMap.insert("getAllCookies", &SuperBrowser::getAllCookies);
    commandMap.insert("setCookie", &SuperBrowser::setCookie);
    commandMap.insert("setCookies", &SuperBrowser::setCookies);
    commandMap.insert("deleteCookie", &SuperBrowser::deleteCookie);
    commandMap.insert("deleteCookies", &SuperBrowser::deleteCookies);
    commandMap.insert("getResponse", &SuperBrowser::getResponse);
    commandMap.insert("getResponses", &SuperBrowser::getResponses);
    QNetworkProxyFactory::setUseSystemConfiguration(true);
}

SuperBrowser::~SuperBrowser()
{

}

void SuperBrowser::close() {
    QApplication::exit(0);
}

void SuperBrowser::navigate(QJsonObject &in, QJsonObject* out) {

}

void SuperBrowser::setProxy(QJsonObject &in, QJsonObject *out) {

}

void SuperBrowser::setUserAgent(QJsonObject &in, QJsonObject *out) {

}

void SuperBrowser::setInterceptor(QJsonObject &in, QJsonObject *out) {

}

bool SuperBrowser::addExtractor(const QString &extractor) {

}

void SuperBrowser::addExtractor(QJsonObject &in, QJsonObject *out) {

}

QByteArray SuperBrowser::getResponse(const QString &extractor) {
    if(extractor.isNull() || extractor.isEmpty()) {
        return NULL;
    }
//    return this->networkAccessManager->extract(QRegExp(extractor));
    return QString("").toUtf8();
}

void SuperBrowser::getResponse(QJsonObject &in, QJsonObject *out) {
    QString extractor = in.value("parameters").toString();
    QByteArray data = getResponse(extractor);
    out->insert("body", QString(data));
}

void SuperBrowser::getResponses(QJsonObject &in, QJsonObject *out) {
    QJsonArray extractorArray = in.value("parameters").toArray();
    for(int i = 0; i < extractorArray.size(); i ++) {
        QString extractor = extractorArray.at(i).toString();
        if(extractor.isNull() || extractor.isEmpty()) {
            continue;
        }
        QByteArray data = getResponse(extractor);
        out->insert(extractor, QString(data));
    }
}

void SuperBrowser::getAllCookies(QJsonObject &in, QJsonObject* out) {
    QJsonArray parameters = in.value("parameters").toArray();
    QList<QNetworkCookie> cookies;
    if(parameters.size() > 0) {
        for(int i = 0; i < parameters.size(); i ++) {
            QString url = parameters.at(i).toString();
            cookies.append(cookieJar->cookies(url));
        }
    } else {
        cookies = cookieJar->cookies(QUrl());
    }
    QJsonArray cookieArray;
    for(int i = 0; i < cookies.size(); i ++) {
        QNetworkCookie cookie = cookies.at(i);
        QJsonObject cookieJson;
        cookieJson.insert("name", QString(cookie.name()));
        cookieJson.insert("value", QString(cookie.value()));
        cookieJson.insert("domain", cookie.domain());
        cookieJson.insert("path", cookie.path());
        cookieJson.insert("expires", QJsonValue(cookie.expirationDate().toMSecsSinceEpoch()));
        cookieJson.insert("httpOnly", QJsonValue(cookie.isHttpOnly()));
        cookieJson.insert("secure", QJsonValue(cookie.isSecure()));
        cookieJson.insert("session", QJsonValue(cookie.isSessionCookie()));
        cookieArray.append(cookieJson);
    }
    out->insert("cookies", cookieArray);
}

void SuperBrowser::getCookies(QJsonObject &in, QJsonObject* out) {
    this->getAllCookies(in, out);
}

bool SuperBrowser::insertCookie(QJsonObject &cookieJson) {
    QString name = cookieJson.value("name").toString();
    if(name.isNull() || name.isEmpty()) {
        return false;
    }
    QString value = cookieJson.value("value").toString();
    if(name.isNull() || name.isEmpty()) {
        return false;
    }
    QString currentHost = this->webPage->currentFrame()->url().host();
    QString domain = cookieJson.value("domain").toString(currentHost);
    QString currentPath = this->webPage->currentFrame()->url().path();
    QString path = cookieJson.value("path").toString(currentPath);
    QDateTime defaultTime = QDateTime::currentDateTime().addDays(7);
    QVariant expiresVariant = cookieJson.value("expires").toVariant();
    qint64 expires = 0;
    if(!expiresVariant.canConvert(QMetaType::LongLong) || expiresVariant.isNull() || expiresVariant.isValid()) {
        expires = defaultTime.toMSecsSinceEpoch();
    } else {
        expires = (qint64) expiresVariant.toLongLong();
    }
    bool httpOnly = cookieJson.value("httpOnly").toBool(false);
    bool secure = cookieJson.value("secure").toBool(false);
    QNetworkCookie cookie;
    cookie.setName(name.toUtf8());
    cookie.setValue(value.toUtf8());
    cookie.setDomain(domain);
    cookie.setPath(path);
    cookie.setExpirationDate(QDateTime::fromMSecsSinceEpoch(expires));
    cookie.setHttpOnly(httpOnly);
    cookie.setSecure(secure);
    return this->cookieJar->insertCookie(cookie);
}

void SuperBrowser::setCookie(QJsonObject &in, QJsonObject *out) {
    QJsonObject cookieJson = in.value("parameters").toObject();
    bool flag = this->insertCookie(cookieJson);
    out->insert("success", flag);
}

void SuperBrowser::setCookies(QJsonObject &in, QJsonObject *out) {
    QJsonArray cookieArray = in.value("parameters").toArray();
    for(int i = 0; i < cookieArray.size(); i ++) {
        QJsonObject cookieJson = cookieArray.at(i).toObject();
        bool flag = this->insertCookie(cookieJson);
        QString name = cookieJson.value("name").toString();
        out->insert(name, flag);
    }
}

bool SuperBrowser::deleteCookie(QJsonObject &cookieJson) {
    QString name = cookieJson.value("name").toString();
    if(name.isNull() || name.isEmpty()) {
        return false;
    }
    QString urlStr = cookieJson.value("url").toString();
    QUrl url;
    if(urlStr.isNull() || urlStr.isEmpty()) {
        url = this->webPage->currentFrame()->url();
    } else {
        url = QUrl::fromUserInput(urlStr);
    }
    QString host = url.host();
    QString path = url.path();
    QNetworkCookie cookie;
    cookie.setName(name.toUtf8());
    cookie.setDomain(host);
    cookie.setPath(path);
    return this->cookieJar->deleteCookie(cookie);
}

void SuperBrowser::deleteCookie(QJsonObject &in, QJsonObject *out) {
    QJsonObject cookieJson = in.value("parameters").toObject();
    bool flag = this->deleteCookie(cookieJson);
    out->insert("success", flag);
}

void SuperBrowser::deleteCookies(QJsonObject &in, QJsonObject *out) {
    QJsonArray array = in.value("parameters").toArray();
    for(int i = 0; i < array.size(); i ++) {
        QJsonObject cookieJson = array.at(i).toObject();
        bool flag = this->deleteCookie(cookieJson);
        QString name = cookieJson.value("name").toString();
        out->insert(name, flag);
    }
}
