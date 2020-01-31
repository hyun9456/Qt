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

    this->close();
}

void TcpServerSocket::sendData(QByteArray block)
{
    emit dataReady(block);
}

void TcpServerSocket::incomingConnection(qintptr socketDescriptor)
{
    // We have a new connection
    qDebug() << socketDescriptor << " Connecting...";

    // Every new connection will be run in a newly created thread
    ServerThread *thread = new ServerThread(socketDescriptor, this, 1);

    // connect signal/slot
    // once a thread is not needed, it will be beleted later
    connect(this, &TcpServerSocket::dataReady, thread, &ServerThread::setData);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    thread->start();

    m_threadList.push_back(QSharedPointer<ServerThread>(thread));
}

