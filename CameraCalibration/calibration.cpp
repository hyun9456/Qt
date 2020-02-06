#include "calibration.h"

Calibration::Calibration()
{

}

void Calibration::run(const string inputSettingsFile)
{
    help();
    Settings s;
    //const string inputSettingsFile = "C:\\Users\\hynyh\\Documents\\Qt\\CameraCalibration\\default.xml";
    FileStorage fs(inputSettingsFile, FileStorage::READ);
    if (!fs.isOpened())
    {
        qDebug() << "Could not open the configuration file: \"" << QString::fromStdString(inputSettingsFile) << "\"" << endl;
    }
    s.read(fs["Settings"]);
    //fs["Settings"] >> s;
    fs.release();

    if (!s.goodInput)
    {
        qDebug() << "Invalid input detected. Application stopping. " << endl;
    }

    vector<vector<Point2f>> imagePoints;
    Size imageSize;
    int mode = s.inputType == Settings::IMAGE_LIST ? CAPTURING : DETECTION;
    clock_t prevTimestamp = 0;
    const Scalar RED(0,0,255), GREEN(0,255,0);
    const char ESC_KEY = 27;

    for(int i = 0;;++i)
    {
        Mat view;
        bool blinkOutput = false;

        view = s.nextImage();

        if(view.empty())
        {
            if(imagePoints.size() > 0)
                runCalibrationAndSave(s, imageSize, m_cameraMatrix, m_distCoeffs, imagePoints);
            break;
        }
        if(mode == CAPTURING && imagePoints.size() >= (unsigned)s.nrFrames)
        {
            if(runCalibrationAndSave(s, imageSize, m_cameraMatrix, m_distCoeffs, imagePoints))
                mode = CALIBRATED;
            else
                mode = DETECTION;
        }

        imageSize = view.size();
        if(s.flipVertical) flip(view, view, 0);

        vector<Point2f> pointBuf;

        bool found;
        switch(s.calibrationPattern)
        {
        case Settings::CHESSBOARD:
            found = findChessboardCorners(view, s.boardSize, pointBuf,
                                          CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);
            break;
        case Settings::CIRCLES_GRID:
            found = findChessboardCorners(view, s.boardSize, pointBuf);
            break;
        case Settings::ASYMMETRIC_CIRCLES_GRID:
            found = findChessboardCorners(view, s.boardSize, pointBuf, CALIB_CB_ASYMMETRIC_GRID);
            break;
        }

        if(found)
        {
            if(s.calibrationPattern == Settings::CHESSBOARD)
            {
                Mat viewGray;
                cvtColor(view, viewGray, COLOR_BGR2GRAY);
                cornerSubPix(viewGray, pointBuf, Size(11,11),
                              Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1));
            }

            if(mode == CAPTURING &&
                    (!s.inputCapture.isOpened() || clock() - prevTimestamp > s.delay*1e-3*CLOCKS_PER_SEC))
            {
                imagePoints.push_back(pointBuf);
                prevTimestamp = clock();
                blinkOutput = s.inputCapture.isOpened();
            }

            drawChessboardCorners(view, s.boardSize, Mat(pointBuf), found);
        }

        string msg = (mode == CAPTURING) ? "100/100" :
                                           mode == CALIBRATED ? "Clibrated" : "Press 'g' to start";
        int baseLine = 0;
        Size textSize = getTextSize(msg, 1, 1, 1, &baseLine);
        Point textOrigin(view.cols - 2*textSize.width - 10, view.rows - 2*baseLine - 10);

        if(mode == CAPTURING)
        {
            if(s.showUndistorsed)
                msg = format("%d/%d Undist", (int)imagePoints.size(), s.nrFrames);
            else
                msg = format("%d/%d", (int)imagePoints.size(), s.nrFrames);
        }

        putText(view, msg, textOrigin, 1, 1, mode == CALIBRATED ? GREEN : RED);

        if(blinkOutput)
            bitwise_not(view, view);

        if(mode == CALIBRATED && s.showUndistorsed)
        {
            Mat temp = view.clone();
            undistort(temp, view, m_cameraMatrix, m_distCoeffs);
        }

        imshow("Image View", view);
        char key = waitKey(s.inputCapture.isOpened() ? 50 : s.delay);

        if(key == ESC_KEY)
            break;

        if(key == 'u' && mode == CALIBRATED)
            s.showUndistorsed = !s.showUndistorsed;

        if(s.inputCapture.isOpened() && key == 'g')
        {
            mode = CAPTURING;
            imagePoints.clear();
        }
    }

    if(s.inputType == Settings::IMAGE_LIST && s.showUndistorsed)
    {
        Mat view, rview, map1, map2;
        initUndistortRectifyMap(m_cameraMatrix, m_distCoeffs, Mat(),
                                getOptimalNewCameraMatrix(m_cameraMatrix, m_distCoeffs, imageSize, 1, imageSize, 0),
                                imageSize, CV_16SC2, map1, map2);

        for(int i = 0; i<(int)s.imageList.size(); i++)
        {
            view = imread(s.imageList[i], 1);
            if(view.empty())
                continue;
            remap(view, rview, map1, map2, INTER_LINEAR);
            imshow("Image View", rview);
            char c = waitKey();
            if(c == ESC_KEY || c == 'q' || c == 'Q')
                break;
        }
    }
}

