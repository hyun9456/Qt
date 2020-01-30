#include "fpsthread.h"

FPSThread::FPSThread(QMutex &mutex, QByteArray &avgRgb, TcpServerSocket& server, unsigned long FPS)
    : m_mutex(mutex),
      m_avgRgb(avgRgb),
      m_server(server),
      m_FPS(FPS)
{

}

void FPSThread::run()
{
}
