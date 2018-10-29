#ifndef BASE64DATAVIEWDIALOG_H
#define BASE64DATAVIEWDIALOG_H

#include <QDialog>

namespace Ui {
class Base64DataViewDialog;
}

class Base64DataViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Base64DataViewDialog(QWidget *parent = 0);
    ~Base64DataViewDialog();
    void setBase64Data(const QString &base64Data);

private slots:
    void on_decodeToStrButton_clicked();

    void on_decodeToImgButton_clicked();

private:
    Ui::Base64DataViewDialog *ui;
};

#endif // BASE64DATAVIEWDIALOG_H