Mat Calibration::getCameraMatrix()
{
    return m_cameraMatrix;
}
Mat Calibration::getDistCoeffs()
{
    return m_distCoeffs;
}

void Calibration::help()
{
    qDebug() << "This is a camera calibration sample." << endl
             << "Usage: calibration configurationFile" << endl
             << "Near the sample file you'll find the configuration file, which has detailed help of "
             "how to edit it. It may be any OpenCV supported file format XML/YAML."<< endl;
}

double Calibration::computeReprojectionErrors(const vector<vector<Point3f>>& objectPoints,
                                 const vector<vector<Point2f>>& imagePoints,
                                 const vector<Mat>& rvecs, const vector<Mat>& tvecs,
                                 const Mat& cameraMatrix, const Mat& distCoeffs,
                                 vector<float>& perViewErrors)
{
    vector<Point2f> imagePoints2;
    int i, totalPoints = 0;
    double totalErr = 0, err;
    perViewErrors.resize(objectPoints.size());

    for(i = 0; i<(int)objectPoints.size(); ++i)
    {
        projectPoints(Mat(objectPoints[i]), rvecs[i], tvecs[i], cameraMatrix,
                      distCoeffs, imagePoints2);
        err = norm(Mat(imagePoints[i]), Mat(imagePoints2), CV_L2);

        int n = (int)objectPoints[i].size();
        perViewErrors[i] = (float) std::sqrt(err*err/n);
        totalErr    += err*err;
        totalPoints += n;
    }

    return std::sqrt(totalErr/totalPoints);
}

void Calibration::calcBoardCornerPositions(Size boardSize, float squreSize, vector<Point3f>& corners,
                              Settings::Pattern patternType)
{
    corners.clear();

    switch(patternType)
    {
    case Settings::CHESSBOARD:
    case Settings::CIRCLES_GRID:
        for(int i = 0; i< boardSize.height; ++i)
            for(int j = 0; j < boardSize.width; ++j)
                corners.push_back(Point3f(float(j*squreSize), float(i*squreSize), 0));
        break;

    case Settings::ASYMMETRIC_CIRCLES_GRID:
        for(int i = 0; i < boardSize.height; i++)
            for(int j = 0; j < boardSize.width; j++)
                corners.push_back(Point3f(float((2*j + i % 2)*squreSize), float(i*squreSize), 0));
        break;
    }
}

bool Calibration::runCalibration(Settings& s, Size imageSize, Mat& cameraMatrix, Mat& distCoeffs,
                    vector<vector<Point2f>> imagePoints, vector<Mat>& rvecs, vector<Mat>& tvecs,
                    vector<float>& reprojErrs, double& totalAvgErr)
{
    cameraMatrix = Mat::eye(3, 3, CV_64F);
    if(s.flag & CALIB_FIX_ASPECT_RATIO)
        cameraMatrix.at<double>(0,0) = 1.0f;

    distCoeffs = Mat::zeros(8, 1, CV_64F);

    vector<vector<Point3f>> objectPoints(1);
    calcBoardCornerPositions(s.boardSize, s.squareSize, objectPoints[0], s.calibrationPattern);

    objectPoints.resize(imagePoints.size(), objectPoints[0]);

    double rms = calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix,
                                 distCoeffs, rvecs, tvecs, s.flag|CALIB_FIX_K4|CALIB_FIX_K5);

    qDebug() << "Re-projection error reported by calibrateCamera: " << rms << endl;

    bool ok = checkRange(cameraMatrix) && checkRange(distCoeffs);

    totalAvgErr = computeReprojectionErrors(objectPoints, imagePoints,
                                            rvecs, tvecs, cameraMatrix, distCoeffs, reprojErrs);

    return ok;
}

