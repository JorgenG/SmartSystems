#ifndef CONNECTION_H
#define CONNECTION_H

#include <QThread>
#include <QTcpSocket>
#include <QDomDocument>

#include "logger.h"

class Connection : public QThread
{
public:
    Connection(QTcpSocket *socketConnection);
    void run();
private:
    QTcpSocket *socketConnection;
    QDomDocument *xmlDocument;
    void webConnection();
    void spotConnection();
};

#endif // CONNECTION_H
