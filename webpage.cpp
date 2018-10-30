#include "webpage.h"
#include <QBuffer>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkProxy>
#include <QDebug>
#include <QStandardPaths>

WebPage::WebPage(QObject* parent):
    QWebPage(parent),
    userAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.186 Safari/537.36")
{
    cookieJar = new MemoryCookieJar(this);
    connect(cookieJar, &MemoryCookieJar::cookieChanged, this, &WebPage::onCookieChanged);
    networkAccessManager = new NetworkAccessManager(cookieJar, this);
    this->setNetworkAccessManager(networkAccessManager);
    QWebSettings * settings = QWebSettings::globalSettings();
    settings->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
    settings->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    settings->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, true);
    settings->setAttribute(QWebSettings::LocalStorageEnabled, true);
    settings->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, true);
    settings->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    settings->setOfflineStorageDefaultQuota(20*1024*1024);
    settings->setOfflineStoragePath(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation));
    commandMap.insert("navigate", &WebPage::navigate);
    commandMap.insert("setProxy", &WebPage::setProxy);
    commandMap.insert("setUserAgent", &WebPage::setUserAgent);
    commandMap.insert("setInterceptors", &WebPage::setInterceptors);
    QNetworkProxyFactory::setUseSystemConfiguration(true);
}

WebPage::~WebPage()
{

}

void WebPage::onCookieOperatorRefreshButtonClicked() {
    QList<QNetworkCookie> cookieList = cookieJar->cookies(QUrl());
    emit cookieChanged(cookieList);
}

void WebPage::onCookieOperatorEdited(const QList<QNetworkCookie> &cookieList) {
    for(QNetworkCookie cookie: cookieList) {
        cookieJar->insertCookie(cookie);
    }
}

void WebPage::onCookieChanged() {
    QList<QNetworkCookie> cookieList = cookieJar->cookies(QUrl());
    emit cookieChanged(cookieList);
}

MemoryCookieJar* WebPage::getCookieJar() {
    return this->cookieJar;
}

NetworkAccessManager* WebPage::getNetworkAccessManager() {
    return this->networkAccessManager;
}

void WebPage::setUserAgent(const QString &ua) {
    this->userAgent = ua;
}

QString WebPage::getUserAgent() {
    return this->userAgent;
}

QString WebPage::userAgentForUrl(const QUrl &url) const {
    return userAgent;
}

QWebPage* WebPage::createWindow(WebWindowType type) {
    return this;
}

void WebPage::navigate(QJsonObject &in, QJsonObject &out) {
    QString url = in.value("parameters").toObject().value("url").toString("about:blank");
    this->currentFrame()->setUrl(QUrl::fromUserInput(url));
}

void WebPage::setProxy(QJsonObject &in, QJsonObject &out) {
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
        QString username = in.value("username").toString();
        if(!username.isNull() && !username.isEmpty()) {
            proxy.setUser(username);
        }
        QString password = in.value("password").toString();
        if(!password.isNull() && !password.isEmpty()) {
            proxy.setPassword(password);
        }
    }
    QNetworkProxy::setApplicationProxy(proxy);
}

void WebPage::setUserAgent(QJsonObject &in, QJsonObject &out) {
    QJsonObject userAgentJson = in.value("parameters").toObject();
    QString ua = userAgentJson.value("userAgent").toString();
    if(!ua.isEmpty()) {
        setUserAgent(ua);
    }
}

void WebPage::setInterceptors(QJsonObject &in, QJsonObject &out) {
    QJsonArray interceptorsArr = in.value("parameters").toArray();
    QList<QString> interceptors;
    for(int i = 0; i < interceptorsArr.size(); i ++) {
        QString interceptor = interceptorsArr.at(i).toString("");
        if(interceptor.isEmpty()) {
            continue;
        }
        interceptors.append(interceptor);
    }
    if(interceptors.size() > 0) {
        networkAccessManager->setInterceptors(interceptors);
    }
}

QList<QString> WebPage::getInterceptors() {
    return networkAccessManager->getInterceptors();
}

void WebPage::setInterceptors(const QList<QString> &interceptors) {
    networkAccessManager->setInterceptors(interceptors);
}

QImage WebPage::renderImage() {
    QRect frameRect;
    QSize viewportSize = this->viewportSize();
    QSize contentsSize = this->mainFrame()->contentsSize();
    frameRect = QRect(QPoint(0, 0), contentsSize);
    this->setViewportSize(contentsSize);
    QImage::Format format = QImage::Format_ARGB32_Premultiplied;
    QImage buffer(frameRect.size(), format);
    buffer.fill(Qt::transparent);
    QPainter painter;
    const int tileSize = 4096;
    int htiles = (buffer.width() + tileSize - 1) / tileSize;
    int vtiles = (buffer.height() + tileSize - 1) / tileSize;
    for (int x = 0; x < htiles; ++x) {
        for (int y = 0; y < vtiles; ++y) {

            QImage tileBuffer(tileSize, tileSize, format);
            tileBuffer.fill(Qt::transparent);

            // Render the web page onto the small tile first
            painter.begin(&tileBuffer);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setRenderHint(QPainter::TextAntialiasing, true);
            painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
            painter.translate(-frameRect.left(), -frameRect.top());
            painter.translate(-x * tileSize, -y * tileSize);
            this->mainFrame()->render(&painter, QRegion(frameRect));
            painter.end();

            // Copy the tile to the main buffer
            painter.begin(&buffer);
            painter.setCompositionMode(QPainter::CompositionMode_Source);
            painter.drawImage(x * tileSize, y * tileSize, tileBuffer);
            painter.end();
        }
    }
    return buffer;
}

void WebPage::onCommandReceived(QWebSocket* client, const QString &command) {
    QJsonParseError* parseError = new QJsonParseError;
    QJsonDocument commandJsonDocument = QJsonDocument::fromJson(command.toUtf8(), parseError);
    if(parseError->error != QJsonParseError::NoError) {
        QJsonObject errorJson;
        errorJson.insert("error", parseError->errorString());
        QByteArray errorResult = QJsonDocument(errorJson).toJson();
        client->sendTextMessage(QString(errorResult));
        client->flush();
        return;
    }
    QJsonObject commandJson = commandJsonDocument.object();
    QString name = commandJson.value("name").toString();
    if(name.isNull() || name.isEmpty()) {
        QJsonObject errorJson;
        errorJson.insert("error", "name lost");
        QByteArray errorResult = QJsonDocument(errorJson).toJson();
        client->sendTextMessage(QString(errorResult));
        client->flush();
        return;
    }
    if(!commandMap.contains(name)) {
        QJsonObject errorJson;
        errorJson.insert("error", name + " not support");
        QByteArray errorResult = QJsonDocument(errorJson).toJson();
        client->sendTextMessage(QString(errorResult));
        client->flush();
        return;
    }
    FUN fun = commandMap.value(name);
    QJsonObject result;
    (this->*fun)(commandJson, result);
    QByteArray resultData = QJsonDocument(result).toJson();
    client->sendTextMessage(QString(resultData));
    client->flush();
}
