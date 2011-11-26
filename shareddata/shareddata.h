#ifndef SHAREDDATA_H
#define SHAREDDATA_H

class SharedData
{
public:
    SharedData();
    void storeNISensorData(double temperatures[], int brightness[]);
    void storeSpotSensorData(double temperature, int brightness);
    void getSensorData(double tempBuffer[], int brightnessBuffer[]);
    void setWantedTempInRoom(int roomNumber, double wantedTemp);
    void setWantedBrightnessInRoom(int roomNumber, int wantedBrightness);
    void setAutomode(bool newAutomode);
    bool getAutomode();
private:
    RoomData *roomData;
    bool automode;

signals:
    void wantedTempChangedInRoom(int roomNumber, double wantedTemp);
    void wantedBrightnessChangedInRoom(int roomNumber, int wantedBrightness);
    void ledChangedInRoom(int roomNumber, int ledValue);
    void fanspeedChangedInRoom(int roomNumber, int fanspeed);
    void heaterToggledInRoom(int roomNumber);
};

#endif // SHAREDDATA_H
