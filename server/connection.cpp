#include "connection.h"

Connection::Connection(int socketDescriptor, QObject *parent)
    : QThread(parent), socketDescriptor(socketDescriptor)
{
    socketConnection = new QTcpSocket();
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
    socketConnection->waitForDisconnected();
}

void Connection::webConnection()
{
    QString data(*receiveData);
    QFile webpage("D:/index.html");
    webpage.open(QFile::ReadOnly);
    QByteArray webpageData = webpage.readAll();

    if(data.startsWith("GET /data "))
    {
        QFile httpOk("D:/httpok.txt");
        QByteArray dataToSend = httpOk.readAll();
        QString dataValues = generateDataValues();
        dataToSend.append(dataValues);
        socketConnection->write(dataToSend);
        return;
    } else if(data.startsWith("POST"))
    {
        QString postMessage = data.mid(data.indexOf("room="));
        logger->addEntry(postMessage);
        socketConnection->write(webpageData);
    } else
    {
        socketConnection->write(webpageData);
    }
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
            dataValues.append(sharedData->getHeaterInRoom(i)?"On":"Off");
            dataValues.append("|");
        }
        dataValues.append(QString::number(sharedData->getWantedTempInRoom(i)));
        dataValues.append("|");
        dataValues.append(QString::number(sharedData->getWantedBrightnessInRoom(i)));
        dataValues.append("\n");
    }
    return dataValues;
}

void Connection::run()
{
    socketConnection->setSocketDescriptor(socketDescriptor);
    processConnection();
}
