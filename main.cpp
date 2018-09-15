#include <QApplication>
#include <QDebug>
#include <QStandardPaths>
#include "terminal.h"
#include "encoding.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qDebug() << "hello headless!";
    QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    qDebug() << "path: " << path;
    Terminal* terminal = Terminal::instance();
    terminal->setEncoding(Encoding::UTF8.getName());
    terminal->cout("hello Terminal", true);
    return app.exec();
}
