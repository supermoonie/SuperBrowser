#include "superbrowser.h"

SuperBrowser::SuperBrowser(QObject* parent): QObject(parent)
{
    cookieJar = MemoryCookieJar::instance(this);
    networkAccessManager = new NetworkAccessManager(cookieJar, this);
    webPage = WebPage::instance(this);
    webPage->setNetworkAccessManager(networkAccessManager);
    commandMap.insert("navigate", &SuperBrowser::navigate);
    commandMap.insert("getAllCookies", &SuperBrowser::getAllCookies);
    commandMap.insert("setCookie", &SuperBrowser::setCookie);
    commandMap.insert("setCookies", &SuperBrowser::setCookies);
    commandMap.insert("deleteCookie", &SuperBrowser::deleteCookie);
    commandMap.insert("deleteCookies", &SuperBrowser::deleteCookies);
    commandMap.insert("setProxy", &SuperBrowser::setProxy);
    commandMap.insert("setUserAgent", &SuperBrowser::setUserAgent);
    commandMap.insert("setInterceptor", &SuperBrowser::setInterceptor);
    commandMap.insert("addExtractor", &SuperBrowser::addExtractor);
    commandMap.insert("addExtractors", &SuperBrowser::addExtractors);
    commandMap.insert("getResponse", &SuperBrowser::getResponse);
    commandMap.insert("getResponses", &SuperBrowser::getResponses);
    receiveThread = new ReceiveThread(NULL);
    connect(receiveThread, &ReceiveThread::received, this, &SuperBrowser::onCommandReceived);
    receiveThread->start();
    QNetworkProxyFactory::setUseSystemConfiguration(true);
}

SuperBrowser::~SuperBrowser()
{

}

void SuperBrowser::close() {
    receiveThread->stop();
    QApplication::exit(0);
}

void SuperBrowser::navigate(QJsonObject &in, QJsonObject* out) {
    webPage->action(QWebPage::Stop);
    // TODO 增加对自定义head的支持
    QString url = in.value("parameters").toObject().value("url").toString("about:blank");
    webPage->currentFrame()->load(QUrl::fromUserInput(url));
    out->insert("code", 200);
}

void SuperBrowser::setProxy(QJsonObject &in, QJsonObject *out) {
    QJsonObject proxyJson = in.value("parameters").toObject();
    QNetworkProxy proxy;
    QString type = proxyJson.value("type").toString();
    if("NO_PROXY" == type) {
        proxy.setType(QNetworkProxy::NoProxy);
    } else {
        if("SOCKS5_PROXY" == type) {
            proxy.setType(QNetworkProxy::Socks5Proxy);
        } else {
            proxy.setType(QNetworkProxy::HttpProxy);
        }
        QString host = proxyJson.value("host").toString();
        int port = proxyJson.value("port").toInt();
        proxy.setHostName(host);
        proxy.setPort(port);
        QString user = in.value("user").toString();
        if(!user.isNull() && !user.isEmpty()) {
            proxy.setUser(user);
        }
        QString password = in.value("password").toString();
        if(!password.isNull() && !password.isEmpty()) {
            proxy.setPassword(password);
        }
    }

    QNetworkProxy::setApplicationProxy(proxy);
    out->insert("success", true);
}

void SuperBrowser::setUserAgent(QJsonObject &in, QJsonObject *out) {
    QString ua = in.value("parameters").toString();
    if(ua.isNull() || ua.isEmpty()) {
        out->insert("success", false);
        return;
    }
    this->webPage->setUserAgent(ua);
    out->insert("success", true);
}

void SuperBrowser::setInterceptor(QJsonObject &in, QJsonObject *out) {
    QString interceptor = in.value("parameters").toString();
    if(interceptor.isNull() || interceptor.isEmpty()) {
        out->insert("success", false);
        return;
    }
    bool flag = this->networkAccessManager->setInterceptor(QRegExp(interceptor));
    out->insert("success", flag);
}

bool SuperBrowser::addExtractor(const QString &extractor) {
    if(extractor.isNull() || extractor.isEmpty()) {
        return false;
    }
    return this->networkAccessManager->addExtractor(QRegExp(extractor));
}

void SuperBrowser::addExtractor(QJsonObject &in, QJsonObject *out) {
    QString extractor = in.value("parameters").toString();
    if(extractor.isNull() || extractor.isEmpty()) {
        return;
    }
    bool flag = this->addExtractor(extractor);
    out->insert(extractor, flag);
}

void SuperBrowser::addExtractors(QJsonObject &in, QJsonObject *out) {
    QJsonArray extractors = in.value("parameters").toArray();
    for(int i = 0; i < extractors.size(); i ++) {
        QString extractor = extractors.at(i).toString();
        if(extractor.isNull() || extractor.isEmpty()) {
            continue;
        }
        bool flag = this->addExtractor(extractor);
        out->insert(extractor, flag);
    }
}

QByteArray SuperBrowser::getResponse(const QString &extractor) {
    if(extractor.isNull() || extractor.isEmpty()) {
        return NULL;
    }
    return this->networkAccessManager->extract(QRegExp(extractor));
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
        cookies = cookieJar->cookies(NULL);
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

void SuperBrowser::onCommandReceived(const QString &rawCommand) {
    QJsonParseError * parseError = new QJsonParseError;
    QJsonDocument commandJsonDocument = QJsonDocument::fromJson(rawCommand.toUtf8(), parseError);
    if(parseError->error != QJsonParseError::NoError)
    {
        QJsonObject errorJson;
        errorJson.insert("code", 400);
        errorJson.insert("desc", parseError->errorString());
        errorJson.insert("data", QJsonValue::Null);
        Terminal::instance()->cout(QString(QJsonDocument(errorJson).toJson()), true);
        return;
    }
    QJsonObject commandJson = commandJsonDocument.object();
    QString name = commandJson.value("name").toString();
    if(name.isNull() || name.isEmpty()) {
        QJsonObject errorJson;
        errorJson.insert("code", 400);
        errorJson.insert("desc", "name lost");
        errorJson.insert("data", QJsonValue::Null);
        Terminal::instance()->cout(QString(QJsonDocument(errorJson).toJson()), true);
        return;
    }
    if("close" == name) {
        this->close();
        return;
    }
    if(!commandMap.contains(name)) {
        QJsonObject errorJson;
        errorJson.insert("code", 404);
        errorJson.insert("desc", name + " not support");
        errorJson.insert("data", QJsonValue::Null);
        Terminal::instance()->cout(QString(QJsonDocument(errorJson).toJson()), true);
        return;
    }
    FUN fun = commandMap.value(name);
    QJsonObject* result = new QJsonObject;
    (this->*fun)(commandJson, result);
    Terminal::instance()->cout(QString(QJsonDocument(*result).toJson()), true);
    delete result;
}
