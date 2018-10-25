#ifndef INTERCEPTORDIALOG_H
#define INTERCEPTORDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include <QList>

namespace Ui {
class InterceptorDialog;
}

class InterceptorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InterceptorDialog(QWidget *parent = 0);
    ~InterceptorDialog();
    QList<QString> getInterceptors();

private:
    Ui::InterceptorDialog *ui;

private slots:
    void on_addButton_clicked();
    void on_removeButton_clicked();
    void on_clearButton_clicked();
    void on_okButton_clicked();
};

#endif // INTERCEPTORDIALOG_H
