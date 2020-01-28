#include "tcpserversocket.h"
#include <QHostAddress>
#include <QAbstractSocket>

TcpServerSocket::TcpServerSocket(QObject *parent)
    : QTcpServer(parent)
{
    connect(this, &TcpServerSocket::newConnection, this, &TcpServerSocket::onNewConnection);
}

void TcpServerSocket::run(quint16 port)
{
    this->listen(QHostAddress::Any, port);
}

void TcpServerSocket::sendData(QByteArray block)
{
    for (auto socket : m_sockets) {
        socket->write(block);
    }
}
void TcpServerSocket::stop()
{
    m_sockets.clear();
    this->close();
}

void TcpServerSocket::onNewConnection()
{
    QSharedPointer<QTcpSocket> clientSocket(this->nextPendingConnection());
    connect(clientSocket.data(), &QTcpSocket::readyRead, this, &TcpServerSocket::onReadyRead);
    connect(clientSocket.data(), &QTcpSocket::stateChanged, this, &TcpServerSocket::onSocketStateChanged);

    m_sockets.push_back(clientSocket);
    for (auto socket : m_sockets) {
        socket->write(QByteArray::fromStdString(clientSocket->peerAddress().toString().toStdString() + " connected to server !\n"));
    }
}

void TcpServerSocket::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::UnconnectedState)
    {
        QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
        m_sockets.removeOne(static_cast<QSharedPointer<QTcpSocket>>(sender));
    }
}

void TcpServerSocket::onReadyRead()
{
    QTcpSocket* sender(static_cast<QTcpSocket*>(QObject::sender()));
    QByteArray datas = sender->readAll();
    for (auto socket : m_sockets) {
        if (socket != sender)
            socket->write(QByteArray::fromStdString(sender->peerAddress().toString().toStdString() + ": " + datas.toStdString()));
    }
}
