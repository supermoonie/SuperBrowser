#include "webpage.h"
#include <QBuffer>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkProxy>
#include <QDebug>
#include <QStandardPaths>
#include <QApplication>
#include <QLabel>
#include <QWebElement>
#include "websocketserver.h"
#include "mainwindow.h"

static WebPage* INSTANCE = NULL;

WebPage::WebPage(QObject* parent):
    QWebPage(parent),
    userAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.186 Safari/537.36")
{
    INSTANCE = this;
    alertBox = new QMessageBox(MainWindow::instance()->getWebView());
    confirmBox = new QMessageBox(MainWindow::instance()->getWebView());
    confirmBox->addButton(tr("Cancel"), QMessageBox::RejectRole);
    confirmBox->addButton(tr("Ok"), QMessageBox::AcceptRole);
    promptBox = new QInputDialog(MainWindow::instance()->getWebView());
    QLabel* label = promptBox->findChild<QLabel*>();
    if(label) {
        label->setTextFormat(Qt::PlainText);
    }
    promptBox->setTextEchoMode(QLineEdit::Normal);
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
    commandMap.insert("getVersion", &WebPage::getVersion);
    commandMap.insert("setProxy", &WebPage::setProxy);
    commandMap.insert("setInterceptors", &WebPage::setInterceptors);
    commandMap.insert("getWindowBounds", &WebPage::getWindowBounds);
    commandMap.insert("setWindowBounds", &WebPage::setWindowBounds);
    commandMap.insert("getWindowState", &WebPage::getWindowState);
    commandMap.insert("setWindowState", &WebPage::setWindowState);
    commandMap.insert("close", &WebPage::close);
    commandMap.insert("navigate", &WebPage::navigate);
    commandMap.insert("currentUrl", &WebPage::currentUrl);
    commandMap.insert("getUserAgent", &WebPage::getUserAgent);
    commandMap.insert("setUserAgent", &WebPage::setUserAgent);
    commandMap.insert("hasAlert", &WebPage::hasAlert);
    commandMap.insert("alertText", &WebPage::alertText);
    commandMap.insert("closeAlert", &WebPage::closeAlert);
    commandMap.insert("hasConfirm", &WebPage::hasConfirm);
    commandMap.insert("confirmText", &WebPage::confirmText);
    commandMap.insert("acceptConfirm", &WebPage::acceptConfirm);
    commandMap.insert("hasPrompt", &WebPage::hasPrompt);
    commandMap.insert("setPromptValue", &WebPage::setPromptValue);
    commandMap.insert("acceptPrompt", &WebPage::acceptPrompt);
    commandMap.insert("setHtml", &WebPage::setHtml);
    commandMap.insert("toHtml", &WebPage::toHtml);
    commandMap.insert("toPlainText", &WebPage::toPlainText);
    commandMap.insert("setScrollBarPolicy", &WebPage::setScrollBarPolicy);
    commandMap.insert("captureScreenshot", &WebPage::captureScreenshot);
    QNetworkProxyFactory::setUseSystemConfiguration(true);
    connect(this, &WebPage::loadFinished, [](){
        QJsonObject data;
        data.insert("event", "loadFinished");
        QJsonObject result;
        data.insert("result", result);
        WebSocketServer::instance()->sendTextMessageToAllClient(QString(QJsonDocument(data).toJson()));
    });
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
    Q_UNUSED(url);
    return userAgent;
}

QWebPage* WebPage::createWindow(WebWindowType type) {
    Q_UNUSED(type);
    return this;
}

void WebPage::javaScriptAlert(QWebFrame *frame, const QString &msg) {
    Q_UNUSED(frame);
    alertBox->setTextFormat(Qt::PlainText);
    alertBox->setWindowTitle(tr("Alert - %1").arg(this->currentFrame()->url().host()));
    alertBox->setText(msg);
    alertBox->setStandardButtons(QMessageBox::Ok);
    alertBox->exec();
}

bool WebPage::javaScriptConfirm(QWebFrame *originatingFrame, const QString &msg) {
    Q_UNUSED(originatingFrame);
    confirmBox->setTextFormat(Qt::PlainText);
    confirmBox->setWindowTitle(tr("Confirm - %1").arg(this->currentFrame()->url().host()));
    confirmBox->setText(msg);
    int code = confirmBox->exec();
    if(code == QMessageBox::Accepted) {
        return true;
    } else {
        qDebug() << "cancel";
        return false;
    }
}

