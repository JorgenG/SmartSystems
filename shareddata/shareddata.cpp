#include "shareddata.h"

SharedData *sharedData = new SharedData();

SharedData::SharedData(QObject *parent) :
                       QObject(parent)
{
    autoModeLock = new QMutex();
    dataFromWebLock = new QMutex();
    roomData = new RoomData*[4];
    for(int i = 0; i < 4; i++)
    {
        roomData[i] = new RoomData();
    }
    automode = false;
    dataFromWeb = false;
}

void SharedData::storeNISensorData(double temperatures[], int brightness[])
{
    for(int i = 0; i < 3; i++) {
        roomData[i]->setTemperature(temperatures[i]);
        roomData[i]->setBrightness(brightness[i]);
    }
    emit sensorDataChanged();
}

void SharedData::storeSpotSensorData(double temperature, int brightness)
{
    roomData[3]->setTemperature(temperature);
    roomData[3]->setBrightness(brightness);
    emit sensorDataChanged();
}

void SharedData::getSensorData(double tempBuffer[], int brightnessBuffer[])
{
    for(int i = 0; i < 4; i++)
    {
        tempBuffer[i] = roomData[i]->getTemperature();
        brightnessBuffer[i] = roomData[i]->getBrightness();
    }
}

void SharedData::setWantedTempInRoom(int roomNumber, double wantedTemp)
{
    roomData[roomNumber - 1]->setWantedTemperature(wantedTemp);
    emit dataChangedInRoom(roomNumber, 0);
    if(dataFromWeb)
        emit dataChangedInRoomFromWeb(roomNumber, 0);
}

void SharedData::setWantedBrightnessInRoom(int roomNumber, int wantedBrightness)
{
    roomData[roomNumber - 1]->setWantedBrightness(wantedBrightness);
    emit dataChangedInRoom(roomNumber, 1);
    if(dataFromWeb)
        emit dataChangedInRoomFromWeb(roomNumber, 1);
}

void SharedData::setLedInRoom(int roomNumber, int newValue)
{
    roomData[roomNumber - 1]->setLed(newValue);
    emit dataChangedInRoom(roomNumber, 2);
    if(dataFromWeb)
        emit dataChangedInRoomFromWeb(roomNumber, 2);
}

void SharedData::setFanInRoom(int roomNumber, int newValue)
{
    roomData[roomNumber - 1]->setFanspeed(newValue);
    emit dataChangedInRoom(roomNumber, 3);
    if(dataFromWeb)
        emit dataChangedInRoomFromWeb(roomNumber, 3);
}

void SharedData::setHeaterInRoom(int roomNumber, bool newValue)
{
    roomData[roomNumber - 1]->setHeater(newValue);
    emit dataChangedInRoom(roomNumber, 4);
    if(dataFromWeb)
        emit dataChangedInRoomFromWeb(roomNumber, 4);
}

double SharedData::getWantedTempInRoom(int roomNumber)
{
    return roomData[roomNumber - 1]->getWantedTemperature();
}

int SharedData::getWantedBrightnessInRoom(int roomNumber)
{
    return roomData[roomNumber - 1]->getWantedBrightness();
}

int SharedData::getLedInRoom(int roomNumber)
{
    return roomData[roomNumber - 1]->getLed();
}

int SharedData::getFanInRoom(int roomNumber)
{
    return roomData[roomNumber - 1]->getFanspeed();
}

bool SharedData::getHeaterInRoom(int roomNumber)
{
    return roomData[roomNumber - 1]->getHeater();
}

void SharedData::setAutomode(bool newAutomode)
{
    autoModeLock->lock();
    automode = newAutomode;
    autoModeLock->unlock();
    emit autoModeChanged(newAutomode);
    if(dataFromWeb)
        emit dataChangedInRoomFromWeb(0, 0);
}

bool SharedData::getAutomode()
{
    autoModeLock->lock();
    bool returnValue = automode;
    autoModeLock->unlock();
    return returnValue;
}

void SharedData::setDataFromWeb(bool data)
{
    dataFromWebLock->lock();
    dataFromWeb = data;
    dataFromWebLock->unlock();
}
