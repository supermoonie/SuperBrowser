#ifndef INTERCEPTORDIALOG_H
#define INTERCEPTORDIALOG_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class InterceptorDialog;
}

class InterceptorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InterceptorDialog(QWidget *parent = 0);
    ~InterceptorDialog();

private:
    Ui::InterceptorDialog *ui;

private slots:

    void on_addButton_clicked();
};

#endif // INTERCEPTORDIALOG_H
