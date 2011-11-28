#include "server.h"

Server::Server()
{
    tcpServer = new QTcpServer();

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

void Server::run()
{
    if(!tcpServer->listen(QHostAddress::LocalHost, 6999))
    {

    }
}
