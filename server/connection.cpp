#include "connection.h"

Connection::Connection(QTcpSocket &newSocketConnection)
{
    socketConnection = &newSocketConnection;
    xmlDocument = new QDomDocument();
}

void Connection::run()
{
    QByteArray receiveData = socketConnection->readAll();
    xmlDocument->setContent(receiveData);

}
