#include "tcpserversocket.h"

TcpServerSocket::TcpServerSocket()
{

}

void TcpServerSocket::newConnection()
{/*
    QSharedPointer<QTcpSocket> clientSocket(server.nextPendingConnection());
    connect(clientSocket.data(), SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStateChanged(QAbstractSocket::SocketState)));

    sockets.push_back(clientSocket);
    for (QSharedPointer<QTcpSocket> socket : sockets) {
        socket->write(QByteArray::fromStdString(clientSocket->peerAddress().toString().toStdString() + " connected to server !\n"));
    }*/
}
