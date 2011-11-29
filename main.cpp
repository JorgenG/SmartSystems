#include <QtGui/QApplication>
#include "mainwindow.h"
#include "shareddata/shareddata.h"
#include "shareddata/newoutputhandler.h"
#include "server/server.h"
#include "logger.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NewOutputHandler *newOutputHandler = new NewOutputHandler();
    QObject::connect(sharedData, SIGNAL(dataChangedInRoom(int,int)),
            newOutputHandler, SLOT(dataChangedInRoom(int,int)));
    QObject::connect(sharedData, SIGNAL(autoModeChanged(bool)),
            newOutputHandler, SLOT(automodeChanged(bool)));

    Server *server = new Server(6999);
    Server *server2 = new Server(5000);

    MainWindow w(0);
    QObject::connect(logger, SIGNAL(logEntryAdded()), &w, SLOT(logEntryAdded()) );
    server->start();
    server2->start();
    w.show();
    QObject::connect(&w, SIGNAL(exitButtonClicked()), qApp, SLOT(quit()));
    return a.exec();
}
