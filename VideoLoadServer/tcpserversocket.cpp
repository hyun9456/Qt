#include "tcpserversocket.h"
#include <QHostAddress>
#include <QAbstractSocket>

TcpServerSocket::TcpServerSocket(QObject *parent)
    : QTcpServer(parent)
{
}

void TcpServerSocket::run(quint16 port)
{
    if(!this->listen(QHostAddress::Any, port)){
        qDebug() << "Could not start server";
    }
    else {
        qDebug() << "Listening to port " << port << "...";
    }
}

void TcpServerSocket::stop()
{
    qDebug() << "Closing to port " "...";
    for(auto thread : m_threadList) {
        thread->stop();
        thread->wait();
    }
    m_threadList.clear();
    this->close();
}

void TcpServerSocket::sendData(QByteArray block)
{
    emit dataReady(block);
}

void TcpServerSocket::readyRead(QByteArray data)
{
    emit readyReadData(data);
}

void TcpServerSocket::incomingConnection(qintptr socketDescriptor)
{
    // We have a new connection
    qDebug() << socketDescriptor << " Connecting...";

    // Every new connection will be run in a newly created thread
    m_threadList.push_back(QSharedPointer<ServerThread>(new ServerThread(socketDescriptor, 1)));

    // connect signal/slot
    // once a thread is not needed, it will be beleted later
    connect(this, &TcpServerSocket::dataReady, m_threadList.last().data(), &ServerThread::setData);
    connect(m_threadList.last().data(), &ServerThread::readyReadData, this, &TcpServerSocket::readyRead);

    m_threadList.last()->start();
}

