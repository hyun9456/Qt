#ifndef TCPSERVERSOCKET_H
#define TCPSERVERSOCKET_H

#include <QTcpServer>
#include <QTcpSocket>

class TcpServerSocket
{
public:
    TcpServerSocket();
private slots:
    void newConnection();
    void socketStateChanged(QAbstractSocket::SocketState socketState);
    void readyData();
    void sendData(int data);
private:
    QTcpServer  server;
    QList<QSharedPointer<QTcpSocket>>  sockets;
};

#endif // TCPSERVERSOCKET_H