void Calibration::saveCameraParams(Settings& s, Size imageSize, Mat& cameraMatrix, Mat& distCoeffs,
                      const vector<Mat>& rvecs, const vector<Mat>& tvecs,
                      const vector<float>& reprojErrs, const vector<vector<Point2f>> imagePoints,
                      double& totalAvgErr)
{
    FileStorage fs(s.outputFileName, FileStorage::WRITE);

    time_t t;
    time(&t);
    struct tm *t2 = localtime(&t);
    char buf[1024];
    strftime(buf, sizeof(buf)-1, "%c", t2);

    fs << "calibration_Time" << buf;

    if(!rvecs.empty() || !reprojErrs.empty())
        fs << "nrOfFrames" << (int)std::max(rvecs.size(), reprojErrs.size());
    fs << "image_Width" << imageSize.width;
    fs << "image_Hight" << imageSize.height;
    fs << "board_Width" << s.boardSize.width;
    fs << "board_Hight" << s.boardSize.height;
    fs << "square_Size" << s.squareSize;

    if(s.flag & CALIB_FIX_ASPECT_RATIO)
        fs << "FixAspectRatio" << s.aspectRatio;

    if(s.flag)
    {
        sprintf(buf, "flags: %s%s%s%s",
                s.flag & CALIB_USE_INTRINSIC_GUESS ? " +use_intrinsic_guess" : "",
                s.flag & CALIB_FIX_ASPECT_RATIO ? " +fix_aspectRatio" : "",
                s.flag & CALIB_FIX_PRINCIPAL_POINT ? " +fix_principal_point" : "",
                s.flag & CALIB_ZERO_TANGENT_DIST ? " +zero_tangent_dist" : "");
        // cvWriteComment(*fs, buf, 0);
    }
    fs << "flagValue" << s.flag;

    fs << "Camera_Matrix" << cameraMatrix;
    fs << "Distortion_Coefficients" << distCoeffs;

    fs << "Avg_Reprojection_Error" << totalAvgErr;
    if( !reprojErrs.empty())
        fs << "Per_View_Reprojection_Errors" << Mat(reprojErrs);

    if(!rvecs.empty() && tvecs.empty())
    {
        CV_Assert(rvecs[0].type() == tvecs[0].type());
        Mat bigmat((int)rvecs.size(), 6, rvecs[0].type());
        for(int i =0; i < (int)rvecs.size(); i++)
        {
            Mat r = bigmat(Range(i, i+1), Range(0,3));
            Mat t = bigmat(Range(i, i+1), Range(3,6));

            CV_Assert(rvecs[i].rows == 3 && rvecs[i].cols == 1);
            CV_Assert(tvecs[i].rows == 3 && tvecs[i].cols == 1);

            r = rvecs[i].t();
            t = tvecs[i].t();
        }
        fs << "Extrinsic_Parameters" << bigmat;
    }

    if(!imagePoints.empty())
    {
        Mat imagePtMat((int)imagePoints.size(), imagePoints[0].size(), CV_32FC2);
        for(int i = 0; i< (int)imagePoints.size(); i++)
        {
            Mat r = imagePtMat.row(i).reshape(2, imagePtMat.cols);
            Mat imgpti(imagePoints[i]);
            imgpti.copyTo(r);
        }
        fs << "Image_points" << imagePtMat;
    }
}

bool Calibration::runCalibrationAndSave(Settings& s, Size imageSize, Mat& cameraMatrix, Mat& distCoeffs,
                           vector<vector<Point2f>> imagePoints)
{
    vector<Mat> rvecs, tvecs;
    vector<float> reprojErrs;
    double totalAvgErr = 0;
    bool ok = runCalibration(s, imageSize, cameraMatrix, distCoeffs, imagePoints, rvecs, tvecs,
                             reprojErrs, totalAvgErr);
    qDebug() << (ok ? "Calibration succeeded" : "Calibration failed")
             << ". avg re Projection error = " << totalAvgErr;

    if(ok)
        saveCameraParams(s, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs, reprojErrs, imagePoints, totalAvgErr);

    return ok;
}
