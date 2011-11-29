#ifndef SERVER_H
#define SERVER_H

#include <QThread>
#include <QTcpServer>

#include "Logger.h"

class Server : public QThread
{
public:
    Server();
    void run();
private:
    QTcpServer *tcpServer;

public slots:
    void newConnection();
};

#endif // SERVER_H
