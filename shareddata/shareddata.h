#ifndef SHAREDDATA_H
#define SHAREDDATA_H

#include <QMutex>
#include <QObject>

#include "roomdata.h"

class SharedData : public QObject
{
    Q_OBJECT
public:
    explicit SharedData(QObject *parent = 0);
    void storeNISensorData(double temperatures[], int brightness[]);
    void storeSpotSensorData(double temperature, int brightness);
    void getSensorData(double tempBuffer[], int brightnessBuffer[]);
    void setWantedTempInRoom(int roomNumber, double wantedTemp);
    void setWantedBrightnessInRoom(int roomNumber, int wantedBrightness);
    void setLedInRoom(int roomNumber, int newValue);
    void setFanInRoom(int roomNumber, int newValue);
    void setHeaterInRoom(int roomNumber, bool newValue);
    double getWantedTempInRoom(int roomNumber);
    int getWantedBrightnessInRoom(int roomNumber);
    int getLedInRoom(int roomNumber);
    int getFanInRoom(int roomNumber);
    bool getHeaterInRoom(int roomNumber);
    static const int WANTEDTEMP = 0, WANTEDBRIGHTNESS = 1, LEDPWM = 2, FANPWM = 3, HEATER = 4;
    void setAutomode(bool newAutomode);
    bool getAutomode();
private:
    RoomData* *roomData;
    bool automode;
    QMutex *autoModeLock;

signals:
    /**
     * dataTypes:
     *  - wantedTemp        = 0;
     *  - wantedBrightness  = 1;
     *  - led               = 2;
     *  - fan               = 3;
     *  - heater            = 4;
     */
    void dataChangedInRoom(int roomNumber, int dataType);
    void autoModeChanged(bool newAutomode);
};

extern SharedData *sharedData;

#endif // SHAREDDATA_H
