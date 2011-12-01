#ifndef NEWOUTPUTHANDLER_H
#define NEWOUTPUTHANDLER_H

#include <QObject>
#include "logger.h"
#include "nidaqmx/niinterface.h"
#include <QTcpSocket>
#include <QByteArray>
#include <QDomDocument>
#include "shareddata.h"
#include <QMutex>
#include <QHostAddress>

/**
 * This class handles the tasks needed to be performed
 * when a change has occured in the output data.
 * If the change is in the sunspot, a socket will be
 * initialized and a message with the needed data will
 * be sent.
 */
class NewOutputHandler : public QObject
{
    Q_OBJECT
public:
    explicit NewOutputHandler(NIInterface *niInterface = 0, QObject *parent = 0);
private:
    void newSunspotOutput(int dataType);
    void newNIOuput(int dataType);
    static const int sunspotRoomNumber = 4;

    NIInterface *niInterface;
    QByteArray generateXmlData(int dataType);
    QMutex *outputLock;

signals:

public slots:
    void dataChangedInRoom(int roomNumber, int dataType);
    void automodeChanged(bool newAutomode);
};

#endif // NEWOUTPUTHANDLER_H
