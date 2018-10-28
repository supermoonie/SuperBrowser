#include "setextractorsdialog.h"
#include "ui_setextractorsdialog.h"
#include <QDebug>

SetExtractorsDialog::SetExtractorsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetExtractorsDialog)
{
    ui->setupUi(this);
    model = new QStandardItemModel(this);
    connect(model, &QStandardItemModel::itemChanged, this, &SetExtractorsDialog::onItemChanged);
    model->setColumnCount(2);
    model->setHeaderData(0, Qt::Horizontal, "Extractor");
    model->setHeaderData(1, Qt::Horizontal, "Base64Data");
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->setMinimumWidth(30);
}

SetExtractorsDialog::~SetExtractorsDialog()
{
    delete ui;
}

void SetExtractorsDialog::updateModel(const QString &extractor, const QString &base64Data) {
    int count = model->rowCount();
    for(int row = 0; row < count; row ++) {
        QStandardItem * firstItem = model->item(row, 0);
        if(!firstItem->text().isEmpty() && firstItem->text() == extractor) {
            QStandardItem * secondItem = model->item(row, 1);
            secondItem->setText(base64Data);
        }

    }
}

void SetExtractorsDialog::onItemChanged(QStandardItem * item) {
    if(item->column() == 0 && !item->text().isEmpty()) {
        if(!item->text().startsWith("http") && !item->text().startsWith("/")) {
            QRegExp regExp(item->text());
            if(!regExp.isValid()) {
                QMessageBox::warning(this, "Warning", "Invalid Regular Expression!");
                item->setText("");
                return;
            }
        }
        int count = model->rowCount();
        for(int row = 0; row < count; row ++) {
            if(row == item->row()) {
                continue;
            }
            QStandardItem * firstItem = model->item(row, 0);
            if(firstItem->text().isEmpty()) {
                continue;
            }
            if(firstItem->text() == item->text()) {
                QMessageBox::warning(this, "Warning", QString("'") + firstItem->text() + "' Exist !");
                item->setText("");
                return;
            }
        }
    }
}

void SetExtractorsDialog::on_addButton_clicked() {
    int count = model->rowCount();
    QStandardItem* firstItem = new QStandardItem;
    model->setItem(count, 0, firstItem);
    QStandardItem* secondItem = new QStandardItem;
    model->setItem(count, 1, secondItem);
}

void SetExtractorsDialog::on_delButton_clicked() {
    QModelIndexList indexList = ui->tableView->selectionModel()->selectedIndexes();
    QList<int> rowWillDel;
    for(QModelIndex modelIndex: indexList) {
        if(modelIndex.column() == 0) {
            rowWillDel.append(modelIndex.column());
        }
    }
    for(int i = indexList.size() - 1; i >= 0; i --) {
        QModelIndex modelIndex = indexList.at(i);
        if(modelIndex.column() == 1 && rowWillDel.indexOf(modelIndex.row()) == -1) {
            model->setData(modelIndex, QVariant(""));
        } else {
            model->takeRow(modelIndex.row());
        }
    }
}

void SetExtractorsDialog::on_clearButton_clicked() {
    QMessageBox::StandardButton button = QMessageBox::warning(this, "Warning", "Delete All ?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(button == QMessageBox::Yes) {
        int count = model->rowCount();
        for(int row = count - 1; row >= 0; row --) {
            model->takeRow(row);
        }
    }
}

void SetExtractorsDialog::on_saveButton_clicked() {
    QMap<QString, QString> extractorItems;
    int count = model->rowCount();
    for(int row = 0; row < count; row ++) {
        QStandardItem * firstItem = model->item(row, 0);
        if(firstItem->text().isEmpty()) {
            continue;
        }
        extractorItems.insert(firstItem->text(), "");
    }
    emit extractorChanged(extractorItems);
}
