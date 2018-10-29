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
#include "extractoreditordialog.h"
#include "cookieoperatordialog.h"

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
    ExtractorEditorDialog* extractorEditorDialog;
    CookieOperatorDialog* cookieOperatorDialog;

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

    // Settings
    void onProxyActionTriggered();
    void onProxyDialogAccpted();
    void onUserAgentActionTriggered();
    void onInterceptorActionTriggered();
    // Settings

    // Cookie
    void onGetCookieActionTriggered();
    // Cookie

    // Extractor
    void onExtractorEeditorActionTriggered();
    void onExtractorChanged(QStringList &extractors);
    void onDataExtracted(const QString &extractor, const QString &base64Data);
    // Extractor

signals:

public slots:

};

#endif // MAINWINDOW_H
