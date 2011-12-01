#include "niinterface.h"

NIInterface::NIInterface(QObject *parent) :
    QObject(parent)
{
    niLock = new QMutex();
    initTaskHandles();

}

void NIInterface::initTaskHandles()
{
    tempAI = new TaskHandle*[3];
    brightnessAI = new TaskHandle*[3];
    pwmAO = new TaskHandle();
    heaterDO = new TaskHandle*[2];
    controlDO = new TaskHandle*[3];
    pwmAO = new TaskHandle();
    DAQmxErrChk(DAQmxCreateTask("PwmAO", pwmAO));

    for(int i = 0; i < 3; i++) {
        QString tempAIStr("TempAI");
        QString brightnessAIStr("BrightnessAI");
        QString heaterDOStr("HeaterDO");
        QString controlDOStr("ControlDO");
        QString number = QString::number(i);
        tempAIStr.append(number);
        brightnessAIStr.append(number);
        heaterDOStr.append(number);
        controlDOStr.append(number);

        tempAI[i] = new TaskHandle();
        brightnessAI[i] = new TaskHandle();        
        heaterDO[i] = new TaskHandle();
        controlDO[i] = new TaskHandle();

        DAQmxErrChk(DAQmxCreateTask(tempAIStr.toStdString().c_str(), tempAI[i]));
        DAQmxErrChk(DAQmxCreateTask(brightnessAIStr.toStdString().c_str(), brightnessAI[i]));

        DAQmxErrChk(DAQmxCreateTask(heaterDOStr.toStdString().c_str(), heaterDO[i]));
        DAQmxErrChk(DAQmxCreateTask(controlDOStr.toStdString().c_str(), controlDO[i]));

        // Create different channels blabla
    }


}

void NIInterface::handleDAQmxFailed(int error)
{
    QString logEntry("DAQmxError: ");
    char errorBuffer[512];
    DAQmxGetErrorString(error, errorBuffer, 512);
    logEntry.append(QString::fromLocal8Bit(errorBuffer));
    logger->addEntry(logEntry);
}

double NIInterface::convertADCValueToTemperature(double adcValue)
{
    double resistance = ((10000.0 * adcValue) / (5 - adcValue));
    double temperature = (1 / (A + B * log(resistance) + C * (pow(log(resistance), 3)))) - 273.15;
    return temperature;
}

void NIInterface::updateSensorData()
{
    double *rawTemperatures = new double[3];
    double *rawBrightness = new double[3];
    double *temperatures = new double[3];
    int *brightness = new int[3];
    niLock->lock();
    for(int i = 0; i < 3; i++) {
        DAQmxErrChk(DAQmxReadAnalogScalarF64(tempAI[i], 0, &rawTemperatures[i], 0));
        DAQmxErrChk(DAQmxReadAnalogScalarF64(brightnessAI[i], 0, &rawBrightness[i], 0));
        temperatures[i] = convertADCValueToTemperature(rawTemperatures[i]);
        // brightness[i] = convertADCValueToLinearBrightness(rawBrightness[i]);
    }
    niLock->unlock();
    sharedData->storeNISensorData(temperatures, brightness);
    logger->addEntry("Sensor data stored!");
}

void NIInterface::autoModeActivated()
{
    while(sharedData->getAutomode()) {
        // If current wanted values are close enough, exit loop, notify updater.
        // Read all wanted values from shareddata
        // Read sensor values and change outputs accordingly
        // Try to increment all outputs.
        // Update sensor data

    }
}

void NIInterface::setHeaterOutputInRoom(int room)
{
    switch(room) {
    case 2:

        break;
    case 3:

        break;
    default:
        // Invalid room, do nothing?!
        break;
    }
}


