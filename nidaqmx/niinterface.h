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
    void setHeaterOutputInRoom(int room);
    void setFanOutput();
    void setLedOutputInRoom(int room);
    void autoModeActivated();

private:
    void setPwmControlToRoom(int room);

    /**
     * Update PWM will set the update bit high and the low again.
     */
    void updatePwm();
    void initTaskHandles();
    void handleDAQmxFailed(int error);
    double convertADCValueToTemperature(double adcValue);
    double static const A = 0.00372307;
    double static const B = -0.000245477;
    double static const C = 0.00000162287;

    int DAQmxError;
    QMutex *niLock;
    TaskHandle* *tempAI;
    TaskHandle* *brightnessAI;
    TaskHandle *pwmAO;
    TaskHandle* *heaterDO;
    TaskHandle* *controlDO;
};

#endif // NIINTERFACE_H
