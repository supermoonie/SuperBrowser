#ifndef COOKIEOPERATORDIALOG_H
#define COOKIEOPERATORDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QNetworkCookie>
#include <QDateTime>
#include "cookieeditdialog.h"

namespace Ui {
class CookieOperatorDialog;
}

class CookieOperatorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CookieOperatorDialog(QWidget *parent = 0);
    ~CookieOperatorDialog();

private slots:
    void on_addButton_clicked();
    void on_delButton_clicked();
    void on_clearButton_clicked();
    void on_okButton_clicked();
    void on_editButton_clicked();

signals:
    void cookieEdited(const QNetworkCookie &cookie);
    void refreshButtonClicked();
    void clearCookie();
    void delCookie(const QNetworkCookie &cookie);

public slots:
    void updateModel(const QList<QNetworkCookie> &cookieList);
    void onCookieEdited(const QNetworkCookie &cookie);

private:
    Ui::CookieOperatorDialog *ui;
    QStandardItemModel* model;
    CookieEditDialog* cookieEditDialog;
    QList<QNetworkCookie> cookies;
};

#endif // COOKIEOPERATORDIALOG_H
