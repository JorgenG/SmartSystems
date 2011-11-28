#include "roomdata.h"

RoomData::RoomData()
{
    temperature = 20.0;
    brightness = 50;
    led = 50;
    fanspeed = 0;
    heater = false;
    roomLock = new QMutex();
}

double RoomData::getTemperature()
{
    roomLock->lock();
    double value = temperature;
    roomLock->unlock();
    return value;
}

void RoomData::setTemperature(double newTemperature)
{
    roomLock->lock();
    temperature = newTemperature;
    roomLock->unlock();
}

int RoomData::getBrightness()
{
    roomLock->lock();
    int value = brightness;
    roomLock->unlock();
    return value;
}

void RoomData::setBrightness(int newBrightness)
{
    roomLock->lock();
    brightness = newBrightness;
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

