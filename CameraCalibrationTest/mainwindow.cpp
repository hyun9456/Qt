#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;
Mat cameraMatrix= Mat::eye(3, 3, CV_64FC1);
Mat distCoeffs = Mat::zeros(1, 5, CV_64FC1);


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cameraMatrix=(Mat1d(3, 3) << 5.9103361437520880e+002, 0., 3.1950000000000000e+002, 0.,
                  5.9103361437520880e+002, 2.3950000000000000e+002, 0., 0., 1 );
    distCoeffs=(Mat1d(1, 5) << 1.1560381963566951e-001, -1.0183054098232041e+000, 0., 0., 2.7143330446497718e+000);
    VideoCapture capture(0);
    if (!capture.isOpened()) {
        //Error
    }
    namedWindow("TEST", WINDOW_AUTOSIZE);
    Mat frame;
    Mat temp;
    while(1) {
        if (!capture.read(frame)) {
            //Error
        }
        undistort(frame,temp,cameraMatrix,distCoeffs);
        imshow("TEST", temp);
        waitKey(1);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

