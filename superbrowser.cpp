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

QJsonObject SuperBrowser::navigate(QJsonArray &parameters) {
    webPage->action(QWebPage::Stop);
    QString url = parameters.at(0).toObject().value("url").toString("about:blank");
    webPage->currentFrame()->load(QUrl::fromUserInput(url));
    QJsonObject json;
    json.insert("code", 200);
    return json;
}

QJsonObject SuperBrowser::getAllCookies(QJsonArray &parameters) {
    QList<QNetworkCookie> cookies;
    if(parameters.size() > 0) {
        for(int i = 0; i < parameters.size(); i ++) {
            QString url = parameters.at(i).toString();
            cookies.append(cookieJar->cookies(url));
        }
    } else {
        cookies = cookieJar->cookies(NULL);
    }
    QJsonObject json;
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
    json.insert("cookies", cookieArray);
    return json;
}

QJsonObject SuperBrowser::getCookies(QJsonArray &parameters) {

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
    QJsonArray parameters = commandJson.value("parameters").toArray();
    QJsonObject json = (this->*fun)(parameters);
    Terminal::instance()->cout(QString(QJsonDocument(json).toJson()), true);
}
