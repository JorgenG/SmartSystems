#ifndef AUTOMODETHREAD_H
#define AUTOMODETHREAD_H

#include <QThread>
#include "shareddata/shareddata.h"
#include "nidaqmx/niinterface.h"
#include <QTimer>

/**
  This class is responsible for updating the output of the NI device
  when in regulated mode. The thread will stop executing when the
  automode boolean in shareddata is set to false.
  */
class AutomodeThread : public QThread
{
    Q_OBJECT
public:
    AutomodeThread(NIInterface *niInterface, QObject *parent = 0);
    void run();

private:
    NIInterface *niInterface;

private slots:
    void automodeUpdate();
};

#endif // AUTOMODETHREAD_H
