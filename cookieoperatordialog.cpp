#include "cookieoperatordialog.h"
#include "ui_cookieoperatordialog.h"
#include <QDebug>

CookieOperatorDialog::CookieOperatorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CookieOperatorDialog)
{
    ui->setupUi(this);
    model = new QStandardItemModel(this);
    connect(model, &QStandardItemModel::itemChanged, this, &CookieOperatorDialog::onItemChanged);
    model->setColumnCount(7);
    model->setHeaderData(0, Qt::Horizontal, "Name");
    model->setHeaderData(1, Qt::Horizontal, "Value");
    model->setHeaderData(2, Qt::Horizontal, "Domain");
    model->setHeaderData(3, Qt::Horizontal, "Path");
    model->setHeaderData(4, Qt::Horizontal, "Expires");
    model->setHeaderData(5, Qt::Horizontal, "HttpOnly");
    model->setHeaderData(6, Qt::Horizontal, "Secure");
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->setMinimumWidth(30);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
}

CookieOperatorDialog::~CookieOperatorDialog()
{
    delete ui;
}

void CookieOperatorDialog::onItemChanged(QStandardItem *item) {
    QString text = item->text();
    if(item->column() == 0 && text.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Name Is Empty!");
        return;
    }
    if(item->column() == 2 && text.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Domain Is Empty!");
        return;
    }
    QList<QNetworkCookie> cookieList;
    int count = model->rowCount();
    for(int row = 0; row < count; row ++) {
        QString name = model->item(row, 0)->text();
        QString value = model->item(row, 1)->text();
        QNetworkCookie cookie(name.toUtf8(), value.toUtf8());
        QString domain = model->item(row, 2)->text();
        QString path = model->item(row, 3)->text();
        if(path.isEmpty()) {
            path = "/";
        }
        QString expirationDateText = model->item(row, 4)->text();
        QDateTime dateTime = QDateTime::fromString(expirationDateText, "yyyy-MM-dd HH:mm:ss");
        if(dateTime.isValid() && dateTime > QDateTime::currentDateTime()) {
            QDateTime currentDateTime = QDateTime::currentDateTime();
            currentDateTime.addDays(7);
            cookie.setExpirationDate(currentDateTime);
        }
        bool httpOnly = model->item(row, 5)->text() == "Yes" ? true : false;
        bool secure = model->item(row, 6)->text() == "Yes" ? true : false;
        cookie.setDomain(domain);
        cookie.setPath(path);

        cookie.setHttpOnly(httpOnly);
        cookie.setSecure(secure);
        cookieList.append(cookie);
    }
    emit cookieOperatorEdited(cookieList);
}

void CookieOperatorDialog::updateModel(const QList<QNetworkCookie> &cookieList) {
    disconnect(model, &QStandardItemModel::itemChanged, this, &CookieOperatorDialog::onItemChanged);
    int count = model->rowCount();
    if(count > 0) {
        for(int row = count - 1; row >= 0; row --) {
            model->removeRow(row);
        }
    }
    count = 0;
    for(QNetworkCookie cookie: cookieList) {
        QStandardItem* nameItem = new QStandardItem;
        nameItem->setText(QString(cookie.name()));
        model->setItem(count, 0, nameItem);
        QStandardItem* valueItem = new QStandardItem;
        valueItem->setText(QString(cookie.value()));
        model->setItem(count, 1, valueItem);
        QStandardItem* domainItem = new QStandardItem;
        domainItem->setText(cookie.domain());
        model->setItem(count, 2, domainItem);
        QStandardItem* pathItem = new QStandardItem;
        pathItem->setText(cookie.path());
        model->setItem(count, 3, pathItem);
        QStandardItem* expiresItem = new QStandardItem;
        if(cookie.expirationDate().isValid()) {
            expiresItem->setText(cookie.expirationDate().toString("yyyy-MM-dd HH:mm:ss"));
        } else {
            expiresItem->setText(QDateTime::fromMSecsSinceEpoch(-1).toUTC().toString("yyyy-MM-dd HH:mm:ss"));
        }
        model->setItem(count, 4, expiresItem);
        QStandardItem* httpOnlyItem = new QStandardItem;
        httpOnlyItem->setText(cookie.isHttpOnly() ? "Yes" : "No");
        model->setItem(count, 5, httpOnlyItem);
        QStandardItem* secureItem = new QStandardItem;
        secureItem->setText(cookie.isSecure() ? "Yes" : "No");
        model->setItem(count, 6, secureItem);
        count ++;
    }
    connect(model, &QStandardItemModel::itemChanged, this, &CookieOperatorDialog::onItemChanged);
}

void CookieOperatorDialog::on_addButton_clicked() {
    int count = model->rowCount();
    QStandardItem* nameItem = new QStandardItem;
    model->setItem(count, 0, nameItem);
    QStandardItem* valueItem = new QStandardItem;
    model->setItem(count, 1, valueItem);
    QStandardItem* domainItem = new QStandardItem;
    model->setItem(count, 2, domainItem);
    QStandardItem* pathItem = new QStandardItem;
    model->setItem(count, 3, pathItem);
    QStandardItem* expiresItem = new QStandardItem;
    model->setItem(count, 4, expiresItem);
    QStandardItem* httpOnlyItem = new QStandardItem;
    model->setItem(count, 5, httpOnlyItem);
    QStandardItem* secureItem = new QStandardItem;
    model->setItem(count, 6, secureItem);
}

void CookieOperatorDialog::on_delButton_clicked() {
    QModelIndexList indexList = ui->tableView->selectionModel()->selectedIndexes();
    QSet<int> rowWillDel;
    for(QModelIndex modelIndex: indexList) {
        rowWillDel.insert(modelIndex.row());
    }
    QList<int> rows = rowWillDel.toList();
    qSort(rows);
    for(int i = rows.size() - 1; i >= 0; i --) {
        model->removeRow(rows.at(i));
    }
}

void CookieOperatorDialog::on_clearButton_clicked() {
    int count = model->rowCount();
    if(count == 0) {
        return;
    }
    QMessageBox::StandardButton button = QMessageBox::warning(this, "Warning", "Delete All ?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(button == QMessageBox::Yes) {
        for(int row = count - 1; row >= 0; row --) {
            model->removeRow(row);
        }
    }
}

void CookieOperatorDialog::on_okButton_clicked() {
    this->close();
}

void CookieOperatorDialog::on_refreshButton_clicked() {
    emit refreshButtonClicked();
}
