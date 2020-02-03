#include "serverthread.h"

ServerThread::ServerThread(qintptr ID, unsigned long FPS)
    : m_socketDescriptor(ID),
      m_FPS(FPS)
{
    moveToThread(this);
}

void ServerThread::run()
{
    // thread starts here
    qDebug() << " Thread started" << this->currentThreadId();

    m_threadTimer.reset(new QTimer()); //타이머 생성
    connect(m_threadTimer.data(), &QTimer::timeout, this, &ServerThread::timerHit);
    m_block = "";

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
    //connect(this, &ServerThread::stopRequested, this, &ServerThread::disconnected);

    // We'll have multiple clients, we want to know which is which
    qDebug() << m_socketDescriptor << " Client connected";

    m_threadTimer->start(1000/m_FPS);
    exec();
    m_threadTimer->stop();
}

void ServerThread::stop()
{
    if(currentThread() != this)
    {
        QMetaObject::invokeMethod(this, "stop", Qt::QueuedConnection);
    }
    else
    {
        quit();
    }
}

void ServerThread::readyRead()
{
    // get the information
    QByteArray Data = m_socket->readAll();

    // will write on server side window
    qDebug() << m_socketDescriptor << " Data in: " << Data;
    emit readyReadData(Data);
}

void ServerThread::disconnected()
{
    if(nullptr != m_socket) {
        qDebug() << m_socketDescriptor << " Disconnected";

        stop();
    }
}

void ServerThread::timerHit()
{
    QMutexLocker locker(&m_mutex);
    m_socket->write(m_block);
}
void ServerThread::setData(QByteArray block)
{
    QMutexLocker locker(&m_mutex);
    m_block = block;
}
