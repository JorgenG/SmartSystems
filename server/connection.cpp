#include "connection.h"

Connection::Connection(QTcpSocket *newSocketConnection)
{
    socketConnection = newSocketConnection;
    xmlDocument = new QDomDocument();
}

void Connection::run()
{
    logger->addEntry("Connection started on port " + QString::number(socketConnection->localPort()));
    if(socketConnection->localPort() == 80) {
        webConnection();
    } else if(socketConnection->localPort() == 6999) {
        spotConnection();
    }
    // QByteArray receiveData = socketConnection->readAll();

    // xmlDocument->setContent(receiveData);

}

void Connection::webConnection()
{
    logger->addEntry("Web connection detected!");
}

void Connection::spotConnection()
{
    logger->addEntry("Spot connection detected!");
}
