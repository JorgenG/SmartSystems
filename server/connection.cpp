#include "connection.h"

Connection::Connection(QTcpSocket *newSocketConnection)
{
    socketConnection = newSocketConnection;
    xmlDocument = new QDomDocument();
}

void Connection::run()
{
    if(socketConnection->localPort() == 5000) {
        webConnection();
    } else if(socketConnection->localPort() == 6999) {
        spotConnection();
    }
}

void Connection::webConnection()
{
    logger->addEntry("Web connection detected!");
}

void Connection::spotConnection()
{
    logger->addEntry("Spot connection detected!");
}
