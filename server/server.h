#ifndef SERVER_H
#define SERVER_H

#include <QThread>
#include <QObject>
#include <QTcpServer>

#include "logger.h"
#include "connection.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0, QTcpServer *theTcpServer = 0, int port = 0);
    void listen();
private:
    QTcpServer *tcpServer;
    int port;
public slots:
    void newConnection();
};

#endif // SERVER_H
