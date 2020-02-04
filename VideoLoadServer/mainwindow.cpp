#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "videoframegrabber.h"
#include <QFileDialog>
#include <QPixmap>
#include <QDebug>
#include <QVideoSurfaceFormat>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_player.reset(new QMediaPlayer);
    m_grabber.reset(new VideoFrameGrabber(this));
    m_player->setVideoOutput(m_grabber.data());
    connect(m_grabber.data(), &VideoFrameGrabber::frameAvailable, m_grabber.data(), &VideoFrameGrabber::calcRgb);
    connect(m_grabber.data(), &VideoFrameGrabber::rgbAvailable, this, &MainWindow::setAvgRgb);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent * event){
    QPainter painter(this);
    if(nullptr != m_grabber)
        m_grabber->paint(&painter);
}

void MainWindow::setAvgRgb(QByteArray avgRgb) {
    m_server.sendData(avgRgb);
}


void MainWindow::setGrabberRgbMode(QByteArray block) {
    if(nullptr != m_grabber) {
        unsigned int rgbMode = 0;
        if(block.compare("Mode : 0") == 0) {
            rgbMode = 0;
        } else if (block.compare("Mode : 1") == 0) {
            rgbMode = 1;
        } else if (block.compare("Mode : 2") == 0) {
            rgbMode = 2;
        }
        m_grabber->setRgbMode(rgbMode);
    }
}


void MainWindow::on_pushButton_clicked()
{
    QString file_path = QFileDialog::getOpenFileName();
    if(false == file_path.isEmpty() ) {
        m_player->setMedia(QUrl::fromLocalFile(file_path));

        m_player->play();
        m_avgRgb.setNum(0);

        qDebug() << m_player->state();
    }
}

void MainWindow::on_pushButton_run_clicked()
{
    m_server.run(4242);
    connect(&m_server, &TcpServerSocket::readyReadData, this, &MainWindow::setGrabberRgbMode);
}

void MainWindow::on_pushButton_stop_clicked()
{
    m_server.stop();
}
