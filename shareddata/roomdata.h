#ifndef ROOMDATA_H
#define ROOMDATA_H
#include <QMutex>

/**
  This class contains all data stored in a particular room. Is only
  used by the Shared Data class, and implements mutex locks to avoid
  synchronization issues.
  */
class RoomData
{
public:
    RoomData();
    double getTemperature();
    void setTemperature(double newTemperature);
    double getWantedTemperature();
    void setWantedTemperature(double newWantedTemperature);
    int getBrightness();
    void setBrightness(int newBrightness);
    int getWantedBrightness();
    void setWantedBrightness(int newWantedBrightness);
    int getFanspeed();
    void setFanspeed(int newFanspeed);
    int getLed();
    void setLed(int newLed);
    bool getHeater();
    void setHeater(bool newHeaterValue);
private:
    double *temperature, wantedTemperature;
    int *brightness, wantedBrightness, fanspeed, led, tempCounter, brightnessCounter;
    bool heater;

    QMutex *roomLock;
};

#endif // ROOMDATA_H
