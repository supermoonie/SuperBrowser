#include <QApplication>
#include <QDebug>
#include <QStandardPaths>
#include <QWebFrame>
#include "terminal.h"
#include "encoding.h"
#include "superbrowser.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Terminal* terminal = Terminal::instance();
    terminal->setEncoding(Encoding::UTF8.getName());
    terminal->cout("Hello Terminal", true);
    QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    terminal->cout("path: ", false);
    terminal->cout(path, true);
//    SuperBrowser browser(&app);
    MainWindow w;
    w.show();
    return app.exec();
}
