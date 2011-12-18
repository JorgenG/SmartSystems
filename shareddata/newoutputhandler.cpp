#include "newoutputhandler.h"

NewOutputHandler::NewOutputHandler(NIInterface *niInterface, QObject *parent) :
    QObject(parent)
{
    this->niInterface = niInterface;
    automodeThread = new AutomodeThread(niInterface);
    outputLock = new QMutex();
}

void NewOutputHandler::dataChangedInRoom(int roomNumber, int dataType)
{
    outputLock->lock();
    if(roomNumber == 4) {
        newSunspotOutput(dataType);
    } else {
        newNIOutput(roomNumber, dataType);
    }
    outputLock->unlock();
}

void NewOutputHandler::automodeChanged(bool newAutomode)
{
    logger->addEntry("Detected automodechange");
    if(newAutomode) {
        automodeThread->start();
    }
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

/**
 * dataTypes:
 *  - wantedTemp        = 0;
 *  - wantedBrightness  = 1;
 *  - led               = 2;
 *  - fan               = 3;
 *  - heater            = 4;
 */
void NewOutputHandler::newNIOutput(int room, int dataType)
{
    switch(dataType) {
    case 2:
        niInterface->setLedOutputInRoom(room);
        break;
    case 3:
        niInterface->setFanOutput();
        break;
    case 4:
        niInterface->setHeaterOutputInRoom(room);
        break;
    default:
        break;
    }
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
    case 5: // Automode
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
