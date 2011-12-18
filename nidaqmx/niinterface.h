#ifndef NIINTERFACE_H
#define NIINTERFACE_H

#include <QObject>
#include <QMutex>
#include "NIDAQmx.h"
#include "logger.h"
#include <math.h>
#include <windows.h>
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

public slots:
    void checkAutomodeThreshold();
    void sensorUpdate();


private:
    double static const A = -0.00300955;
    double static const B = 0.000655085;
    double static const C = -0.000000750788;
    double static const tempThreshold = 1;
    int static const brightnessThreshold = 5;

    uInt8 *oneValue;
    uInt8 *zeroValue;

    QString currentOperation;
    int DAQmxError;
    QMutex *niLock;
    TaskHandle *tempAI;
    TaskHandle *brightnessAI;
    TaskHandle pwmAO;
    TaskHandle *heaterDO;
    TaskHandle *controlDO;


    void setPwmControlToRoom(int room);

    /**
     * Update PWM will set the update bit high and the low again.
     */
    void updatePwm();

    /**
     * Sets the control pins for the NI.
     * The value specifies which state to change it to.
     * 0 - 2 represents LED in room 1 - 3.
     * 3 represents the FAN in room 2
     */
    void setControlPins(int value);
    void initTaskHandles();
    void handleDAQmxFailed(int error);
    void createAIChannels();
    void createDOChannels();
    double convertADCValueToTemperature(double adcValue);
    int convertADCValueToLinearBrightness(double adcValue);



};

#endif // NIINTERFACE_H
