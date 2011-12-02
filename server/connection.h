#ifndef CONNECTION_H
#define CONNECTION_H

#include <QThread>
#include <QTcpSocket>
#include <QDomDocument>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include "shareddata/shareddata.h"
#include "logger.h"

class Connection
{
public:
    Connection(QTcpSocket *socketConnection);
    void processConnection();
private:
    QTcpSocket *socketConnection;
    QByteArray *receiveData;
    void webConnection();
    void spotConnection();
    QString generateDataValues();
};

#endif // CONNECTION_H
