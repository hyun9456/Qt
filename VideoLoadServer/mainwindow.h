#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMediaPlayer>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int counter = 0;

private slots:
    void on_pushButton_clicked();
    void drawImage(QImage image);
    void calcAvgRgb(QImage image);

private:
    Ui::MainWindow *ui;
    QSharedPointer<QMediaPlayer> player;
    QImage frameImage;
};
#endif // MAINWINDOW_H
