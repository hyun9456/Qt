#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "videoframegrabber.h"
#include <QFileDialog>
#include <QPixmap>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawImage(QImage image)
{
    QPixmap pix = QPixmap::fromImage(image);
    int w = ui->label->width();
    int h = ui->label->height();
    ui->label->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio));

    update();
}

void MainWindow::calcAvgRgb(QImage image)
{
    int sum_red = 0;
    int sum_green = 0;
    int sum_blue = 0;

    for(int w = 0; w < image.width(); w++) {
        for(int h = 0; h< image.height(); h++) {
            QRgb rgb = image.pixel(w,h);
            sum_red += qRed(rgb);
            sum_green += qGreen(rgb);
            sum_blue += qBlue(rgb);
        }
    }
    int size = image.width() * image.height();

    sum_red /= size;
    sum_green /= size;
    sum_blue /= size;

    qDebug()<<"("<<sum_red<<", "<<sum_green<<", "<<sum_blue << ")";
}

void MainWindow::on_pushButton_clicked()
{
    QString file_path = QFileDialog::getOpenFileName();
    if(false == file_path.isEmpty() ) {
        player.reset(new QMediaPlayer);
        //player->setVideoOutput(ui->videowidget);
        VideoFrameGrabber* grabber = new VideoFrameGrabber(ui->videowidget, this);
        player->setVideoOutput(grabber);

        connect(grabber, SIGNAL(frameAvailable(QImage)), this, SLOT(drawImage(QImage)));
        connect(grabber, SIGNAL(frameAvailable(QImage)), this, SLOT(calcAvgRgb(QImage)));
        player->setMedia(QUrl::fromLocalFile(file_path));

        player->play();

        qDebug() << player->state();

    }
}
