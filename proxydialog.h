#ifndef PROXYDIALOG_H
#define PROXYDIALOG_H

#include <QDialog>
#include <QNetworkProxy>

namespace Ui {
class ProxyDialog;
}

class ProxyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProxyDialog(QWidget *parent = 0);
    ~ProxyDialog();
    void setProxy(QNetworkProxy proxy);
    QNetworkProxy getProxy();

private slots:
    void done(int r);

private:
    Ui::ProxyDialog *ui;
    QNetworkProxy proxy;
};

#endif // PROXYDIALOG_H
