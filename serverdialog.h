#ifndef SERVERDIALOG_H
#define SERVERDIALOG_H

#include <QDialog>

namespace Ui {
class ServerDialog;
}

class ServerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ServerDialog(QWidget *parent = 0);
    ~ServerDialog();
    void setPort(int port);
    int getPort();

private:
    Ui::ServerDialog *ui;
    int port;

private slots:
    void done(int r);
};

#endif // SERVERDIALOG_H
