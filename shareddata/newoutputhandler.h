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
#include "automodethread.h"

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
    /**
      If new sunspot is detected it will call this method. This method
      then generates a new xml message to be sent to the sunspot.
      */
    void newSunspotOutput(int dataType);

    /**
      If new NI output is detected this method will call the appropriate
      functions in the niInterface objects to change the output.
      */
    void newNIOutput(int room, int dataType);
    static const int sunspotRoomNumber = 4;

    NIInterface *niInterface;

    /**
      Generates xml data for sending to the sunspot application.
      */
    QByteArray generateXmlData(int dataType);
    QMutex *outputLock;
    AutomodeThread *automodeThread;

signals:

public slots:
    void dataChangedInRoom(int roomNumber, int dataType);
    void automodeChanged(bool newAutomode);
};

#endif // NEWOUTPUTHANDLER_H
