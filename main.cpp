#include <QtGui/QApplication>
#include "mainwindow.h"
#include "shareddata/shareddata.h"
#include "shareddata/newoutputhandler.h"
#include "server/server.h"
#include "logger.h"
#include "nidaqmx/niinterface.h"
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NIInterface *niInterface = new NIInterface();
    NewOutputHandler *newOutputHandler = new NewOutputHandler(niInterface);
    MainWindow w(0);

    Server *webServer = new Server();
    Server *spotServer = new Server();
    QObject::connect(sharedData, SIGNAL(dataChangedInRoom(int,int)),newOutputHandler, SLOT(dataChangedInRoom(int,int)));
    QObject::connect(sharedData, SIGNAL(autoModeChanged(bool)),newOutputHandler, SLOT(automodeChanged(bool)));

    QObject::connect(logger, SIGNAL(logEntryAdded()), &w, SLOT(logEntryAdded()) );


    webServer->listen(QHostAddress::Any, 5000);
    spotServer->listen(QHostAddress::Any, 6999);
    QObject::connect(sharedData, SIGNAL(dataChangedInRoomFromWeb(int,int)), &w, SLOT(dataChangedInRoomFromWeb(int,int)));
    QObject::connect(sharedData, SIGNAL(sensorDataChanged()), &w, SLOT(updateSensorData()));
    QObject::connect(&w, SIGNAL(exitButtonClicked()), qApp, SLOT(quit()));
    w.show();

    QTimer *sensUpdTimer = new QTimer();
    QObject::connect(sensUpdTimer, SIGNAL(timeout()), niInterface, SLOT(sensorUpdate()));
    sensUpdTimer->start(200);

    return a.exec();
}
