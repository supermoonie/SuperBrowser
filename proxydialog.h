#ifndef PROXYDIALOG_H
#define PROXYDIALOG_H

#include <QDialog>

namespace Ui {
class ProxyDialog;
}

class ProxyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProxyDialog(QWidget *parent = 0);
    ~ProxyDialog();

private:
    Ui::ProxyDialog *ui;
};

#endif // PROXYDIALOG_H
