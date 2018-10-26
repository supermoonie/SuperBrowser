#include "interceptordialog.h"
#include "ui_interceptordialog.h"

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

QList<QString> InterceptorDialog::getInterceptors() {
    QList<QString> list;
    int count = ui->listWidget->count();
    for(int i = 0; i < count; i ++) {
        QListWidgetItem* item = ui->listWidget->item(i);
        QString text = item->text();
        if(text == "NULL" || text.isEmpty()) {
            continue;
        }
        list.append(text);
    }
    return list;
}

void InterceptorDialog::setInterceptors(const QList<QString> &interceptors) {
    for(int i = 0; i < interceptors.size(); i ++) {
        QString interceptor = interceptors[i];
        ui->listWidget->addItem(interceptor);
    }
}

void InterceptorDialog::on_addButton_clicked() {
    ui->listWidget->addItem("NULL");
    ui->listWidget->setCurrentRow(ui->listWidget->count() - 1);
}

void InterceptorDialog::on_removeButton_clicked() {
    ui->listWidget->takeItem(ui->listWidget->currentRow());
}

void InterceptorDialog::on_clearButton_clicked() {
    ui->listWidget->clear();
}

void InterceptorDialog::on_okButton_clicked() {
    this->close();
    emit accepted();
}
