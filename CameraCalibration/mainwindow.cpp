#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "calibration.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Calibration calib;
    calib.run("C:\\Users\\hynyh\\Documents\\Qt\\CameraCalibration\\default.xml");
}


MainWindow::~MainWindow()
{
    delete ui;
}

