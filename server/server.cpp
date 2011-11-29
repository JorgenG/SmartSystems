#include "server.h"

Server::Server(int aport)
{
    port = aport;
    tcpServer = new QTcpServer();
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

void Server::run()
{
    if(!tcpServer->listen(QHostAddress::LocalHost, port)) {
        QString string("Listen on port ");
        string.append(port);
        string.append(" failed.");
        logger->addEntry(string);
    } else {
        QString string("Listen on port ");
        string.append(port);
        string.append(" successful.");
        logger->addEntry(string);
    }
}

void Server::newConnection()
{
    //Connection *newConnection = new Connection(tcpServer->nextPendingConnection());
}
