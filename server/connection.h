#ifndef CONNECTION_H
#define CONNECTION_H

#include <QThread>
#include <QTcpSocket>

class Connection : public QThread
{
public:
    Connection(QTcpSocket &socketConnection);
    void run();
private:
    QTcpSocket *socketConnection;
};

#endif // CONNECTION_H
