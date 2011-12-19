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
    void setAutomode(bool newAutomode);
    bool getAutomode();
    void setDataFromWeb(bool data);
    bool getDataFromWeb();
private:
    RoomData* *roomData;
    bool automode;
    bool dataFromWeb;
    QMutex *autoModeLock, *dataFromWebLock;

signals:
    /**
     * dataTypes:
     *  - wantedTemp        = 0;
     *  - wantedBrightness  = 1;
     *  - led               = 2;
     *  - fan               = 3;
     *  - heater            = 4;
     */
    void sensorDataChanged();
    void dataChangedInRoom(int roomNumber, int dataType);
    void dataChangedInRoomFromWeb(int roomNumber, int dataType);
    void autoModeChanged(bool newAutomode);
};

extern SharedData *sharedData;

#endif // SHAREDDATA_H
