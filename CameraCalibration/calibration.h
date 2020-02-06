#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <QDebug>
#include "settings.h"
#include <opencv2/core/types_c.h>
#include <opencv2/core/core_c.h>

class Calibration
{
public:
    Calibration();
    void run(const string inputSettingsFile);
    Mat getCameraMatrix();
    Mat getDistCoeffs();

private:
    enum {DETECTION = 0, CAPTURING = 1, CALIBRATED = 2};
    void help();
    double computeReprojectionErrors(const vector<vector<Point3f>>& objectPoints,
                                     const vector<vector<Point2f>>& imagePoints,
                                     const vector<Mat>& rvecs, const vector<Mat>& tvecs,
                                     const Mat& cameraMatrix, const Mat& distCoeffs,
                                     vector<float>& perViewErrors);
    void calcBoardCornerPositions(Size boardSize, float squreSize, vector<Point3f>& corners,
                                  Settings::Pattern patternType);
    bool runCalibration(Settings& s, Size imageSize, Mat& cameraMatrix, Mat& distCoeffs,
                        vector<vector<Point2f>> imagePoints, vector<Mat>& rvecs, vector<Mat>& tvecs,
                        vector<float>& reprojErrs, double& totalAvgErr);
    void saveCameraParams(Settings& s, Size imageSize, Mat& cameraMatrix, Mat& distCoeffs,
                          const vector<Mat>& rvecs, const vector<Mat>& tvecs,
                          const vector<float>& reprojErrs, const vector<vector<Point2f>> imagePoints,
                          double& totalAvgErr);
    bool runCalibrationAndSave(Settings& s, Size imageSize, Mat& cameraMatrix, Mat& distCoeffs,
                               vector<vector<Point2f>> imagePoints);

    Mat m_cameraMatrix;
    Mat m_distCoeffs;
};

#endif // CALIBRATION_H
