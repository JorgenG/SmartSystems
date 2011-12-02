#include "server.h"

Server::Server(QObject *parent) :
    QTcpServer(parent)
{
}

void Server::incomingConnection(int socketDescriptor)
{
    Connection *connThread = new Connection(socketDescriptor, this);
    connect(connThread, SIGNAL(finished()), connThread, SLOT(deleteLater()));
    connThread->start();
}

