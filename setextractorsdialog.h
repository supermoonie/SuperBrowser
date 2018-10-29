#ifndef SETEXTRACTORSDIALOG_H
#define SETEXTRACTORSDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMap>
#include <QMessageBox>
#include <QRegExp>
#include <QMenu>
#include <QAction>
#include "base64dataviewdialog.h"

namespace Ui {
class SetExtractorsDialog;
}

class SetExtractorsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetExtractorsDialog(QWidget *parent = 0);
    ~SetExtractorsDialog();
    void updateModel(const QString &extractor, const QString &base64Data);

signals:
    void extractorChanged(QStringList &extractors);

private slots:
    void on_addButton_clicked();
    void on_delButton_clicked();
    void on_clearButton_clicked();
    void on_okButton_clicked();
    void onItemChanged(QStandardItem * item);
    void onCustomContextMenuRequested(const QPoint &pos);
    void onViewActionTriggered();

private:
    Ui::SetExtractorsDialog *ui;
    QStandardItemModel* model;
    QMenu* rightMenu;
    QAction* viewAction;
    Base64DataViewDialog* base64DataViewDialog;

};

#endif // SETEXTRACTORSDIALOG_H
