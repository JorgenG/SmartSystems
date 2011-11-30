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
    pwmAO = new TaskHandle*[3];
    heaterDO = new TaskHandle*[2];
    controlDO = new TaskHandle*[3];
    QString test("test");
    test.toStdString().c_str();

    for(int i = 0; i < 3; i++) {
        QString tempAIStr("TempAI");
        QString brightnessAIStr("BrightnessAI");
        QString pwmAOStr("PwmAO");
        tempAIStr.append(QString::number(i));
        brightnessAIStr.append(QString::number(i));
        pwmAOStr.append(QString::number(i));

        tempAI[i] = new TaskHandle();
        brightnessAI[i] = new TaskHandle();
        pwmAO[i] = new TaskHandle();

        DAQmxErrChk(DAQmxCreateTask(tempAIStr.toStdString().c_str(), tempAI[i]));
        DAQmxErrChk(DAQmxCreateTask(brightnessAIStr.toStdString().c_str(), brightnessAI[i]));
    }
}

void NIInterface::handleDAQmxFailed(int error)
{
    QString logEntry("DAQmxError: ");
    char errorBuffer[512];
    DAQmxGetErrorString(error, errorBuffer, 512);
    logEntry.append(QString::fromLocal8Bit(errorBuffer));
}
