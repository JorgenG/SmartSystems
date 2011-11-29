#include <QtGui/QApplication>
#include "mainwindow.h"
#include "shareddata/shareddata.h"
#include "shareddata/newoutputhandler.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NewOutputHandler *newOutputHandler = new NewOutputHandler();
    QObject::connect(sharedData, SIGNAL(dataChangedInRoom(int,int)),
            newOutputHandler, SLOT(dataChangedInRoom(int,int)));
    QObject::connect(sharedData, SIGNAL(autoModeChanged(bool)),
            newOutputHandler, SLOT(automodeChanged(bool)));

    MainWindow w;
    w.show();
    QObject::connect(&w, SIGNAL(exitButtonClicked()), qApp, SLOT(quit()));
    // EN kommentar
    return a.exec();
}
