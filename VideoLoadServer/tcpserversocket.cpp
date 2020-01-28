#include "tcpserversocket.h"
#include <QHostAddress>
#include <QAbstractSocket>

TcpServerSocket::TcpServerSocket(QObject *parent)
    : QTcpServer(parent)
{
    connect(this, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

void TcpServerSocket::run(quint16 port)
{
    this->listen(QHostAddress::Any, port);
}

void TcpServerSocket::sendData(QByteArray block)
{
    for (QTcpSocket* socket : sockets) {
        socket->write(block);
    }
}
void TcpServerSocket::stop()
{
    sockets.clear();
    this->close();
}

void TcpServerSocket::onNewConnection()
{
    QTcpSocket *clientSocket = this->nextPendingConnection();
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStateChanged(QAbstractSocket::SocketState)));

    sockets.push_back(clientSocket);
    for (QTcpSocket* socket : sockets) {
        socket->write(QByteArray::fromStdString(clientSocket->peerAddress().toString().toStdString() + " connected to server !\n"));
    }
}

void TcpServerSocket::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::UnconnectedState)
    {
        QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
        sockets.removeOne(sender);
    }
}

void TcpServerSocket::onReadyRead()
{
    QTcpSocket* sender(static_cast<QTcpSocket*>(QObject::sender()));
    QByteArray datas = sender->readAll();
    for (QTcpSocket* socket : sockets) {
        if (socket != sender)
            socket->write(QByteArray::fromStdString(sender->peerAddress().toString().toStdString() + ": " + datas.toStdString()));
    }
}
