#include <QApplication>
#include <QDebug>
#include <QCommandLineParser>
#include <iostream>
#include <QNetworkProxy>
//#include "superbrowser.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationVersion("0.0.1");
    app.setApplicationName("SuperBrowser");
    //    Terminal* terminal = Terminal::instance();
    //    terminal->setEncoding(Encoding::UTF8.getName());
    //    terminal->cout("Hello Terminal", true);
    //    QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    //    terminal->cout("path: ", false);
    //    terminal->cout(path, true);
    //    SuperBrowser browser(&app);
#ifdef QT_NOT_DEBUG
    QCommandLineParser commandLineParser;
    commandLineParser.addHelpOption();
    commandLineParser.addVersionOption();
    commandLineParser.addPositionalArgument(QStringLiteral("url"),
                                            QStringLiteral("The url to be loaded in the browser window."));
    commandLineParser.addPositionalArgument(QStringLiteral("port"),
                                            QStringLiteral("The port to be listened in the tcp server."));
    int port = 0;
    if(commandLineParser.isSet(QStringLiteral("port"))) {
        port = commandLineParser.value("port").toInt();
    } else {
        std::cout << "argument 'port' not set." << std::endl;
        app.exit(-1);
        return -1;

    }
    QUrl url("about:blank");
    if(commandLineParser.isSet(QStringLiteral("url"))) {
        QString urlStr = commandLineParser.value(QStringLiteral("url"));
        url = QUrl::fromUserInput(urlStr);
    }
    MainWindow window;
    if(port > 0) {
        if(!window.startTcpServer(port)) {
            std::cout << "could not listen on " << port << std::endl;
            app.exit(-1);
            return -1;
        }
    }
    window.show();
#else
    MainWindow window;
    window.show();
#endif
    return app.exec();
}
