#ifndef NIINTERFACE_H
#define NIINTERFACE_H

#include <QObject>
#include <QMutex>
#include "NIDAQmx.h"
#include "logger.h"
#include <math.h>
#include "shareddata/shareddata.h"

#define DAQmxErrChk(functionCall) if(DAQmxFailed(DAQmxError = (functionCall))) handleDAQmxFailed(DAQmxError);


class NIInterface : public QObject
{
    Q_OBJECT
public:
    explicit NIInterface(QObject *parent = 0);    
    void updateSensorData();

private:
    void initTaskHandles();
    void handleDAQmxFailed(int error);
    double convertADCValueToTemperature(double adcValue);
    double const A = 0.00372307;
    double const B = -0.000245477;
    double const C = 0.00000162287;

    int DAQmxError;
    QMutex *niLock;
    TaskHandle* *tempAI;
    TaskHandle* *brightnessAI;
    TaskHandle* *pwmAO;
    TaskHandle* *heaterDO;
    TaskHandle* *controlDO;
};

#endif // NIINTERFACE_H
