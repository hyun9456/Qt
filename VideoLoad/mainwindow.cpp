#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
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


void MainWindow::on_pushButton_load_clicked()
{
    file_path = QFileDialog::getOpenFileName();

    player.reset(new QMediaPlayer);
    player->setVideoOutput(ui->vw);
    player->setMedia(QUrl::fromLocalFile(file_path));

    player->play();

    qDebug() << player->state();
}
