#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>

#include "logger.h"
#include "connection.h"

/**
  This class inherits from the QTcpServer. To actually start the server
  you need to instantiate this object and call listen. Then the incomingConnection
  method will be called each time a new connections is initiated.
  */
class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(QObject *parent = 0);
protected:
    /**
      This method handles an incoming connection. It creates a new connection
      thread and starts it. Allowing for a huge amount of connections to be
      made at the same time.
      */
    void incomingConnection(int socketDescriptor);
};

#endif // SERVER_H
