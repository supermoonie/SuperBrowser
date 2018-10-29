#include "base64dataviewdialog.h"
#include "ui_base64dataviewdialog.h"

Base64DataViewDialog::Base64DataViewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Base64DataViewDialog)
{
    ui->setupUi(this);
}

Base64DataViewDialog::~Base64DataViewDialog()
{
    delete ui;
}

void Base64DataViewDialog::setBase64Data(const QString &base64Data) {
    ui->base64TextEdit->setPlainText(base64Data);
}

void Base64DataViewDialog::on_decodeToStrButton_clicked() {
    ui->decodedTextEdit->setPlainText("");
    QString base64Data = ui->base64TextEdit->toPlainText();
    QString decodedText(QByteArray::fromBase64(base64Data.toUtf8()));
    ui->decodedTextEdit->setPlainText(decodedText);
}

void Base64DataViewDialog::on_decodeToImgButton_clicked() {
    ui->decodedTextEdit->setHtml("");
    QString base64Data = ui->base64TextEdit->toPlainText();
    QString html = QString("<img src=\"data:image/png;base64,%1\"/>").arg(base64Data);
    ui->decodedTextEdit->setHtml(html);
}
