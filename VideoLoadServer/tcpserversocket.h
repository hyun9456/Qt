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

public slots:
    void stop();
    void readyRead(QByteArray data);

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    QList<QSharedPointer<ServerThread>> m_threadList;
signals:
    void dataReady(QByteArray block);
    void readyReadData(QByteArray data);
};

#endif // TCPSERVERSOCKET_H
