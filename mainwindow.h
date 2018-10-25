#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QWebView>
#include <QLineEdit>
#include <QMainWindow>
#include "tcpserver.h"
#include "webpage.h"
#include "proxydialog.h"
#include "interceptordialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool startTcpServer(int port);

private:
    QWebView *view;
    WebPage* webPage;
    QLineEdit *locationEdit;
    QAction *rotateAction;
    QAction* startTcpAction;
    QAction* stopTcpAction;
    int progress;
    TcpServer* tcpServer;
    ProxyDialog* proxyDialog;
    InterceptorDialog* interceptorDialog;

private slots:
    void changeLocation();
    void onWebPageLoadStarted();
    void onWebPageLoadFinished();
    void onWebViewLoadProgress(int p);
    void onWebViewTitleChanged();
    void onStartTcpActionTriggered();
    void onStopTcpActionTriggered();
    void onProxyActionTriggered();
    void onProxyDialogAccpted();
    void onUserAgentActionTriggered();
    void onInterceptorActionTriggered();

signals:

public slots:

};

#endif // MAINWINDOW_H
