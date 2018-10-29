#include "extractoreditordialog.h"
#include "ui_extractoreditordialog.h"
#include <QDebug>

ExtractorEditorDialog::ExtractorEditorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExtractorEditorDialog)
{
    ui->setupUi(this);
    model = new QStandardItemModel(this);
    connect(model, &QStandardItemModel::itemChanged, this, &ExtractorEditorDialog::onItemChanged);
    model->setColumnCount(2);
    model->setHeaderData(0, Qt::Horizontal, "Extractor");
    model->setHeaderData(1, Qt::Horizontal, "Base64Data");
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->setMinimumWidth(30);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    rightMenu = new QMenu(this);
    viewAction = new QAction("View", this);
    rightMenu->addAction(viewAction);
    connect(ui->tableView, &QTableView::customContextMenuRequested,
            this, &ExtractorEditorDialog::onCustomContextMenuRequested);
    connect(viewAction, &QAction::triggered, this, &ExtractorEditorDialog::onViewActionTriggered);
}

ExtractorEditorDialog::~ExtractorEditorDialog()
{
    delete ui;
}

void ExtractorEditorDialog::onViewActionTriggered() {
    if(base64DataViewDialog == NULL) {
        base64DataViewDialog = new Base64DataViewDialog(this);
    }
    QModelIndexList indexList = ui->tableView->selectionModel()->selectedIndexes();
    if(indexList.size() == 0) {
        return;
    }
    QModelIndex modelIndex = indexList.at(0);
    QStandardItem* item = model->itemFromIndex(modelIndex);
    base64DataViewDialog->setBase64Data(item->text());
    base64DataViewDialog->show();
}

void ExtractorEditorDialog::onCustomContextMenuRequested(const QPoint &pos) {
    QModelIndex modelIndex = ui->tableView->indexAt(pos);
    QStandardItem* item = model->itemFromIndex(modelIndex);
    if(item == NULL || item->column() != 1) {
        return;
    }
    rightMenu->exec(QCursor::pos());
}

void ExtractorEditorDialog::updateModel(const QString &extractor, const QString &base64Data) {
    int count = model->rowCount();
    for(int row = 0; row < count; row ++) {
        QStandardItem * firstItem = model->item(row, 0);
        if(!firstItem->text().isEmpty() && firstItem->text() == extractor) {
            QStandardItem * secondItem = model->item(row, 1);
            secondItem->setText(base64Data);
        }

    }
}

void ExtractorEditorDialog::onItemChanged(QStandardItem * item) {
    if(item->column() == 0 && !item->text().isEmpty()) {
        if(!item->text().startsWith("http") && !item->text().startsWith("/")) {
            QRegExp regExp(item->text());
            if(!regExp.isValid()) {
                QMessageBox::warning(this, "Warning", "Invalid Regular Expression!");
                item->setText("");
                return;
            }
        }
        QStringList extractors;
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
            extractors.append(firstItem->text());
        }
        extractors.append(item->text());
        emit extractorChanged(extractors);
    }
}

void ExtractorEditorDialog::on_addButton_clicked() {
    int count = model->rowCount();
    QStandardItem* firstItem = new QStandardItem;
    model->setItem(count, 0, firstItem);
    QStandardItem* secondItem = new QStandardItem;
    model->setItem(count, 1, secondItem);
}

bool compareModelIndex(const QModelIndex &index1, const QModelIndex &index2) {
    if(index1.row() < index2.row()) {
        return true;
    } else if(index1.row() == index2.row()){
        if(index1.column() < index2.column()) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

void ExtractorEditorDialog::on_delButton_clicked() {
    qDebug() << "---------------------------";
    QModelIndexList indexList = ui->tableView->selectionModel()->selectedIndexes();
    QList<int> rowWillDel;
    for(QModelIndex modelIndex: indexList) {
        qDebug() << QString("row: %1, col: %2").arg(modelIndex.row()).arg(modelIndex.column());
        if(modelIndex.column() == 0) {
            rowWillDel.append(modelIndex.row());
        }
    }
    qDebug() << "---------------------------";
    QModelIndexList itemWillBlank;
    for(QModelIndex modelIndex: indexList) {
        if(modelIndex.column() != 0 && rowWillDel.indexOf(modelIndex.row()) == -1) {
            itemWillBlank.append(modelIndex);
        }
    }
    qSort(itemWillBlank.begin(), itemWillBlank.end(), compareModelIndex);
    for(QModelIndex modelIndex: itemWillBlank) {
        qDebug() << QString("rowWillBlank: %1, col: %2").arg(modelIndex.row()).arg(modelIndex.column());
        model->setData(modelIndex, QVariant(""));
    }
    qDebug() << "---------------------------";
    qSort(rowWillDel);
    for(int i = rowWillDel.size() -1; i >= 0; i --) {
        qDebug() << QString("rowWillDel: %1").arg(i);
        model->takeRow(i);
    }
    qDebug() << "---------------------------";
    QStringList extractors;
    int count = model->rowCount();
    for(int row = 0; row < count; row ++) {
        QStandardItem * firstItem = model->item(row, 0);
        if(firstItem->text().isEmpty()) {
            continue;
        }
        extractors.append(firstItem->text());
    }
    emit extractorChanged(extractors);
}

void ExtractorEditorDialog::on_clearButton_clicked() {
    QMessageBox::StandardButton button = QMessageBox::warning(this, "Warning", "Delete All ?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(button == QMessageBox::Yes) {
        int count = model->rowCount();
        for(int row = count - 1; row >= 0; row --) {
            model->takeRow(row);
        }
        QStringList extractors;
        emit extractorChanged(extractors);
    }
}

void ExtractorEditorDialog::on_okButton_clicked() {
    this->close();
}
