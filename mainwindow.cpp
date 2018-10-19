#include "mainwindow.h"
#include <QToolBar>
#include "webpage.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    progress = 0;
    view = new QWebView(this);
    view->setPage(WebPage::instance(this));

    locationEdit = new QLineEdit(this);
    locationEdit->setSizePolicy(QSizePolicy::Expanding, locationEdit->sizePolicy().verticalPolicy());
    connect(locationEdit, &QLineEdit::returnPressed, this, &MainWindow::changeLocation);
    QToolBar *toolBar = addToolBar(tr("Navigation"));
    toolBar->addAction(view->pageAction(QWebPage::Back));
    toolBar->addAction(view->pageAction(QWebPage::Forward));
    toolBar->addAction(view->pageAction(QWebPage::Reload));
    toolBar->addAction(view->pageAction(QWebPage::Stop));
    toolBar->addWidget(locationEdit);
    setCentralWidget(view);
    setUnifiedTitleAndToolBarOnMac(true);
}

MainWindow::~MainWindow()
{

}

void MainWindow::changeLocation() {
    QUrl url = QUrl::fromUserInput(locationEdit->text());
    view->setUrl(url);
    view->setFocus();
}

