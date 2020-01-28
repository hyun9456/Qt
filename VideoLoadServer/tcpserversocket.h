#ifndef TCPSERVERSOCKET_H
#define TCPSERVERSOCKET_H

#include <QTcpServer>
#include <QTcpSocket>

class TcpServerSocket : public QTcpServer
{
    Q_OBJECT
public:
    TcpServerSocket(QObject *parent = nullptr);
    void run(quint16 port);
    void sendData(QByteArray block);
    void stop();

private slots:
    void onNewConnection();
    void onReadyRead();
    void onSocketStateChanged(QAbstractSocket::SocketState socketState);

private:
    QList<QSharedPointer<QTcpSocket>> m_sockets;
};

#endif // TCPSERVERSOCKET_H
