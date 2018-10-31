#ifndef COOKIEEDITDIALOG_H
#define COOKIEEDITDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QNetworkCookie>

namespace Ui {
class CookieEditDialog;
}

class CookieEditDialog : public QDialog
{
    Q_OBJECT
    Q_ENUMS(Operate)

public:
    enum Operate{
        ADD,
        EDIT
    };
    explicit CookieEditDialog(QWidget *parent = 0);
    ~CookieEditDialog();
    void reset();
    void setOperate(Operate op);
    Operate getOperate() const;
    void setCookie(const QNetworkCookie &cookie);

private:
    Ui::CookieEditDialog *ui;
    Operate op;

signals:
    void cookieEdited(const QNetworkCookie &cookie);

public slots:
    virtual void accept() Q_DECL_OVERRIDE;
};

#endif // COOKIEEDITDIALOG_H
