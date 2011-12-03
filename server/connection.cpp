#include "connection.h"

Connection::Connection(int socketDescriptor, QObject *parent)
    : QThread(parent), socketDescriptor(socketDescriptor)
{
    path = "D:/Workspace/SmartSystems/http";
    socketConnection = new QTcpSocket();
}

void Connection::processConnection()
{
    socketConnection->waitForReadyRead();
    receiveData = new QString(socketConnection->readAll());
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


    if(receiveData->startsWith("GET /data "))
    {
        QByteArray dataToSend = "HTTP/1.1 200 OK\r\n\r\n";
        QString dataValues = generateDataValues();
        dataToSend.append(dataValues);
        socketConnection->write(dataToSend);
        return;
    } else if(receiveData->startsWith("POST"))
    {
        QFile webpage(path + "/index.html");
        webpage.open(QFile::ReadOnly);
        QByteArray webpageData;
        webpageData = webpage.readAll();
        webpage.close();

        socketConnection->write("HTTP/1.1 200 OK\r\n\r\n", 19);
        socketConnection->write(webpageData);
        QString postMessage = receiveData->mid(receiveData->indexOf("room="));
        logger->addEntry(postMessage);
        // Interpret data
    } else if(receiveData->startsWith("GET"))
    {
        QString filename = interpretFilename();
        logger->addEntry(filename);
        QFile file(path + filename);
        if(!file.open(QFile::ReadOnly)) {
            file.setFileName(path + "/index.html");
            file.open(QFile::ReadOnly);
        }
        QByteArray fileData = file.readAll();
        file.close();

        socketConnection->write("HTTP/1.1 200 OK\r\n\r\n", 19);
        socketConnection->write(fileData);
    } else
    {
        logger->addEntry("Unknown request.");
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

QString Connection::interpretFilename()
{
    if(receiveData->startsWith("GET ")) {
        int stopIndex = receiveData->indexOf(" ", 4);
        return receiveData->mid(4, stopIndex - 4);
    }
    else
        return "/index.html";
}
