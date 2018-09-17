#include <QApplication>
#include <QDebug>
#include <QStandardPaths>
#include "terminal.h"
#include "encoding.h"
#include "webpage.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Terminal* terminal = Terminal::instance();
    terminal->setEncoding(Encoding::UTF8.getName());
    terminal->cout("Hello Terminal", true);
    QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    terminal->cout("path: ", false);
    terminal->cout(path, true);
    WebPage webPage;
    return app.exec();
}
