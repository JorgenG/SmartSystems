#include "niinterface.h"

NIInterface::NIInterface(QObject *parent) :
    QObject(parent)
{
    niLock = new QMutex();
    oneValue = new uInt8[8];
    zeroValue = new uInt8[8];

    for(int i = 0; i < 8; i++) {
        oneValue[i] = 1;
        zeroValue[i] = 0;
    }

    initTaskHandles();
    createAIChannels();
    createDOChannels();
}

void NIInterface::initTaskHandles()
{
    tempAI = new TaskHandle[3];
    brightnessAI = new TaskHandle[3];
    heaterDO = new TaskHandle[2];
    // [0 - 1] = value for which PWM to control, 00 = LEDR1, 01 = LEDR2, 10 = LEDR3, 11 = FANR2
    // [2]     = pin to be set high when pwm should be updated.
    controlDO = new TaskHandle[3];

    pwmAO = new TaskHandle();
    currentOperation = "CreateTaskPWMAO";
    DAQmxErrChk(DAQmxCreateTask("PwmAO", &pwmAO));
    currentOperation = "CreateVoltChanPWMAO";
    DAQmxErrChk(DAQmxCreateAOVoltageChan(pwmAO, "Dev1/ao0", "PwmAO", 0, 5, DAQmx_Val_Volts, 0));

    for(int i = 0; i < 3; i++) {
        QString number = QString::number(i);

        tempAI[i] = new TaskHandle();
        brightnessAI[i] = new TaskHandle();
        if(i < 2)
            heaterDO[i] = new TaskHandle();
        controlDO[i] = new TaskHandle();

        currentOperation = "CreateTaskTempAI" + number;
        DAQmxErrChk(DAQmxCreateTask("", &tempAI[i]));
        currentOperation = "CreateTaskBrightnessAI" + number;
        DAQmxErrChk(DAQmxCreateTask("", &brightnessAI[i]));
        if(i < 2) {
            currentOperation = "CreateTaskHeaterDO" + number;
            DAQmxErrChk(DAQmxCreateTask("", &heaterDO[i]));
        }
        currentOperation = "CreateTaskControlDO" + number;
        DAQmxErrChk(DAQmxCreateTask("", &controlDO[i]));
    }
}

void NIInterface::handleDAQmxFailed(int error)
{
    QString logEntry("DAQmxError: " + currentOperation + " failed - ");
    char errorBuffer[512];
    DAQmxGetErrorString(error, errorBuffer, 512);
    logEntry.append(QString::fromLocal8Bit(errorBuffer));
    //logger->addEntry(logEntry);
}

double NIInterface::convertADCValueToTemperature(double adcValue)
{
    double resistance = (10000.0 * (5.16 - adcValue)) / adcValue;
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
    currentOperation = "UpdateSensorData";
    for(int i = 0; i < 3; i++) {
        DAQmxErrChk(DAQmxReadAnalogScalarF64(tempAI[i], 0, &rawTemperatures[i], 0));
        DAQmxErrChk(DAQmxReadAnalogScalarF64(brightnessAI[i], 0, &rawBrightness[i], 0));

        temperatures[i] = convertADCValueToTemperature(rawTemperatures[i]);
        brightness[i] = convertADCValueToLinearBrightness(rawBrightness[i]);
    }
    niLock->unlock();
    sharedData->storeNISensorData(temperatures, brightness);
    delete rawTemperatures;
    delete rawBrightness;
    delete temperatures;
    delete brightness;
}

void NIInterface::autoModeActivated()
{
    double *temperatures = new double[4];
    int *brightness = new int[4];
    bool brightnessOk = true;
    int tries = 0;

    while(sharedData->getAutomode()) {
        brightnessOk = true;
        updateSensorData();
        sharedData->getSensorData(temperatures, brightness);

        for(int i = 1; i < 4; i++) {
            int brightnessDiff = sharedData->getWantedBrightnessInRoom(i) - brightness[i - 1];
            if( brightnessDiff > brightnessThreshold && sharedData->getLedInRoom(i) <= 100) {
                sharedData->setLedInRoom(i, brightness[i - 1] + 5);
                brightnessOk = false; // If any of the brightness tests fail, will be set to false
            } else if(brightnessDiff < -brightnessThreshold && sharedData->getLedInRoom(i) >= 0) {
                sharedData->setLedInRoom(i, brightness[i - 1] - 5);
                brightnessOk = false;
            }
        }
        tries++;
        if(brightnessOk || tries > 10)
            break;
    }

    delete temperatures;
    delete brightness;
}

