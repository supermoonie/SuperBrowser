#ifndef SETEXTRACTORSDIALOG_H
#define SETEXTRACTORSDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMap>
#include <QMessageBox>
#include <QRegExp>

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
    void extractorChanged(QMap<QString, QString> &extractorItems);

private slots:
    void on_addButton_clicked();
    void on_delButton_clicked();
    void on_clearButton_clicked();
    void on_saveButton_clicked();
    void onItemChanged(QStandardItem * item);

private:
    Ui::SetExtractorsDialog *ui;
    QStandardItemModel* model;

};

#endif // SETEXTRACTORSDIALOG_H
