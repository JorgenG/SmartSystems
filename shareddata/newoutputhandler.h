#ifndef NEWOUTPUTHANDLER_H
#define NEWOUTPUTHANDLER_H

#include <QObject>

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
    explicit NewOutputHandler(QObject *parent = 0);
private:
    void newSunspotOutput();
    void newNIOuput();

signals:

public slots:
    void dataChangedInRoom(int roomNumber, int dataType);
    void automodeChanged(bool newAutomode);
};

#endif // NEWOUTPUTHANDLER_H
