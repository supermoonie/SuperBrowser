#ifndef COOKIEOPERATORDIALOG_H
#define COOKIEOPERATORDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QNetworkCookie>
#include <QDateTime>

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

public slots:
    void updateModel(const QList<QNetworkCookie> &cookieList);

private:
    Ui::CookieOperatorDialog *ui;
    QStandardItemModel* model;
};

#endif // COOKIEOPERATORDIALOG_H