bool WebPage::javaScriptPrompt(QWebFrame *originatingFrame, const QString &msg, const QString &defaultValue, QString *result) {
    Q_UNUSED(originatingFrame);
    promptBox->setLabelText(msg);
    promptBox->setTextValue(defaultValue);
    bool ok = !!promptBox->exec();
    if(ok && result) {
        *result = promptBox->textValue();
    }
    return ok;
}

QList<QString> WebPage::getInterceptors() {
    return networkAccessManager->getInterceptors();
}

void WebPage::setInterceptors(const QList<QString> &interceptors) {
    networkAccessManager->setInterceptors(interceptors);
}

// Command
void WebPage::getVersion(QJsonObject &in, QJsonObject &out) {
    Q_UNUSED(in);
    QJsonObject result;
    result.insert("major", 1);
    result.insert("minor", 0);
    out.insert("result", result);
}

void WebPage::setProxy(QJsonObject &in, QJsonObject &out) {
    Q_UNUSED(out);
    QJsonObject proxyJson = in.value("params").toObject();
    QJsonDocument document(proxyJson);
    qDebug() << QString(document.toJson());
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
    Q_UNUSED(in);
    QJsonObject userAgentJson;
    userAgentJson.insert("userAgent", this->userAgent);
    out.insert("result", userAgentJson);
}

void WebPage::setUserAgent(QJsonObject &in, QJsonObject &out) {
    Q_UNUSED(out);
    QJsonObject userAgentJson = in.value("params").toObject();
    QString ua = userAgentJson.value("userAgent").toString();
    if(!ua.isEmpty()) {
        setUserAgent(ua);
    }
}

void WebPage::hasAlert(QJsonObject &in, QJsonObject &out) {
    Q_UNUSED(in);
    bool isHidden = alertBox->isHidden();
    QJsonObject result;
    result.insert("exist", !isHidden);
    out.insert("result", result);
}

void WebPage::alertText(QJsonObject &in, QJsonObject &out) {
    Q_UNUSED(in);
    if(alertBox->isHidden()) {
        out.insert("error", "No Alert!");
    } else {
        QJsonObject result;
        result.insert("text", alertBox->text());
        out.insert("result", result);
    }
}

void WebPage::closeAlert(QJsonObject &in, QJsonObject &out) {
    Q_UNUSED(in);
    Q_UNUSED(out);
    alertBox->accept();
}

void WebPage::hasConfirm(QJsonObject &in, QJsonObject &out) {
    Q_UNUSED(in);
    bool isHidden = confirmBox->isHidden();
    QJsonObject result;
    result.insert("exist", !isHidden);
    out.insert("result", result);
}

void WebPage::confirmText(QJsonObject &in, QJsonObject &out) {
    Q_UNUSED(in);
    if(confirmBox->isHidden()) {
        out.insert("error", "No Confirm!");
    } else {
        QJsonObject result;
        result.insert("text", confirmBox->text());
        out.insert("result", result);
    }
}

void WebPage::acceptConfirm(QJsonObject &in, QJsonObject &out) {
    if(confirmBox->isHidden()) {
        out.insert("error", "No Confirm!");
    } else {
        bool accept = in.value("params").toObject().value("accept").toBool(true);
        if(accept) {
            confirmBox->accept();
        } else {
            confirmBox->reject();
        }
    }
}

void WebPage::hasPrompt(QJsonObject &in, QJsonObject &out) {
    Q_UNUSED(in);
    bool isHidden = promptBox->isHidden();
    QJsonObject result;
    result.insert("exist", !isHidden);
    out.insert("result", result);
}

void WebPage::setPromptValue(QJsonObject &in, QJsonObject &out) {
    Q_UNUSED(out);
    if(promptBox->isHidden()) {
        out.insert("error", "No Prompt!");
    } else {
        QString text = in.value("params").toObject().value("text").toString("");
        promptBox->setTextValue(text);
    }
}

void WebPage::acceptPrompt(QJsonObject &in, QJsonObject &out) {
    Q_UNUSED(out);
    if(promptBox->isHidden()) {
        out.insert("error", "No Prompt!");
    } else {
        bool accept = in.value("params").toObject().value("accept").toBool(false);
        if(accept) {
            promptBox->accept();
        } else {
            promptBox->reject();
        }
    }
}

