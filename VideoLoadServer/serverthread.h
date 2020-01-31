#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QTimer>

class ServerThread : public QThread
{
    Q_OBJECT
public:
    ServerThread(qintptr ID, QObject *parent = 0, unsigned long FPS = 60);
    void run();

signals:
    void error(QTcpSocket::SocketError socketerror);

public slots:
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
};

#endif // SERVERTHREAD_H
