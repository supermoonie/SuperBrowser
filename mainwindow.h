#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QWebView>
#include <QLineEdit>
#include <QMainWindow>
#include "tcpserver.h"
#include "webpage.h"

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

private slots:
    void changeLocation();
    void onStartTcpActionTriggered();
    void onStopTcpActionTriggered();

signals:

public slots:

};

#endif // MAINWINDOW_H
