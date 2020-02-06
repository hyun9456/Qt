#ifndef SETTINGS_H
#define SETTINGS_H
#include <QDebug>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

class Settings
{
public:
    Settings();
    enum Pattern { NOT_EXISTING, CHESSBOARD, CIRCLES_GRID, ASYMMETRIC_CIRCLES_GRID };
    enum InputType {INVALID, CAMERA, VIDEO_FILE, IMAGE_LIST};

    void write(FileStorage& fs) const;
    void read(const FileNode& node);
    void interprate();
    Mat nextImage();
    static bool readStringList(const string& filename, vector<string>& l);

public:
    Size boardSize;
    Pattern calibrationPattern;
    float squareSize;
    int nrFrames;
    float aspectRatio;
    int delay;
    bool bwritePoints;
    bool bwriteExtrinsics;
    bool calibZeroTangentDist;
    bool calibFixPrincipalPoint;
    bool flipVertical;
    string outputFileName;
    bool showUndistorsed;
    string input;


    int cameraID;
    vector<string> imageList;
    int atImageList;
    VideoCapture inputCapture;
    InputType inputType;
    bool goodInput;
    int flag;

private:
    string patternToUse;
};

#endif // SETTINGS_H
