#include "mainwindow.h"
#include <QToolBar>
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QInputDialog>
#include <QNetworkProxy>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    progress(0),
    proxyDialog(NULL),
    interceptorDialog(NULL),
    serverDialog(NULL),
    extractorEditorDialog(NULL),
    cookieOperatorDialog(NULL)
{
    webSocketServer = new WebSocketServer(this);
    view = new QWebView(this);
    webPage = new WebPage(this);
    view->setPage(webPage);
    connect(webSocketServer, &WebSocketServer::commandReceived, webPage, &WebPage::onCommandReceived);
    connect(webPage, &WebPage::loadStarted, this, &MainWindow::onWebPageLoadStarted);
    connect(webPage, &WebPage::loadFinished, this, &MainWindow::onWebPageLoadFinished);
    connect(view, &QWebView::titleChanged, this, &MainWindow::onWebViewTitleChanged);
    connect(view, &QWebView::loadProgress, this, &MainWindow::onWebViewLoadProgress);

    locationEdit = new QLineEdit(this);
    locationEdit->setCursor(Qt::IBeamCursor);
    locationEdit->setSizePolicy(QSizePolicy::Expanding, locationEdit->sizePolicy().verticalPolicy());
    connect(locationEdit, &QLineEdit::returnPressed, this, &MainWindow::changeLocation);

    QToolBar *toolBar = addToolBar(tr("Navigation"));
    toolBar->addAction(view->pageAction(QWebPage::Back));
    toolBar->addAction(view->pageAction(QWebPage::Forward));
    toolBar->addAction(view->pageAction(QWebPage::Reload));
    toolBar->addAction(view->pageAction(QWebPage::Stop));
    toolBar->addWidget(locationEdit);

    // Server
    QMenu* serverMenu = menuBar()->addMenu("Server");
    startAction = new QAction("Start", this);
    connect(startAction, &QAction::triggered, this, &MainWindow::onStartActionTriggered);
    serverMenu->addAction(startAction);

    stopAction = new QAction("Stop", this);
    stopAction->setDisabled(true);
    connect(stopAction, &QAction::triggered, this, &MainWindow::onStopActionTriggered);
    serverMenu->addAction(stopAction);
    // Server

    // Settings
    QMenu* settingsMenu = menuBar()->addMenu("Settings");
    QAction* proxyAction = new QAction("Proxy", this);
    connect(proxyAction, &QAction::triggered, this, &MainWindow::onProxyActionTriggered);
    settingsMenu->addAction(proxyAction);

    QAction* userAgentAction = new QAction("User-Agent", this);
    connect(userAgentAction, &QAction::triggered, this, &MainWindow::onUserAgentActionTriggered);
    settingsMenu->addAction(userAgentAction);

    QAction* interceptorAction = new QAction("Interceptor", this);
    settingsMenu->addAction(interceptorAction);
    connect(interceptorAction, &QAction::triggered, this, &MainWindow::onInterceptorActionTriggered);
    // Settings

    // Network
    QMenu* networkMenu = menuBar()->addMenu("Network");
    QAction* extractorEeditorAction = new QAction("Extractor", this);
    connect(extractorEeditorAction, &QAction::triggered, this, &MainWindow::onExtractorEeditorActionTriggered);
    networkMenu->addAction(extractorEeditorAction);

    QAction* getCookieAction = new QAction("Cookie", this);
    connect(getCookieAction, &QAction::triggered, this, &MainWindow::onGetCookieActionTriggered);
    networkMenu->addAction(getCookieAction);
    // Network

    setCentralWidget(view);
    setUnifiedTitleAndToolBarOnMac(true);
}

MainWindow::~MainWindow()
{

}

void MainWindow::onGetCookieActionTriggered() {
    if(cookieOperatorDialog == NULL) {
        cookieOperatorDialog = new CookieOperatorDialog(this);
        connect(webPage, &WebPage::cookieChanged, cookieOperatorDialog, &CookieOperatorDialog::updateModel);
        connect(cookieOperatorDialog, &CookieOperatorDialog::cookieOperatorEdited, webPage, &WebPage::onCookieOperatorEdited);
        connect(cookieOperatorDialog, &CookieOperatorDialog::refreshButtonClicked, webPage, &WebPage::onCookieOperatorRefreshButtonClicked);
    }
    cookieOperatorDialog->show();
}

