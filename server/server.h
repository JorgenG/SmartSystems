#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>

#include "logger.h"
#include "connection.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(QObject *parent = 0);
protected:
    void incomingConnection(int socketDescriptor);
};

#endif // SERVER_H
