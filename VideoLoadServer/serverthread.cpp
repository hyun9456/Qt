#include "serverthread.h"

ServerThread::ServerThread(qintptr ID, QObject *parent, unsigned long FPS)
    : QThread(parent),
      m_socketDescriptor(ID),
      m_FPS(FPS)
{
    m_threadTimer.reset(new QTimer()); //타이머 생성
    connect(m_threadTimer.data(), &QTimer::timeout, this, &ServerThread::timerHit, Qt::DirectConnection);
    m_threadTimer->moveToThread(this);
    m_block = "";
}

void ServerThread::run()
{
    // thread starts here
    qDebug() << " Thread started";

    m_socket.reset(new QTcpSocket());

    // set the ID
    if(!m_socket->setSocketDescriptor(m_socketDescriptor))
    {
        // something's wrong, we just emit a signal
        emit error(m_socket->error());
        return;
    }

    // connect socket and signal
    // note - Qt::DirectConnection is used because it's multithreaded
    //        This makes the slot to be invoked immediately, when the signal is emitted.

    connect(m_socket.data(), &QTcpSocket::readyRead, this, &ServerThread::readyRead, Qt::DirectConnection);
    connect(m_socket.data(), &QTcpSocket::disconnected, this, &ServerThread::disconnected);

    // We'll have multiple clients, we want to know which is which
    qDebug() << m_socketDescriptor << " Client connected";

//    connect(m_threadTimer, &QTimer::timeout, this, &ServerThread::timerHit, Qt::DirectConnection);
//    timer.setInterval(1000/m_FPS);
//    timer.moveToThread(this);
//    timer.start();
//    exec();
//    timer.stop();
    m_threadTimer->start(1000/m_FPS);
    exec();
    m_threadTimer->stop();
}


void ServerThread::readyRead()
{
    // get the information
    QByteArray Data = m_socket->readAll();

    // will write on server side window
    qDebug() << m_socketDescriptor << " Data in: " << Data;

    //m_socket->write(Data);
}

void ServerThread::disconnected()
{
    qDebug() << m_socketDescriptor << " Disconnected";

    m_socket->deleteLater();

    exit(0);
}

void ServerThread::timerHit()
{
    m_socket->write(m_block);
}
void ServerThread::setData(QByteArray block)
{
    m_block = block;
}
