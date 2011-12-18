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
    niInterface->autoModeActivated();
    if(sharedData->getAutomode())
        QTimer::singleShot(1000, this, SLOT(automodeUpdate()));
}
