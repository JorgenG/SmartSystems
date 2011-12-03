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

class Connection : public QThread
{
    Q_OBJECT
public:
    Connection(int socketDescriptor, QObject *parent = 0);
    void run();
    void processConnection();
private:
    int socketDescriptor;
    QTcpSocket *socketConnection;
    QString *receiveData;
    void webConnection();
    void spotConnection();
    QString generateDataValues();
    QString interpretFilename();
    QString path;
};

#endif // CONNECTION_H
