#include "proxydialog.h"
#include "ui_proxydialog.h"
#include <QMessageBox>
#include <QRegExp>

ProxyDialog::ProxyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProxyDialog)
{
    ui->setupUi(this);
    QRegExp hostRegExp("^\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}$");
    QValidator* hostValidator = new QRegExpValidator(hostRegExp, this);
    ui->hostLineEdit->setValidator(hostValidator);
}

ProxyDialog::~ProxyDialog()
{
    delete ui;
}

void ProxyDialog::setProxy(QNetworkProxy proxy) {
    if(proxy.hostName().isEmpty() || proxy.port() == 0) {
        return;
    }
    this->proxy = proxy;
    ui->hostLineEdit->setText(this->proxy.hostName());
    ui->portBox->setValue(this->proxy.port());
    if(proxy.type() == QNetworkProxy::HttpProxy) {
        ui->typeBox->setCurrentText("HTTP");
    } else if (proxy.type() == QNetworkProxy::Socks5Proxy) {
        ui->typeBox->setCurrentText("SOCKS5");
    } else {
        ui->typeBox->setCurrentText("NONE");
    }
    if(!proxy.user().isEmpty()) {
        ui->usernameLineEdit->setText(proxy.user());
    }
    ui->passwordLineEdit->setText("");
}

void ProxyDialog::done(int r) {
    if(QDialog::Accepted == r) {
        QString host = ui->hostLineEdit->text();
        if(host.isEmpty()) {
            QMessageBox::warning(this, "Warning", "Host is empty!");
            return;
        }
        QRegExp hostRegExp("^\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}$");
        if(!hostRegExp.exactMatch(host)) {
            QMessageBox::warning(this, "Warning", "Error host!");
            return;
        }
        int port = ui->portBox->value();
        QString typeText = ui->typeBox->currentText();
        if(typeText == "HTTP") {
            proxy.setType(QNetworkProxy::HttpProxy);
        } else if (typeText == "SOCKS5") {
            proxy.setType(QNetworkProxy::Socks5Proxy);
        } else {
            proxy.setType(QNetworkProxy::NoProxy);
        }
        proxy.setHostName(host);
        proxy.setPort(port);
        QString username = ui->usernameLineEdit->text();
        if(!username.isEmpty()) {
            proxy.setUser(username);
        }
        QString password = ui->passwordLineEdit->text();
        if(!password.isEmpty()) {
            proxy.setPassword(password);
        }
    }
    QDialog::done(r);
}

QNetworkProxy ProxyDialog::getProxy() {
    return proxy;
}
