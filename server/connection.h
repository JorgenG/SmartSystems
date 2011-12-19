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

/**
  This class handles a new connection to the server. It detects what
  kind of connection it is, and interprets the message received.
  */
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
    /**
      This method is called when a webconnectino is detected.
      The webconnections use port 5000.
      */
    void webConnection();

    /**
      This method is called when a spotconnection is detected.
      The spotconnections use port 6999.
      */
    void spotConnection();

    /**
      Is called when a post message is coming. Interprets the what data
      is changed and stores this in the shared data.
      */
    void handlePostMessage(QString data);

    /**
      Generates data for the javascript to display as dynamic data.
      */
    QString generateDataValues();

    /**
      Interprets a GET message from a browser. Returns the filename.
      */
    QString interpretFilename();
    QString path;
};

#endif // CONNECTION_H
