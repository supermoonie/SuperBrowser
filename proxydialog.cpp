#include "proxydialog.h"
#include "ui_proxydialog.h"

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
