#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QTimer>
#include <QMutex>

class ServerThread : public QThread
{
    Q_OBJECT
public:
    ServerThread(qintptr ID, unsigned long FPS = 60);

signals:
    void error(QTcpSocket::SocketError socketerror);
    void readyReadData(QByteArray Data);

protected:
    void run();

public slots:
    void stop();
    void readyRead();
    void disconnected();
    void timerHit();
    void setData(QByteArray block);

private:
    QSharedPointer<QTcpSocket> m_socket;
    qintptr m_socketDescriptor;
    unsigned long m_FPS;
    QByteArray m_block;
    QSharedPointer<QTimer> m_threadTimer;

    QMutex m_mutex;
};

#endif // SERVERTHREAD_H
