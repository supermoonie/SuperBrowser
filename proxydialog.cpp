#include "proxydialog.h"
#include "ui_proxydialog.h"
#include <QMessageBox>

ProxyDialog::ProxyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProxyDialog)
{
    ui->setupUi(this);
}

ProxyDialog::~ProxyDialog()
{
    delete ui;
}


void ProxyDialog::on_buttons_accepted()
{
    QMessageBox::about(this, "About", "Hello Qt!");
}
