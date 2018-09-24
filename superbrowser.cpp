#include "superbrowser.h"

SuperBrowser::SuperBrowser(QObject* parent): QObject(parent)
{
    cookieJar = MemoryCookieJar::instance();
    networkAccessManager = new NetworkAccessManager(cookieJar, parent);
    webPage = WebPage::instance();
    webPage->setNetworkAccessManager(networkAccessManager);
    commandMap = new QMap<QString, FUN>;
    commandMap->insert("navigate", &SuperBrowser::navigate);
    commandMap->insert("getAllCookies", &SuperBrowser::getAllCookies);
    commandMap->insert("setCookie", &SuperBrowser::setCookie);
    receiveThread = new ReceiveThread(NULL);
    connect(receiveThread, &ReceiveThread::received, this, &SuperBrowser::onCommandReceived);
    receiveThread->start();
}

SuperBrowser::~SuperBrowser()
{
    receiveThread->deleteLater();
    cookieJar->deleteLater();
    webPage->deleteLater();
}

void SuperBrowser::close() {
    commandMap->clear();
    delete commandMap;
    receiveThread->stop();
    QApplication::exit(0);
}

void SuperBrowser::navigate(QJsonObject &in, QJsonObject* out) {
    webPage->action(QWebPage::Stop);
    // TODO 增加对修改head的支持
    QString url = in.value("parameters").toObject().value("url").toString("about:blank");
    webPage->currentFrame()->load(QUrl::fromUserInput(url));
    out->insert("code", 200);
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

void SuperBrowser::setCookie(QJsonObject &in, QJsonObject *out) {
    QJsonObject cookieJson = in.value("parameters").toObject();
    QString name = cookieJson.value("name").toString();
    if(name.isNull() || name.isEmpty()) {
        out->insert("success", false);
        return;
    }
    QString value = cookieJson.value("value").toString();
    if(name.isNull() || name.isEmpty()) {
        out->insert("success", false);
        return;
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
    bool flag = this->cookieJar->insertCookie(cookie);
    out->insert("success", flag);
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
    if(!commandMap->contains(name)) {
        QJsonObject errorJson;
        errorJson.insert("code", 404);
        errorJson.insert("desc", name + " not support");
        errorJson.insert("data", QJsonValue::Null);
        Terminal::instance()->cout(QString(QJsonDocument(errorJson).toJson()), true);
        return;
    }
    FUN fun = commandMap->value(name);
    QJsonObject* result = new QJsonObject;
    (this->*fun)(commandJson, result);
    Terminal::instance()->cout(QString(QJsonDocument(*result).toJson()), true);
    delete result;
}
