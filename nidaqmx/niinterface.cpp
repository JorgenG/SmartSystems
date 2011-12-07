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
    heaterDO = new TaskHandle*[2];


    // [0 - 1] = value for which PWM to control, 00 = LEDR1, 01 = LEDR2, 10 = LEDR3, 11 = FANR2
    // [2]     = pin to be set high when pwm should be updated.
    controlDO = new TaskHandle*[3];

    pwmAO = new TaskHandle();
    DAQmxErrChk(DAQmxCreateTask("PwmAO", pwmAO));
    DAQmxErrChk(DAQmxCreateAOVoltageChan(pwmAO, "Dev1/ao0", "PwmAO", 0, 5, DAQmx_Val_Volts, 0));

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
        if(i < 2)
            DAQmxErrChk(DAQmxCreateTask(heaterDOStr.toStdString().c_str(), heaterDO[i]));
        DAQmxErrChk(DAQmxCreateTask(controlDOStr.toStdString().c_str(), controlDO[i]));

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
        // Read sensor values, compare and then change outputs accordingly
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

void NIInterface::updatePwm()
{
    uInt8 *oneArray = new uInt8[1];
    oneArray[0] = 1;
    DAQmxErrChk(DAQmxWriteDigitalLines(controlDO[2], 1, 1, 0, DAQmx_Val_GroupByChannel, oneArray, 0, 0));

    uInt8 *zeroArray = new uInt8[1];
    zeroArray[0] = 0;
    DAQmxErrChk(DAQmxWriteDigitalLines(controlDO[2], 1, 1, 0, DAQmx_Val_GroupByChannel, zeroArray, 0, 0));
}

void NIInterface::createAIChannels()
{
    DAQmxErrChk(DAQmxCreateAIVoltageChan(tempAI[0], "Dev1/ai0", "TempAI0", DAQmx_Val_Diff, 3, 5, DAQmx_Val_Volts, 0));
    DAQmxErrChk(DAQmxCreateAIVoltageChan(tempAI[1], "Dev1/ai1", "TempAI1", DAQmx_Val_Diff, 3, 5, DAQmx_Val_Volts, 0));
    DAQmxErrChk(DAQmxCreateAIVoltageChan(tempAI[2], "Dev1/ai2", "TempAI2", DAQmx_Val_Diff, 3, 5, DAQmx_Val_Volts, 0));
    DAQmxErrChk(DAQmxCreateAICurrentChan(brightnessAI[0], "Dev1/ai3", "BrightnessAI0", DAQmx_Val_Diff, 0.000001, 0.01, DAQmx_Val_Amps, DAQmx_Val_Internal, 0, 0));
    DAQmxErrChk(DAQmxCreateAICurrentChan(brightnessAI[1], "Dev1/ai4", "BrightnessAI1", DAQmx_Val_Diff, 0.000001, 0.01, DAQmx_Val_Amps, DAQmx_Val_Internal, 0, 0));
    DAQmxErrChk(DAQmxCreateAICurrentChan(brightnessAI[2], "Dev1/ai5", "BrightnessAI2", DAQmx_Val_Diff, 0.000001, 0.01, DAQmx_Val_Amps, DAQmx_Val_Internal, 0, 0));
}

void NIInterface::createDOChannels()
{
    DAQmxErrChk(DAQmxCreateDOChan(heaterDO[0], "Dev1/port0/line0", "HeaterDO0", DAQmx_Val_ChanPerLine));
    DAQmxErrChk(DAQmxCreateDOChan(heaterDO[1], "Dev1/port0/line1", "HeaterDO1", DAQmx_Val_ChanPerLine));
    DAQmxErrChk(DAQmxCreateDOChan(controlDO[0], "Dev1/port0/line2", "ControlDO0", DAQmx_Val_ChanPerLine));
    DAQmxErrChk(DAQmxCreateDOChan(controlDO[1], "Dev1/port0/line3", "ControlDO1", DAQmx_Val_ChanPerLine));
    DAQmxErrChk(DAQmxCreateDOChan(controlDO[2], "Dev1/port0/line4", "ControlDO2", DAQmx_Val_ChanPerLine));
}


