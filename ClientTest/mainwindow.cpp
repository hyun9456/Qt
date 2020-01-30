#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QHostAddress>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_socket(this)
    , m_avgRgbMode(0)
{
    ui->setupUi(this);
    ui->radioButton_mode0->click();
    connect(ui->radioButton_mode0, &QRadioButton::clicked, this, &MainWindow::onSendMode);
    connect(ui->radioButton_mode1, &QRadioButton::clicked, this, &MainWindow::onSendMode);
    connect(ui->radioButton_mode2, &QRadioButton::clicked, this, &MainWindow::onSendMode);
    m_socket.connectToHost(QHostAddress("127.0.0.1"), 4242);
    connect(&m_socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onReadyRead()
{
    QByteArray datas = m_socket.readAll();
    qDebug() << datas;
    m_socket.write(QByteArray("ok !\n"));
}

void MainWindow::onSendMode()
{
    if (ui->radioButton_mode0->isChecked())
        m_avgRgbMode = 0;
    if (ui->radioButton_mode1->isChecked())
        m_avgRgbMode = 1;
    if (ui->radioButton_mode2->isChecked())
        m_avgRgbMode = 2;
//    qDebug() << m_rgbAvgMode;
    m_socket.write(QByteArray("Mode : " + QByteArray::number(m_avgRgbMode)));
}
