#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QWebView>
#include <QLineEdit>
#include <QMainWindow>
#include "websocketserver.h"
#include "webpage.h"
#include "proxydialog.h"
#include "interceptordialog.h"
#include "serverdialog.h"
#include "setextractorsdialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QWebView *view;
    WebPage* webPage;
    QLineEdit *locationEdit;
    QAction *rotateAction;
    QAction* startAction;
    QAction* stopAction;
    int progress;
    WebSocketServer* webSocketServer;
    ProxyDialog* proxyDialog;
    InterceptorDialog* interceptorDialog;
    ServerDialog* serverDialog;
    SetExtractorsDialog* setExtractorsDialog;

private slots:
    void changeLocation();
    void onWebPageLoadStarted();
    void onWebPageLoadFinished();
    void onWebViewLoadProgress(int p);
    void onWebViewTitleChanged();
    void onInterceptorDialogAccepted();

    // Server
    void onServerDialogAccepted();
    void onStartActionTriggered();
    void onStopActionTriggered();
    // Server

    void onProxyActionTriggered();
    void onProxyDialogAccpted();
    void onUserAgentActionTriggered();
    void onInterceptorActionTriggered();
    // Cookie
    void onGetCookieActionTriggered();

signals:

public slots:

};

#endif // MAINWINDOW_H
