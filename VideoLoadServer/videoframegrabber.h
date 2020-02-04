#ifndef VIDEOFRAMEGRABBER_H
#define VIDEOFRAMEGRABBER_H

#include <QAbstractVideoSurface>
#include <QtWidgets>

class VideoFrameGrabber : public QAbstractVideoSurface
{
    Q_OBJECT

public:
    VideoFrameGrabber(QWidget *widget, QObject *parent = nullptr);

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;
    bool isFormatSupported(const QVideoSurfaceFormat &format) const;

    bool start(const QVideoSurfaceFormat &format);
    void stop();

    bool present(const QVideoFrame &frame);

    QRect videoRect() const { return m_targetRect; }
    void updateVideoRect();

    void paint(QPainter *painter);
    void calcRgb(QImage frame);

    void setRgbMode(unsigned int rgbMode);

private:
    QByteArray calcAvgRgb(QImage frame);
    QByteArray calcMedianRgb(QImage frame);
    QByteArray calcCenterRgb(QImage frame);

    QWidget *m_widget;
    QImage::Format m_imageFormat;
    QRect m_targetRect;
    QSize m_imageSize;
    QRect m_sourceRect;
    QVideoFrame m_currentFrame;
    unsigned int m_rgbMode;

signals:
    void frameAvailable(QImage frame);
    void rgbAvailable(QByteArray avgRgb);
};

#endif // VIDEOFRAMEGRABBER_H