void WebPage::setHtml(QJsonObject &in, QJsonObject &out) {
    Q_UNUSED(out);
    QJsonObject htmlJson = in.value("params").toObject();
    QString html = htmlJson.value("html").toString("");
    QUrl url = QUrl::fromUserInput(htmlJson.value("baseUrl").toString(""));
    QWebView* webView = MainWindow::instance()->getWebView();
    webView->setHtml(html, url);
}

void WebPage::toHtml(QJsonObject &in, QJsonObject &out) {
    Q_UNUSED(in);
    QString html = this->mainFrame()->toHtml();
    QJsonObject result;
    result.insert("html", html);
    out.insert("result", result);
}

void WebPage::toPlainText(QJsonObject &in, QJsonObject &out) {
    Q_UNUSED(in);
    QString text = this->mainFrame()->toPlainText();
    QJsonObject result;
    result.insert("text", text);
    out.insert("result", result);
}

void WebPage::setScrollBarPolicy(QJsonObject &in, QJsonObject &out) {
    Q_UNUSED(out);
    QJsonObject paramJson = in.value("params").toObject();
    QString horizontalPolicy = paramJson.value("horizontalPolicy").toString("ScrollBarAsNeeded");
    QString verticalPolicy = paramJson.value("verticalPolicy").toString("ScrollBarAsNeeded");
    if(horizontalPolicy == "ScrollBarAlwaysOff") {
        this->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
        QSize originSize = MainWindow::instance()->size();
        QSize size(originSize.width() + 100, originSize.height() + 100);
        MainWindow::instance()->resize(size);
        MainWindow::instance()->resize(originSize);
    } else if(horizontalPolicy == "ScrollBarAlwaysOn") {
        this->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOn);
    } else {
        this->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAsNeeded);
    }
    if(verticalPolicy == "ScrollBarAlwaysOff") {
        this->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
        QSize originSize = MainWindow::instance()->size();
        QSize size(originSize.width() + 100, originSize.height() + 100);
        MainWindow::instance()->resize(size);
        MainWindow::instance()->resize(originSize);
    } else if(verticalPolicy == "ScrollBarAlwaysOn") {
        this->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOn);
    } else {
        this->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAsNeeded);
    }

}

void WebPage::captureScreenshot(QJsonObject &in, QJsonObject &out) {
    QJsonObject paramJson = in.value("params").toObject();
    QString format = "png";
    int quality = 20;
    QSize contentsSize = this->mainFrame()->contentsSize();
    QSize viewportSize = this->viewportSize();
    QRect frameRect(QPoint(0, 0), viewportSize);
    if(paramJson.contains("format") && paramJson.contains("quality") && paramJson.contains("clip")) {
        QString imageFormat = paramJson.value("format").toString();
        if(!imageFormat.isEmpty() && imageFormat == "jpeg") {
            format = "jpeg";
        }
        int imageQuality = paramJson.value("quality").toInt();
        if(imageQuality >= 0 && imageQuality <= 100) {
            quality = imageQuality;
        }
        QJsonObject clipJson = paramJson.value("clip").toObject();
        int x = clipJson.value("x").toInt();
        int y = clipJson.value("y'").toInt();
        int width = clipJson.value("width").toInt();
        int height = clipJson.value("height").toInt();
        if(x >= 0 && width > 0 && (x + width) <= contentsSize.width() &&
                y >= 0 && height > 0 &&(y + height) <= contentsSize.height()) {
            frameRect.setX(x);
            frameRect.setY(y);
            frameRect.setWidth(width);
            frameRect.setHeight(height);
        }
    } else if(paramJson.contains("fullScreen")) {
        bool fullScreen = in.value("params").toObject().value("fullScreen").toBool(false);
        if(fullScreen) {
            frameRect.setSize(contentsSize);
        }
    }
    QByteArray base64Image = this->renderImage(format, quality, frameRect);
    QJsonObject result;
    result.insert("data", tr(base64Image));
    out.insert("result", result);
}

