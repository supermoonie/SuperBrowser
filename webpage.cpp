#include "webpage.h"
#include <QBuffer>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkProxy>
#include <QDebug>
#include <QStandardPaths>
#include <QApplication>
#include "websocketserver.h"
#include "mainwindow.h"

static WebPage* INSTANCE = NULL;

WebPage::WebPage(QObject* parent):
    QWebPage(parent),
    userAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.186 Safari/537.36")
{
    INSTANCE = this;
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
    commandMap.insert("getUserAgent", &WebPage::getUserAgent);
    commandMap.insert("setUserAgent", &WebPage::setUserAgent);
    commandMap.insert("setInterceptors", &WebPage::setInterceptors);
    commandMap.insert("getWindowBounds", &WebPage::getWindowBounds);
    commandMap.insert("setWindowBounds", &WebPage::setWindowBounds);
    commandMap.insert("getWindowState", &WebPage::getWindowState);
    commandMap.insert("setWindowState", &WebPage::setWindowState);
    commandMap.insert("close", &WebPage::close);
    QNetworkProxyFactory::setUseSystemConfiguration(true);
}

WebPage::~WebPage()
{

}

WebPage* WebPage::instance() {
    return INSTANCE;
}

void WebPage::onCookieEdited(const QNetworkCookie &cookie) {
    bool inserted = cookieJar->addCookie(cookie, QUrl());
    if(inserted) {
        QList<QNetworkCookie> cookieList = cookieJar->cookies(QUrl());
        emit cookieChanged(cookieList);
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

QList<QString> WebPage::getInterceptors() {
    return networkAccessManager->getInterceptors();
}

void WebPage::setInterceptors(const QList<QString> &interceptors) {
    networkAccessManager->setInterceptors(interceptors);
}

// Command
void WebPage::navigate(QJsonObject &in, QJsonObject &out) {
    QString url = in.value("params").toObject().value("url").toString("about:blank");
    this->currentFrame()->setUrl(QUrl::fromUserInput(url));
}

void WebPage::setProxy(QJsonObject &in, QJsonObject &out) {
    QJsonObject proxyJson = in.value("params").toObject();
    QNetworkProxy proxy;
    QString type = proxyJson.value("type").toString();
    if("NoProxy" == type) {
        proxy.setType(QNetworkProxy::NoProxy);
    } else {
        if("Socks5Proxy" == type) {
            proxy.setType(QNetworkProxy::Socks5Proxy);
        } else {
            proxy.setType(QNetworkProxy::HttpProxy);
        }
        QString host = proxyJson.value("host").toString();
        int port = proxyJson.value("port").toInt();
        proxy.setHostName(host);
        proxy.setPort(port);
        QString username = in.value("userName").toString();
        if(!username.isNull() && !username.isEmpty()) {
            proxy.setUser(username);
        }
        QString password = in.value("passWord").toString();
        if(!password.isNull() && !password.isEmpty()) {
            proxy.setPassword(password);
        }
    }
    QNetworkProxy::setApplicationProxy(proxy);
}

void WebPage::getUserAgent(QJsonObject &in, QJsonObject &out) {
    out.insert("userAgent", this->userAgent);
}

void WebPage::setUserAgent(QJsonObject &in, QJsonObject &out) {
    QJsonObject userAgentJson = in.value("params").toObject();
    QString ua = userAgentJson.value("userAgent").toString();
    if(!ua.isEmpty()) {
        setUserAgent(ua);
    }
}

void WebPage::setInterceptors(QJsonObject &in, QJsonObject &out) {
    QJsonArray interceptorsArr = in.value("params").toArray();
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

void WebPage::getWindowBounds(QJsonObject &in, QJsonObject &out) {
    QRect rect = MainWindow::instance()->frameGeometry();
    out.insert("x", rect.x());
    out.insert("y", rect.y());
    out.insert("width", rect.width());
    out.insert("height", rect.height());
}

void WebPage::setWindowBounds(QJsonObject &in, QJsonObject &out) {
    QJsonObject boundsJson = in.value("params").toObject();
    if(boundsJson.contains("bounds")) {
        boundsJson = boundsJson.value("bounds").toObject();
    }
    MainWindow* window = MainWindow::instance();
    QRect frameGeometry = window->frameGeometry();
    QRect geometry = window->geometry();
    int xDiff = geometry.x() - frameGeometry.x();
    int yDiff = geometry.y() - frameGeometry.y();
    int wDiff = frameGeometry.width() - geometry.width();
    int hDiff = frameGeometry.height() - geometry.height();
    int x = boundsJson.value("x").toInt(frameGeometry.x());
    int y = boundsJson.value("y").toInt(frameGeometry.y());
    int width = boundsJson.value("width").toInt(frameGeometry.width());
    int height = boundsJson.value("height").toInt(frameGeometry.width());
    MainWindow::instance()->setGeometry(x + xDiff, y + yDiff, width - wDiff, height - hDiff);
}

void WebPage::getWindowState(QJsonObject &in, QJsonObject &out) {
    Qt::WindowStates windowState = MainWindow::instance()->windowState();
    QString state;
    switch (windowState) {
    case Qt::WindowNoState:
        state = "normal";
        break;
    case Qt::WindowMinimized:
        state = "minimized";
        break;
    case Qt::WindowMaximized:
        state = "maximized";
        break;
    case Qt::WindowFullScreen:
        state = "fullscreen";
        break;
    case Qt::WindowActive:
        state = "active";
        break;
    default:
        state = "unknow";
        break;
    }
    out.insert("state", state);
}

void WebPage::setWindowState(QJsonObject &in, QJsonObject &out) {
    QString state = in.value("params").toObject().value("state").toString("normal");
    MainWindow* window = MainWindow::instance();
    if(state == "normal" && Qt::WindowNoState != window->windowState()) {
        window->showNormal();
    } else if(state == "minimized" && !window->isMinimized()) {
        window->showMinimized();
    } else if(state == "maximized" && !window->isMaximized()) {
        window->showMaximized();
    } else if(state == "fullscreen" && !window->isFullScreen()) {
        window->showFullScreen();
    } else if(state == "active" && !window->isActiveWindow()) {
        window->activateWindow();
    }
}

void WebPage::close(QJsonObject &in, QJsonObject &out) {
    WebSocketServer::instance()->exit();
    MainWindow::instance()->close();
    QApplication::exit(0);
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
