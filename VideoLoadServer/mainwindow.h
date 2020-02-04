#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMediaPlayer>
#include <QMutex>
#include "tcpserversocket.h"
#include "videoframegrabber.h"
#include "serverthread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_run_clicked();
    void on_pushButton_stop_clicked();
    void paintEvent(QPaintEvent * event);
    void setAvgRgb(QByteArray avgRgb);
    void setGrabberRgbMode(QByteArray block);

private:
    Ui::MainWindow *ui;
    QByteArray m_avgRgb;
    QMutex m_mutex;
    TcpServerSocket m_server;
    QSharedPointer<QMediaPlayer> m_player;
    QSharedPointer<VideoFrameGrabber> m_grabber;
    //QSharedPointer<FPSThread> m_fpsThread;
};
#endif // MAINWINDOW_H
