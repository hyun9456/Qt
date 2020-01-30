#ifndef FPSTHREAD_H
#define FPSTHREAD_H

#include <QThread>
#include <QMutex>
#include "tcpserversocket.h"

class FPSThread : public QThread
{
    Q_OBJECT
public:
    FPSThread(QMutex &mutex, QByteArray &avgRgb, TcpServerSocket& server, unsigned long FPS = 60);
    void setFPS(unsigned long FPS);
private:
    void run();

    QMutex &m_mutex;
    QByteArray &m_avgRgb;
    TcpServerSocket& m_server;
    unsigned long m_FPS;
};

#endif // FPSTHREAD_H
