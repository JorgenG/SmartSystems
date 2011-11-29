#include "server.h"

Server::Server(QObject *parent, QTcpServer *theTcpServer, int thePort) :
    QObject(parent)
{
    port = thePort;
    tcpServer = theTcpServer;
}

void Server::listen()
{
    if(!tcpServer->listen(QHostAddress::LocalHost, port)) {
        QString string("Listen on port ");
        string.append(QString::number(tcpServer->serverPort()));
        string.append(" failed.");
        logger->addEntry(string);
    } else {
        QString string("Listen on port ");
        string.append(QString::number(port));
        string.append(" successful.");
        logger->addEntry(string);
    }
    if(tcpServer->isListening())
        logger->addEntry("Is listening");
    else
        logger->addEntry("Is not listening.");
}

void Server::newConnection()
{
    logger->addEntry("Connection started on port ");
    Connection *newConnection = new Connection(tcpServer->nextPendingConnection());
    newConnection->start();
}
