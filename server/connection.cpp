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
    delete socketConnection;
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

        socketConnection->write("HTTP/1.1 303 See Other\r\nLocation: index.html\r\n\r\n");
        socketConnection->write(webpageData);
        QString postMessage = receiveData->mid(receiveData->indexOf("room="));
        handlePostMessage(postMessage);
        // Interpret data
    } else if(receiveData->startsWith("GET /background.jpg"))
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

        socketConnection->write("HTTP/1.1 200 OK\r\nCache-Control: cache\r\nExpires: Wed, 12 Dec 2012 12:12:12 GMT\r\n\r\n");
        socketConnection->write(fileData);

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

        socketConnection->write("HTTP/1.1 200 OK\r\n\r\n");
        socketConnection->write(fileData);
    } else
    {
        logger->addEntry("Unknown request.");
    }
}

void Connection::spotConnection()
{
    logger->addEntry("Spot connection detected!");
    QDomDocument *xmlDocument = new QDomDocument();
    xmlDocument->setContent(receiveData);
    double temperature;
    bool tempConvertOk = false;
    bool brightnessConvertOk = false;
    int brightness;
    if(xmlDocument->childNodes().length() == 2) {
        temperature = xmlDocument->childNodes().at(0).childNodes().item(0).toText().data().toDouble(tempConvertOk);
        brightness = xmlDocument->childNodes().at(1).childNodes().item(0).toText().data().toDouble(brightnessConvertOk);
        if(brightnessConvertOk && tempConvertOk) {
            sharedData->storeSpotSensorData(temperature, brightness);
        }
    } else {
        logger->addEntry("Invalid xml document. Number of childs: " + QString::number(xmlDocument->childNodes().length()));
    }

    delete xmlDocument;
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
    dataValues.append(sharedData->getAutomode()?"On":"Off");
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

void Connection::handlePostMessage(QString data)
{
    int pivot = 0;
    int endPivot = 0;
    pivot = data.indexOf("=");
    bool convertOk;
    int roomNumber = data.mid(pivot + 1, 1).toInt(&convertOk);

    logger->addEntry(data);

    sharedData->setDataFromWeb(true);
    if(convertOk) {
        bool automode = sharedData->getAutomode();
        switch(roomNumber) {
        case 0:
            pivot = data.indexOf("=", pivot + 1);
            endPivot = data.indexOf("&", pivot);
            if(data.mid(pivot + 1).startsWith("on"))
                sharedData->setAutomode(true);
            else
                sharedData->setAutomode(false);
            break;
        default:
            pivot = data.indexOf("=", pivot + 1);
            endPivot = data.indexOf("&", pivot);
            int led = data.mid(pivot + 1, endPivot - (pivot + 1)).toInt(&convertOk);
            if(convertOk && led <= 100 && led >= 0 && !automode)
                sharedData->setLedInRoom(roomNumber, led);

            if(roomNumber == 2 || roomNumber == 4) {
                pivot = data.indexOf("=", pivot + 1);
                endPivot = data.indexOf("&", pivot);
                int fan = data.mid(pivot + 1, endPivot - (pivot + 1)).toInt(&convertOk);
                if(convertOk && fan <= 100 && fan >= 0 && !automode)
                    sharedData->setFanInRoom(roomNumber, fan);
            }

            if(roomNumber == 2 || roomNumber == 3 || roomNumber == 4) {
                if(data.indexOf("heater", pivot) == -1) {
                    if(!automode)
                        sharedData->setHeaterInRoom(roomNumber, false);
                } else {
                    pivot = data.indexOf("=", pivot + 1);
                    endPivot = data.indexOf("&", pivot);
                    if(!automode)
                        sharedData->setHeaterInRoom(roomNumber, true);
                }
            }

            pivot = data.indexOf("=", pivot + 1);
            endPivot = data.indexOf("&", pivot);
            double wtemp = data.mid(pivot + 1, endPivot - (pivot + 1)).toDouble(&convertOk);
            if(convertOk && wtemp < 35.0 && wtemp > 15.0)
                sharedData->setWantedTempInRoom(roomNumber, wtemp);

            pivot = data.indexOf("=", pivot + 1);
            endPivot = data.indexOf("&", pivot);
            int wbrightness = data.mid(pivot + 1, endPivot - (pivot + 1)).toInt(&convertOk);
            if(convertOk && wbrightness <= 100 && wbrightness >= 0)
                sharedData->setWantedBrightnessInRoom(roomNumber, wbrightness);
            break;
        }
    } else {
        logger->addEntry("Room number could not be interpreted");
    }
    sharedData->setDataFromWeb(false);
}
