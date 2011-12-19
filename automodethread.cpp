#include "automodethread.h"

AutomodeThread::AutomodeThread(NIInterface *niInterface, QObject *parent)
    : QThread(parent)
{
    this->niInterface = niInterface;
}

void AutomodeThread::run()
{
    automodeUpdate();
}

void AutomodeThread::automodeUpdate()
{
    while(sharedData->getAutomode()) {
        niInterface->autoModeActivated();
        QThread::msleep(1000);
    }
}
