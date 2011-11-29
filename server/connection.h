#ifndef CONNECTION_H
#define CONNECTION_H

#include <QThread>
#include <QTcpSocket>
#include <QDomDocument>

class Connection : public QThread
{
public:
    Connection(QTcpSocket &socketConnection);
    void run();
private:
    QTcpSocket *socketConnection;
    QDomDocument *xmlDocument;
};

#endif // CONNECTION_H
