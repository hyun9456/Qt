#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMediaPlayer>
#include "tcpserversocket.h"
#include "videoframegrabber.h"

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
    void sendAvgRgb(QByteArray avgRgb);

private:
    Ui::MainWindow *ui;
    TcpServerSocket m_server;
    QSharedPointer<QMediaPlayer> m_player;
    QSharedPointer<VideoFrameGrabber> m_grabber;
};
#endif // MAINWINDOW_H
