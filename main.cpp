#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QCDEStyle>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QObject::connect(&w, SIGNAL(exitButtonClicked()), qApp, SLOT(quit()));

    return a.exec();
}
