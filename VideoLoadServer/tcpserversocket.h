#ifndef TCPSERVERSOCKET_H
#define TCPSERVERSOCKET_H

#include <QTcpServer>
#include <QTcpSocket>
#include "serverthread.h"

class TcpServerSocket : public QTcpServer
{
    Q_OBJECT
public:
    TcpServerSocket(QObject *parent = nullptr);
    void run(quint16 port);
    void sendData(QByteArray block);
    void stop();

private slots:

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    QList<QSharedPointer<ServerThread>> m_threadList;
signals:
    void dataReady(QByteArray block);
};

#endif // TCPSERVERSOCKET_H
