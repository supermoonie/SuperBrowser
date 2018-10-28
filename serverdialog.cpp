#include "serverdialog.h"
#include "ui_serverdialog.h"

ServerDialog::ServerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerDialog),
    port(0)
{
    ui->setupUi(this);
}

ServerDialog::~ServerDialog()
{
    delete ui;
}

void ServerDialog::setPort(int port) {
    if(port > 0) {
        this->port = port;
    }
}

int ServerDialog::getPort() {
    return this->port;
}

void ServerDialog::done(int r) {
    if(QDialog::Accepted == r) {
        this->port = ui->portBox->value();
    }
    QDialog::done(r);
}
