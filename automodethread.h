#ifndef AUTOMODETHREAD_H
#define AUTOMODETHREAD_H

#include <QThread>
#include "shareddata/shareddata.h"
#include "nidaqmx/niinterface.h"
#include <QTimer>

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
