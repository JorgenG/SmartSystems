#include "newoutputhandler.h"

NewOutputHandler::NewOutputHandler(QObject *parent) :
    QObject(parent)
{
}

void NewOutputHandler::dataChangedInRoom(int roomNumber, int dataType)
{
}

void NewOutputHandler::automodeChanged(bool newAutomode)
{
}