void NIInterface::setHeaterOutputInRoom(int room)
{
    currentOperation = "SetHeaterOutputInRoom";
    switch(room) {
    case 2:        
        if(sharedData->getHeaterInRoom(room)) {
            DAQmxErrChk(DAQmxWriteDigitalLines(heaterDO[0], 1, 1, 2.0, DAQmx_Val_GroupByChannel, oneValue, 0, 0));
        } else {
            DAQmxErrChk(DAQmxWriteDigitalLines(heaterDO[0], 1, 1, 2.0, DAQmx_Val_GroupByChannel, zeroValue, 0, 0));
        }
        break;
    case 3:
        if(sharedData->getHeaterInRoom(room)) {
            DAQmxErrChk(DAQmxWriteDigitalLines(heaterDO[1], 1, 1, 2.0, DAQmx_Val_GroupByChannel, oneValue, 0, 0));
        } else {
            DAQmxErrChk(DAQmxWriteDigitalLines(heaterDO[1], 1, 1, 2.0, DAQmx_Val_GroupByChannel, zeroValue, 0, 0));
        }
        break;
    default:
        // Invalid room, do nothing?!
        break;
    }
}

void NIInterface::updatePwm()
{
    currentOperation = "UpdatePWM";
    DAQmxErrChk(DAQmxWriteDigitalLines(controlDO[2], 1, 1, 2.0, DAQmx_Val_GroupByChannel, oneValue, 0, 0));
    DAQmxErrChk(DAQmxWriteDigitalLines(controlDO[2], 1, 1, 2.0, DAQmx_Val_GroupByChannel, zeroValue, 0, 0));
}

void NIInterface::createAIChannels()
{
    currentOperation = "CreateVoltChanTempAI0";
    DAQmxErrChk(DAQmxCreateAIVoltageChan(tempAI[0], "Dev1/ai0", "TempAI0", DAQmx_Val_RSE, 0, 5, DAQmx_Val_Volts , 0));
    DAQmxErrChk(DAQmxSetSampTimingType(tempAI[0], DAQmx_Val_OnDemand));

    currentOperation = "CreateVoltChanTempAI1";
    DAQmxErrChk(DAQmxCreateAIVoltageChan(tempAI[1], "Dev1/ai1", "TempAI1", DAQmx_Val_RSE, 0, 5, DAQmx_Val_Volts , 0));
    DAQmxErrChk(DAQmxSetSampTimingType(tempAI[1], DAQmx_Val_OnDemand));
    currentOperation = "CreateVoltChanTempAI2";
    DAQmxErrChk(DAQmxCreateAIVoltageChan(tempAI[2], "Dev1/ai2", "TempAI2", DAQmx_Val_RSE, 0, 5, DAQmx_Val_Volts , 0));
    DAQmxErrChk(DAQmxSetSampTimingType(tempAI[2], DAQmx_Val_OnDemand));

    currentOperation = "CreateCurrentChanBrightnssAI0";
    DAQmxErrChk(DAQmxCreateAIVoltageChan(brightnessAI[0], "Dev1/ai3", "BrightnessAI0", DAQmx_Val_RSE, 0, 5, DAQmx_Val_Volts, 0));
    DAQmxErrChk(DAQmxSetSampTimingType(brightnessAI[0], DAQmx_Val_OnDemand));
    currentOperation = "CreateCurrentChanBrightnssAI1";
    DAQmxErrChk(DAQmxCreateAIVoltageChan(brightnessAI[1], "Dev1/ai4", "BrightnessAI1", DAQmx_Val_RSE, 0, 5, DAQmx_Val_Volts, 0));
    DAQmxErrChk(DAQmxSetSampTimingType(brightnessAI[1], DAQmx_Val_OnDemand));
    currentOperation = "CreateCurrentChanBrightnssAI2";
    DAQmxErrChk(DAQmxCreateAIVoltageChan(brightnessAI[2], "Dev1/ai5", "BrightnessAI2", DAQmx_Val_RSE, 0, 5, DAQmx_Val_Volts, 0));
    DAQmxErrChk(DAQmxSetSampTimingType(brightnessAI[2], DAQmx_Val_OnDemand));
}

