#include "server.h"

Server::Server(QObject *parent, QTcpServer *theTcpServer, int thePort) :
    QObject(parent)
{
    port = thePort;
    tcpServer = theTcpServer;
}

void Server::listen()
{
    if(!tcpServer->listen(QHostAddress::Any, port)) {
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
}

void Server::newConnection()
{
    Connection *newConnection = new Connection(tcpServer->nextPendingConnection());
    newConnection->processConnection();
}
