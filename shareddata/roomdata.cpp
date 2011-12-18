#include "roomdata.h"

RoomData::RoomData()
{
    tempCounter = 0;
    brightnessCounter = 0;
    temperature = new double[10];
    brightness = new int[10];
    for(int i = 0; i < 10; i++) {
        temperature[i] = 0;
        brightness[i] = 0;
    }
    led = 0;
    fanspeed = 0;
    heater = false;
    wantedBrightness = 50;
    wantedTemperature = 22.0;
    roomLock = new QMutex();
}

double RoomData::getTemperature()
{
    roomLock->lock();
    double sum = 0;
    for(int i = 0; i < 10; i++) {
        sum += temperature[i];
    }
    double value = sum/10;
    roomLock->unlock();
    return value;
}

void RoomData::setTemperature(double newTemperature)
{
    roomLock->lock();
    temperature[tempCounter++] = newTemperature;
    if(tempCounter == 10)
        tempCounter = 0;
    roomLock->unlock();
}

int RoomData::getBrightness()
{
    roomLock->lock();
    int sum = 0;
    for(int i = 0; i < 10; i++) {
        sum += brightness[i];
    }
    int value = sum/10;
    roomLock->unlock();
    return value;
}

void RoomData::setBrightness(int newBrightness)
{
    roomLock->lock();
    brightness[brightnessCounter++] = newBrightness;
    if(brightnessCounter == 10)
        brightnessCounter = 0;
    roomLock->unlock();
}

int RoomData::getFanspeed()
{
    roomLock->lock();
    int value = fanspeed;
    roomLock->unlock();
    return value;
}

void RoomData::setFanspeed(int newFanspeed)
{
    roomLock->lock();
    fanspeed = newFanspeed;
    roomLock->unlock();
}

int RoomData::getLed()
{
    roomLock->lock();
    int value = led;
    roomLock->unlock();
    return value;
}

void RoomData::setLed(int newLed)
{
    roomLock->lock();
    led = newLed;
    roomLock->unlock();
}

bool RoomData::getHeater()
{
    roomLock->lock();
    bool value = heater;
    roomLock->unlock();
    return value;
}

void RoomData::setHeater(bool newHeaterValue)
{
    roomLock->lock();
    heater = newHeaterValue;
    roomLock->unlock();
}

double RoomData::getWantedTemperature()
{
    roomLock->lock();
    double value = wantedTemperature;
    roomLock->unlock();
    return value;
}

void RoomData::setWantedTemperature(double newWantedTemperature)
{
    roomLock->lock();
    wantedTemperature = newWantedTemperature;
    roomLock->unlock();
}

int RoomData::getWantedBrightness()
{
    roomLock->lock();
    int value = wantedBrightness;
    roomLock->unlock();
    return value;
}

void RoomData::setWantedBrightness(int newWantedBrightness)
{
    roomLock->lock();
    wantedBrightness = newWantedBrightness;
    roomLock->unlock();
}

