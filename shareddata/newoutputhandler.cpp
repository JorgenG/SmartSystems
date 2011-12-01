#include "newoutputhandler.h"

NewOutputHandler::NewOutputHandler(NIInterface *niInterface, QObject *parent) :
    QObject(parent)
{
    this->niInterface = niInterface;
    outputLock = new QMutex();
}

void NewOutputHandler::dataChangedInRoom(int roomNumber, int dataType)
{
    QString debugMessage("Detected data change in room: ");
    debugMessage.append(QString::number(roomNumber));
    debugMessage.append(" with datatype: ");
    debugMessage.append(QString::number(dataType));
    logger->addEntry(debugMessage);

    outputLock->lock();
    if(roomNumber == 4) {
        newSunspotOutput(dataType);
    } else {
        newNIOuput(dataType);
    }
    outputLock->unlock();
}

void NewOutputHandler::automodeChanged(bool newAutomode)
{
    if(newAutomode)
        logger->addEntry("Detected automode change to true");
    else
        logger->addEntry("Detected automode change to false");

    newSunspotOutput(5);
}

void NewOutputHandler::newSunspotOutput(int dataType)
{
    QTcpSocket *socketConnection = new QTcpSocket();
    QByteArray data = generateXmlData(dataType);
    socketConnection->connectToHost(QHostAddress::LocalHost, 6998);
    socketConnection->write(data);
    socketConnection->close();
}

void NewOutputHandler::newNIOuput(int dataType)
{

}

QByteArray NewOutputHandler::generateXmlData(int dataType)
{
    QDomDocument *xmlDocument = new QDomDocument();
    QDomElement messageElement = xmlDocument->createElement("message");
    QDomElement dataElement = xmlDocument->createElement("data");
    dataElement.setAttribute("type", dataType);
    QDomText *value = new QDomText();

    switch(dataType) {
    case 0: // wantedTemp
        *value = xmlDocument->createTextNode(QString::number(sharedData->getWantedTempInRoom(sunspotRoomNumber)));
        break;
    case 1: // wantedBrightness
        *value = xmlDocument->createTextNode(QString::number(sharedData->getWantedBrightnessInRoom(sunspotRoomNumber)));
        break;
    case 2: // LED Pwm
        *value = xmlDocument->createTextNode(QString::number(sharedData->getLedInRoom(sunspotRoomNumber)));
        break;
    case 3: // Fan PWM
        *value = xmlDocument->createTextNode(QString::number(sharedData->getFanInRoom(sunspotRoomNumber)));
        break;
    case 4: // Heater bool
        *value = xmlDocument->createTextNode(QString(sharedData->getHeaterInRoom(sunspotRoomNumber)?"true":"false"));
        break;
    case 5: // Automode changed
        *value = xmlDocument->createTextNode(QString(sharedData->getAutomode()?"true":"false"));
        break;
    default:
        *value = xmlDocument->createTextNode("unknowntype");
        break;
    }
    dataElement.appendChild(*value);
    messageElement.appendChild(dataElement);
    xmlDocument->appendChild(messageElement);
    logger->addEntry(xmlDocument->toString());
    return xmlDocument->toByteArray();
}
