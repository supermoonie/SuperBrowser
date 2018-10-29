#ifndef COOKIEOPERATORDIALOG_H
#define COOKIEOPERATORDIALOG_H

#include <QDialog>
#include <QStandardItemModel>

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

private:
    Ui::CookieOperatorDialog *ui;
    QStandardItemModel* model;
};

#endif // COOKIEOPERATORDIALOG_H
