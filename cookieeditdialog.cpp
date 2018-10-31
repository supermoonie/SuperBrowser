#include "cookieeditdialog.h"
#include "ui_cookieeditdialog.h"

CookieEditDialog::CookieEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CookieEditDialog)
{
    ui->setupUi(this);
    op = ADD;
}

CookieEditDialog::~CookieEditDialog()
{
    delete ui;
}

void CookieEditDialog::reset() {
    ui->nameLineEdit->clear();
    ui->valueLineEdit->clear();
    ui->domainLineEdit->clear();
    ui->pathLineEdit->clear();
    ui->expiresDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->httpOnlyCheckBox->setCheckState(Qt::Unchecked);
    ui->secureCheckBox->setCheckState(Qt::Unchecked);
}

void CookieEditDialog::setOperate(Operate op) {
    this->op = op;
}

CookieEditDialog::Operate CookieEditDialog::getOperate() const {
    return this->op;
}

void CookieEditDialog::setCookie(const QNetworkCookie &cookie) {
    ui->nameLineEdit->setText(cookie.name());
    ui->valueLineEdit->setText(cookie.value());
    ui->domainLineEdit->setText(cookie.domain());
    ui->pathLineEdit->setText(cookie.path());
    ui->expiresDateTimeEdit->setDateTime(cookie.expirationDate());
    ui->httpOnlyCheckBox->setCheckState(cookie.isHttpOnly() ? Qt::Checked : Qt::Unchecked);
    ui->secureCheckBox->setCheckState(cookie.isSecure() ? Qt::Checked : Qt::Unchecked);
}

void CookieEditDialog::accept() {
    QString name = ui->nameLineEdit->text();
    if(name.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Name Is Empty !");
        ui->nameLineEdit->setFocus();
        return;
    }
    QString domain = ui->domainLineEdit->text();
    if(domain.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Domain Is Empty !");
        ui->domainLineEdit->setFocus();
        return;
    }
    QString value = ui->valueLineEdit->text();
    QString path = ui->pathLineEdit->text();
    QDateTime dateTime = ui->expiresDateTimeEdit->dateTime();
    if(path.isEmpty()) {
        path = "/";
    }
    QNetworkCookie cookie(name.toUtf8(), value.toUtf8());
    cookie.setDomain(domain);
    cookie.setPath(path);
    cookie.setExpirationDate(dateTime);
    cookie.setHttpOnly(ui->httpOnlyCheckBox->isChecked());
    cookie.setSecure(ui->secureCheckBox->isChecked());
    emit cookieEdited(cookie);
    QDialog::accept();
}
