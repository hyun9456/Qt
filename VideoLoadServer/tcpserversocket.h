#ifndef TCPSERVERSOCKET_H
#define TCPSERVERSOCKET_H

#include <QTcpServer>
#include <QTcpSocket>

class TcpServerSocket
{
public:
    TcpServerSocket();
    void run();
    void sendData(int data);
    void stop();
    QString readyData();
private slots:
    void newConnection();
    void socketStateChanged(QAbstractSocket::SocketState socketState);
private:
    QTcpServer  server;
    QList<QSharedPointer<QTcpSocket>>  sockets;
};

#endif // TCPSERVERSOCKET_H
