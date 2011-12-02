#include "connection.h"

Connection::Connection(QTcpSocket *newSocketConnection)
{
    socketConnection = newSocketConnection;
    receiveData = new QByteArray();
}

void Connection::processConnection()
{
    socketConnection->waitForReadyRead();
    *receiveData = socketConnection->readAll();
    if(receiveData->size() > 0) {
        if(socketConnection->localPort() == 5000) {
            webConnection();
        } else if(socketConnection->localPort() == 6999) {
            spotConnection();
        }
    }
    socketConnection->waitForBytesWritten();
    socketConnection->disconnectFromHost();
}

void Connection::webConnection()
{
    QString data(*receiveData);
    if(data.startsWith("GET"))
    {
        QFile httpOk("D:/httpok.txt");
        QFile webpage("D:/index.html");
        if(!webpage.exists())
            logger->addEntry("Could not locate file");
        else {
            if(!(webpage.open(QFile::ReadOnly) && httpOk.open(QFile::ReadOnly)))
                logger->addEntry("Open failed");
            else {
                QByteArray dataToSend = httpOk.readAll();
                QByteArray webpageData = webpage.readAll();

                if(data.startsWith("GET /data")) {
                    QString dataValues = generateDataValues();
                    dataToSend.append(dataValues);
                    socketConnection->write(dataToSend);
                }
                else
                    socketConnection->write(webpageData);
            }
        }
    } else if(data.startsWith("POST"))
    {

    } else
    {

    }
    // Interpret if POST or GET - Send webpage
}

void Connection::spotConnection()
{
    logger->addEntry("Spot connection detected!");

    // Only format received is update of sensor data from Spot.
}

QString Connection::generateDataValues()
{
    QString dataValues;
    double temp[4];
    int brightness[4];
    sharedData->getSensorData(temp, brightness);

    for(int i = 1; i < 5; i++) {
        dataValues.append(QString::number(temp[i - 1]));
        dataValues.append("|");
        dataValues.append(QString::number(brightness[i - 1]));
        dataValues.append("|");
        dataValues.append(QString::number(sharedData->getLedInRoom(i)));
        dataValues.append("|");
        if(i == 2 || i == 4) {
            dataValues.append(QString::number(sharedData->getFanInRoom(i)));
            dataValues.append("|");
        }
        if(i != 1) {
            dataValues.append(sharedData->getHeaterInRoom(i)?"true":"false");
            dataValues.append("|");
        }
        dataValues.append(QString::number(sharedData->getWantedTempInRoom(i)));
        dataValues.append("|");
        dataValues.append(QString::number(sharedData->getWantedBrightnessInRoom(i)));
        dataValues.append("\n");
    }
    return dataValues;
}
