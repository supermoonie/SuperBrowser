#include "mainwindow.h"
#include <QToolBar>
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QInputDialog>
#include <QNetworkProxy>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    progress = 0;
    tcpServer = new TcpServer(this);
    connect(tcpServer, &TcpServer::newConnection, tcpServer, &TcpServer::onNewConnection);
    view = new QWebView(this);
    webPage = new WebPage(this);
    view->setPage(webPage);
    connect(tcpServer, &TcpServer::commandReceived, webPage, &WebPage::onCommandReceived);
    connect(webPage, &WebPage::commandProcessed, tcpServer, &TcpServer::write);
    connect(webPage, &WebPage::loadStarted, this, &MainWindow::onWebPageLoadStarted);

    locationEdit = new QLineEdit(this);
    locationEdit->setSizePolicy(QSizePolicy::Expanding, locationEdit->sizePolicy().verticalPolicy());
    connect(locationEdit, &QLineEdit::returnPressed, this, &MainWindow::changeLocation);

    QToolBar *toolBar = addToolBar(tr("Navigation"));
    toolBar->addAction(view->pageAction(QWebPage::Back));
    toolBar->addAction(view->pageAction(QWebPage::Forward));
    toolBar->addAction(view->pageAction(QWebPage::Reload));
    toolBar->addAction(view->pageAction(QWebPage::Stop));
    toolBar->addWidget(locationEdit);

    QMenu* tcpMenu = menuBar()->addMenu("TcpServer");
    startTcpAction = new QAction("Start", this);
    connect(startTcpAction, &QAction::triggered, this, &MainWindow::onStartTcpActionTriggered);
    tcpMenu->addAction(startTcpAction);

    stopTcpAction = new QAction("Stop", this);
    stopTcpAction->setDisabled(true);
    connect(stopTcpAction, &QAction::triggered, this, &MainWindow::onStopTcpActionTriggered);
    tcpMenu->addAction(stopTcpAction);

    QMenu* settingsMenu = menuBar()->addMenu("Settings");
    QAction* proxyAction = new QAction("Proxy", this);
    connect(proxyAction, &QAction::triggered, this, &MainWindow::onProxyActionTriggered);
    settingsMenu->addAction(proxyAction);

    proxyDialog = new ProxyDialog(this);

    setCentralWidget(view);
    setUnifiedTitleAndToolBarOnMac(true);
}

MainWindow::~MainWindow()
{

}

void MainWindow::changeLocation() {
    QUrl url = QUrl::fromUserInput(locationEdit->text());
    view->setUrl(url);
}

void MainWindow::onProxyActionTriggered() {
    QNetworkProxy proxy = QNetworkProxy::applicationProxy();
    proxyDialog->show();
}

void MainWindow::onStartTcpActionTriggered() {
    int port = QInputDialog::getInt(this, "port", "port", 9900, 1000, 100000, 10);
    bool tcpServerStarted = startTcpServer(port);
    if(!tcpServerStarted) {
        QMessageBox::warning(this, "warn", QString("could not listen on %1!").arg(port));
    } else {
        startTcpAction->setDisabled(true);
        stopTcpAction->setEnabled(true);
    }
}

void MainWindow::onStopTcpActionTriggered() {
    tcpServer->close();
    startTcpAction->setEnabled(true);
    stopTcpAction->setDisabled(true);
}

void MainWindow::onWebPageLoadStarted() {
    QUrl currentUrl = webPage->currentFrame()->url();
    locationEdit->setText(currentUrl.toString());
}

bool MainWindow::startTcpServer(int port) {
    if(!tcpServer->listen(QHostAddress::Any, port)) {
        return false;
    }
    return true;
}
