#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QWebView>
#include <QLineEdit>
#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QWebView *view;
    QLineEdit *locationEdit;
    QAction *rotateAction;
    int progress;

private slots:
    void changeLocation();

signals:

public slots:
};

#endif // MAINWINDOW_H