void WebPage::setInterceptors(QJsonObject &in, QJsonObject &out) {
    Q_UNUSED(out);
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
    Q_UNUSED(in);
    QRect rect = MainWindow::instance()->frameGeometry();
    QJsonObject result;
    result.insert("x", rect.x());
    result.insert("y", rect.y());
    result.insert("width", rect.width());
    result.insert("height", rect.height());
    out.insert("result", result);
}

void WebPage::setWindowBounds(QJsonObject &in, QJsonObject &out) {
    Q_UNUSED(out);
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
    Q_UNUSED(in);
    QJsonObject result;
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
    result.insert("state", state);
    out.insert("result", result);
}

void WebPage::setWindowState(QJsonObject &in, QJsonObject &out) {
    Q_UNUSED(out);
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
    Q_UNUSED(in);
    Q_UNUSED(out);
}

void WebPage::navigate(QJsonObject &in, QJsonObject &out) {
    Q_UNUSED(out);
    if(!alertBox->isHidden()) {
        return;
    }
    QString url = in.value("params").toObject().value("url").toString("about:blank");
    this->mainFrame()->setUrl(QUrl::fromUserInput(url));
}

void WebPage::currentUrl(QJsonObject &in, QJsonObject &out) {
    Q_UNUSED(in);
    QString currentUrl = this->mainFrame()->url().toString();
    QJsonObject result;
    result.insert("currentUrl", currentUrl);
    out.insert("result", result);
}

QByteArray WebPage::renderImage(const QString &format, int quality, const QRect &frameRect) {
    this->setViewportSize(frameRect.size());
    QImage img(frameRect.size(), QImage::Format_ARGB32_Premultiplied);
    img.fill(Qt::transparent);
    QPainter painter;
    const int tileSize = 4096;
    int htiles = (img.width() + tileSize - 1) / tileSize;
    int vtiles = (img.height() + tileSize - 1) / tileSize;
    for (int x = 0; x < htiles; ++x) {
        for (int y = 0; y < vtiles; ++y) {
            QImage tileImg(tileSize, tileSize, QImage::Format_ARGB32_Premultiplied);
            tileImg.fill(Qt::transparent);
            // Render the web page onto the small tile first
            painter.begin(&tileImg);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setRenderHint(QPainter::TextAntialiasing, true);
            painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
            painter.translate(-frameRect.left(), -frameRect.top());
            painter.translate(-x * tileSize, -y * tileSize);
            this->mainFrame()->render(&painter, QRegion(frameRect));
            painter.end();
            // Copy the tile to the main buffer
            painter.begin(&img);
            painter.setCompositionMode(QPainter::CompositionMode_Source);
            painter.drawImage(x * tileSize, y * tileSize, tileImg);
            painter.end();
        }
    }
    QByteArray imgByteArray;
    QBuffer buf(&imgByteArray);
    img.save(&buf, format.toUtf8().data(), quality);
    QByteArray hexed = imgByteArray.toBase64();
    buf.close();
    return hexed;
}

void WebPage::onCommandReceived(QWebSocket* client, const QString &command) {
    qDebug() << command;
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
    int id = commandJson.value("id").toInt(-1);
    if(-1 == id) {
        QJsonObject errorJson;
        errorJson.insert("error", "id lost");
        QByteArray errorResult = QJsonDocument(errorJson).toJson();
        client->sendTextMessage(QString(errorResult));
        client->flush();
        return;
    }
    QString method = commandJson.value("method").toString();
    if(method.isNull() || method.isEmpty()) {
        QJsonObject errorJson;
        errorJson.insert("error", "method lost");
        QByteArray errorResult = QJsonDocument(errorJson).toJson();
        client->sendTextMessage(QString(errorResult));
        client->flush();
        return;
    }
    if(!commandMap.contains(method)) {
        QJsonObject errorJson;
        errorJson.insert("error", method + " not support");
        QByteArray errorResult = QJsonDocument(errorJson).toJson();
        client->sendTextMessage(QString(errorResult));
        client->flush();
        return;
    }
    FUN fun = commandMap.value(method);
    QJsonObject result;
    result.insert("id", id);
    (this->*fun)(commandJson, result);
    QByteArray resultData = QJsonDocument(result).toJson();
    client->sendTextMessage(QString(resultData));
    client->flush();
    if(method == "close") {
        connect(client, &QWebSocket::disconnected, [](){
            QApplication::exit();
        });
    }
}