void NIInterface::createDOChannels()
{
    currentOperation = "CreateDOHeater0";
    DAQmxErrChk(DAQmxCreateDOChan(heaterDO[0], "Dev1/port0/line0", "HeaterDO0", DAQmx_Val_ChanPerLine));
    currentOperation = "CreateDOHeater1";
    DAQmxErrChk(DAQmxCreateDOChan(heaterDO[1], "Dev1/port0/line1", "HeaterDO1", DAQmx_Val_ChanPerLine));
    currentOperation = "CreateDOContro0";
    DAQmxErrChk(DAQmxCreateDOChan(controlDO[0], "Dev1/port1/line0", "ControlDO0", DAQmx_Val_ChanPerLine));
    currentOperation = "CreateDOContro1";
    DAQmxErrChk(DAQmxCreateDOChan(controlDO[1], "Dev1/port1/line1", "ControlDO1", DAQmx_Val_ChanPerLine));
    currentOperation = "CreateDOContro2";
    DAQmxErrChk(DAQmxCreateDOChan(controlDO[2], "Dev1/port1/line2", "ControlDO2", DAQmx_Val_ChanPerLine));
}

void NIInterface::setLedOutputInRoom(int room)
{
    double aoVoltage = ((double) sharedData->getLedInRoom(room) * 5.0 ) / 100.0;
    logger->addEntry("Voltage: " + QString::number(aoVoltage));
    currentOperation = "WriteLedPwmAO";
    DAQmxErrChk(DAQmxWriteAnalogScalarF64(pwmAO, 1, 0, aoVoltage, 0));

    setControlPins(room - 1);
    updatePwm();
}

void NIInterface::setControlPins(int value)
{
    currentOperation = "SetControlPins";
    switch(value) {
    case 0:
        DAQmxErrChk(DAQmxWriteDigitalLines(controlDO[0], 1, 1, 2.0, DAQmx_Val_GroupByChannel, zeroValue, 0, 0));
        DAQmxErrChk(DAQmxWriteDigitalLines(controlDO[1], 1, 1, 2.0, DAQmx_Val_GroupByChannel, zeroValue, 0, 0));
        break;
    case 1:
        DAQmxErrChk(DAQmxWriteDigitalLines(controlDO[0], 1, 1, 0, DAQmx_Val_GroupByChannel, zeroValue, 0, 0));
        DAQmxErrChk(DAQmxWriteDigitalLines(controlDO[1], 1, 1, 0, DAQmx_Val_GroupByChannel, oneValue, 0, 0));
        break;
    case 2:
        DAQmxErrChk(DAQmxWriteDigitalLines(controlDO[0], 1, 1, 0, DAQmx_Val_GroupByChannel, oneValue, 0, 0));
        DAQmxErrChk(DAQmxWriteDigitalLines(controlDO[1], 1, 1, 0, DAQmx_Val_GroupByChannel, zeroValue, 0, 0));
        break;
    case 3:
        DAQmxErrChk(DAQmxWriteDigitalLines(controlDO[0], 1, 1, 0, DAQmx_Val_GroupByChannel, oneValue, 0, 0));
        DAQmxErrChk(DAQmxWriteDigitalLines(controlDO[1], 1, 1, 0, DAQmx_Val_GroupByChannel, oneValue, 0, 0));
        break;
    }
}

void NIInterface::setFanOutput()
{
    double aoVoltage = ( sharedData->getFanInRoom(2) * 5 ) / 100;
    currentOperation = "WriteFanPwmAO";
    DAQmxErrChk(DAQmxWriteAnalogScalarF64(pwmAO, 1, 0, aoVoltage, 0));
    setControlPins(3);
    updatePwm();
}

int NIInterface::convertADCValueToLinearBrightness(double adcValue)
{
    double percentage = ((adcValue - 2.2) * 100.0 / 1.9);
    if(percentage > 100)
        return 100;
    else if(percentage < 0)
        return 0;
    else
        return (int) percentage;
}

void NIInterface::checkAutomodeThreshold()
{
    autoModeActivated();
}

void NIInterface::sensorUpdate()
{
    updateSensorData();
}
