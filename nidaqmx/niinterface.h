#ifndef NIINTERFACE_H
#define NIINTERFACE_H

#include <QObject>
#include <QMutex>
#include "NIDAQmx.h"
#include "logger.h"

#define DAQmxErrChk(functionCall) if(DAQmxFailed(DAQmxError = (functionCall))) handleDAQmxFailed(DAQmxError);


class NIInterface : public QObject
{
    Q_OBJECT
public:
    explicit NIInterface(QObject *parent = 0);


private:
    void initTaskHandles();
    void handleDAQmxFailed(int error);

    int DAQmxError;
    QMutex *niLock;
    TaskHandle* *tempAI;
    TaskHandle* *brightnessAI;
    TaskHandle* *pwmAO;
    TaskHandle* *heaterDO;
    TaskHandle* *controlDO;
};

#endif // NIINTERFACE_H
