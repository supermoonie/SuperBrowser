#include "superbrowser.h"

SuperBrowser::SuperBrowser(QObject* parent): QObject(parent)
{
    cookieJar = MemoryCookieJar::instance();
    webPage = WebPage::instance();
    commandMap = new QMap<QString, FUN>;
    commandMap->insert("navigate", &SuperBrowser::navigate);
    receiveThread = new ReceiveThread(NULL);
    connect(receiveThread, &ReceiveThread::received, this, &SuperBrowser::onCommandReceived);
    receiveThread->start();
}

SuperBrowser::~SuperBrowser()
{
    commandMap->clear();
    delete commandMap;
    receiveThread->stop();
    receiveThread->deleteLater();
    cookieJar->deleteLater();
    webPage->deleteLater();
}

QJsonObject SuperBrowser::navigate(QJsonArray &parameters) {
    webPage->action(QWebPage::Stop);
    QString url = parameters.at(0).toObject().value("url").toString("about:blank");
    webPage->currentFrame()->load(QUrl::fromUserInput(url));
    QJsonObject json;
    json.insert("code", 200);
    return json;
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
