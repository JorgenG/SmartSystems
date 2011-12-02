#include <QtGui/QApplication>
#include "mainwindow.h"
#include "shareddata/shareddata.h"
#include "shareddata/newoutputhandler.h"
#include "server/server.h"
#include "logger.h"
#include "nidaqmx/niinterface.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NIInterface *niInterface = new NIInterface();
    NewOutputHandler *newOutputHandler = new NewOutputHandler(niInterface);
    MainWindow w(0);
    QTcpServer *tcpServer = new QTcpServer();
    QTcpServer *tcpServer2 = new QTcpServer();
    Server *server = new Server(0, tcpServer, 6999);
    Server *server2 = new Server(0, tcpServer2, 5000);
    QObject::connect(sharedData, SIGNAL(dataChangedInRoom(int,int)),newOutputHandler, SLOT(dataChangedInRoom(int,int)));
    QObject::connect(sharedData, SIGNAL(autoModeChanged(bool)),newOutputHandler, SLOT(automodeChanged(bool)));
    QObject::connect(logger, SIGNAL(logEntryAdded()), &w, SLOT(logEntryAdded()) );
    QObject::connect(tcpServer, SIGNAL(newConnection()), server, SLOT(newConnection()));
    QObject::connect(tcpServer2, SIGNAL(newConnection()), server2, SLOT(newConnection()));
    server->listen();
    server2->listen();
    w.show();
    QObject::connect(&w, SIGNAL(exitButtonClicked()), qApp, SLOT(quit()));
    return a.exec();
}
