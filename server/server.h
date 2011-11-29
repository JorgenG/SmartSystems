#ifndef SERVER_H
#define SERVER_H

#include <QThread>
#include <QTcpServer>

#include "logger.h"
#include "connection.h"

class Server : public QThread
{
public:
    Server(int port);
    void run();
private:
    QTcpServer *tcpServer;
    int port;
public slots:
    void newConnection();
};

#endif // SERVER_H