// Extracotr
void MainWindow::onExtractorEeditorActionTriggered() {
    if(extractorEditorDialog == NULL) {
        extractorEditorDialog = new ExtractorEditorDialog(this);
        connect(extractorEditorDialog, &ExtractorEditorDialog::extractorChanged, this, &MainWindow::onExtractorChanged);
        connect(webPage->getNetworkAccessManager(), &NetworkAccessManager::dataExtracted, this, &MainWindow::onDataExtracted);
    }
    extractorEditorDialog->show();
}

void MainWindow::onExtractorChanged(QStringList &extractors) {
    webPage->getNetworkAccessManager()->setExtractors(extractors);
}

void MainWindow::onDataExtracted(const QString &extractor, const QString &base64Data) {
    extractorEditorDialog->updateModel(extractor, base64Data);
}
// Extractor

void MainWindow::onInterceptorDialogAccepted() {
    QList<QString> interceptors = interceptorDialog->getInterceptors();
    if(interceptors.size() > 0) {
        webPage->setInterceptors(interceptors);
    }
}

void MainWindow::onInterceptorActionTriggered() {
    if(interceptorDialog == NULL) {
        interceptorDialog = new InterceptorDialog(this);
        connect(interceptorDialog, &InterceptorDialog::accepted, this, &MainWindow::onInterceptorDialogAccepted);
    }
    QList<QString> interceptros = webPage->getInterceptors();
    interceptorDialog->setInterceptors(interceptros);
    interceptorDialog->show();
}

void MainWindow::onWebPageLoadFinished() {
    this->progress = 100;
    onWebViewTitleChanged();
}

void MainWindow::onWebViewLoadProgress(int p) {
    this->progress = p;
    onWebViewTitleChanged();
}

void MainWindow::onWebViewTitleChanged() {
    QString title = view->title();
    if(title.isEmpty()) {
        title = view->url().toString();
    }
    if(this->progress <= 0 || this->progress >= 100) {
        setWindowTitle(title);
    } else {
        setWindowTitle(QString("%1 (%2%)").arg(title).arg(this->progress));
    }
}

void MainWindow::onUserAgentActionTriggered() {
    QString currentUserAgent = webPage->getUserAgent();
    QString userAgent = QInputDialog::getText(this, "User-Agent", "User-Agent: ", QLineEdit::Normal, currentUserAgent);
    if(userAgent.isEmpty()) {
        return;
    }
    webPage->setUserAgent(userAgent);
}

void MainWindow::changeLocation() {
    QString urlText = locationEdit->text();
    if(!urlText.startsWith("http") && !urlText.startsWith("file://")) {
        urlText = QString("http://") + urlText;
    }
    QUrl url = QUrl::fromUserInput(urlText);
    view->setUrl(url);
}

void MainWindow::onProxyDialogAccpted() {
    QNetworkProxy proxy = proxyDialog->getProxy();
    QNetworkProxy::setApplicationProxy(proxy);
}

void MainWindow::onProxyActionTriggered() {
    if(proxyDialog == NULL) {
        proxyDialog = new ProxyDialog(this);
        connect(proxyDialog, &ProxyDialog::accepted, this, &MainWindow::onProxyDialogAccpted);
    }
    QNetworkProxy proxy = QNetworkProxy::applicationProxy();
    proxyDialog->setProxy(proxy);
    proxyDialog->show();
}

// Server
void MainWindow::onServerDialogAccepted() {
    int port = serverDialog->getPort();
    if(!webSocketServer->listen(QHostAddress::Any, port)) {
        QMessageBox::warning(this, "warn", QString("could not listen on %1!").arg(port));
    } else {
        startAction->setDisabled(true);
        stopAction->setEnabled(true);
    }
}

void MainWindow::onStartActionTriggered() {
    if(serverDialog == NULL) {
        serverDialog = new ServerDialog(this);
        connect(serverDialog, &ServerDialog::accepted, this, &MainWindow::onServerDialogAccepted);
    }
    serverDialog->show();
}

void MainWindow::onStopActionTriggered() {
    webSocketServer->disconnect();
    webSocketServer->close();
    startAction->setEnabled(true);
    stopAction->setDisabled(true);
}
// Server

void MainWindow::onWebPageLoadStarted() {
    QUrl currentUrl = webPage->currentFrame()->url();
    locationEdit->setText(currentUrl.toString());
}
