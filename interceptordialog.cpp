#include "interceptordialog.h"
#include "ui_interceptordialog.h"
#include <QDebug>

InterceptorDialog::InterceptorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InterceptorDialog)
{
    ui->setupUi(this);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, [](QListWidgetItem *item){
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    });
}

InterceptorDialog::~InterceptorDialog()
{
    delete ui;
}

void InterceptorDialog::on_addButton_clicked()
{
    ui->listWidget->addItem("NULL");
    int count = ui->listWidget->count();
    qDebug() << count;
    QListWidgetItem* lastItem = ui->listWidget->item(count - 1);
    lastItem->setFlags(lastItem->flags() | Qt::ItemIsEditable);
    lastItem->listWidget()->setEditTriggers(QAbstractItemView::DoubleClicked);
}
