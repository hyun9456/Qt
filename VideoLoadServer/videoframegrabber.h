#ifndef VIDEOFRAMEGRABBER_H
#define VIDEOFRAMEGRABBER_H

#include <QAbstractVideoSurface>

class VideoFrameGrabber : public QAbstractVideoSurface
{
public:
    VideoFrameGrabber();
    bool present(const QVideoFrame &frame);
signals:
    void frame(QImage frame);
};

#endif // VIDEOFRAMEGRABBER_H
