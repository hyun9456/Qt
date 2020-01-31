#include "videoframegrabber.h"
#include <QVideoSurfaceFormat>

VideoFrameGrabber::VideoFrameGrabber(QWidget *widget, QObject *parent)
    : QAbstractVideoSurface(parent)
    , m_widget(widget)
    , m_imageFormat(QImage::Format_Invalid)
{
}

QList<QVideoFrame::PixelFormat> VideoFrameGrabber::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    Q_UNUSED(handleType);
    return QList<QVideoFrame::PixelFormat>()
        << QVideoFrame::Format_ARGB32
        << QVideoFrame::Format_ARGB32_Premultiplied
        << QVideoFrame::Format_RGB32
        << QVideoFrame::Format_RGB24
        << QVideoFrame::Format_RGB565
        << QVideoFrame::Format_RGB555
        << QVideoFrame::Format_ARGB8565_Premultiplied
        << QVideoFrame::Format_BGRA32
        << QVideoFrame::Format_BGRA32_Premultiplied
        << QVideoFrame::Format_BGR32
        << QVideoFrame::Format_BGR24
        << QVideoFrame::Format_BGR565
        << QVideoFrame::Format_BGR555
        << QVideoFrame::Format_BGRA5658_Premultiplied
        << QVideoFrame::Format_AYUV444
        << QVideoFrame::Format_AYUV444_Premultiplied
        << QVideoFrame::Format_YUV444
        << QVideoFrame::Format_YUV420P
        << QVideoFrame::Format_YV12
        << QVideoFrame::Format_UYVY
        << QVideoFrame::Format_YUYV
        << QVideoFrame::Format_NV12
        << QVideoFrame::Format_NV21
        << QVideoFrame::Format_IMC1
        << QVideoFrame::Format_IMC2
        << QVideoFrame::Format_IMC3
        << QVideoFrame::Format_IMC4
        << QVideoFrame::Format_Y8
        << QVideoFrame::Format_Y16
        << QVideoFrame::Format_Jpeg
        << QVideoFrame::Format_CameraRaw
        << QVideoFrame::Format_AdobeDng;
}

bool VideoFrameGrabber::isFormatSupported(const QVideoSurfaceFormat &format) const
{
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();

    return imageFormat != QImage::Format_Invalid
            && !size.isEmpty()
            && format.handleType() == QAbstractVideoBuffer::NoHandle;
}

bool VideoFrameGrabber::start(const QVideoSurfaceFormat &format)
{
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();

    if (imageFormat != QImage::Format_Invalid && !size.isEmpty()) {
        this->m_imageFormat = imageFormat;
        m_imageSize = size;
        m_sourceRect = format.viewport();

        QAbstractVideoSurface::start(format);

        m_widget->updateGeometry();
        updateVideoRect();

        return true;
    } else {
        return false;
    }
}

void VideoFrameGrabber::stop()
{
    m_currentFrame = QVideoFrame();
    m_targetRect = QRect();

    QAbstractVideoSurface::stop();

    m_widget->update();
}

bool VideoFrameGrabber::present(const QVideoFrame &frame)
{
    if (frame.isValid())
    {
        QVideoFrame cloneFrame(frame);
        cloneFrame.map(QAbstractVideoBuffer::ReadOnly);
        const QImage image(cloneFrame.bits(),
                           cloneFrame.width(),
                           cloneFrame.height(),
                           QVideoFrame::imageFormatFromPixelFormat(cloneFrame .pixelFormat()));
        emit frameAvailable(image); // this is very important
        cloneFrame.unmap();
    }

    if (surfaceFormat().pixelFormat() != frame.pixelFormat()
            || surfaceFormat().frameSize() != frame.size()) {
        setError(IncorrectFormatError);
        stop();

        return false;
    } else {
        m_currentFrame = frame;

        m_widget->repaint(m_targetRect);

        return true;
    }
}

void VideoFrameGrabber::updateVideoRect()
{
    QSize size = surfaceFormat().sizeHint();
    size.scale(m_widget->size().boundedTo(size), Qt::KeepAspectRatio);

    m_targetRect = QRect(QPoint(0, 0), size);
    m_targetRect.moveCenter(m_widget->rect().center());
}

void VideoFrameGrabber::paint(QPainter *painter)
{
    if (m_currentFrame.map(QAbstractVideoBuffer::ReadOnly)) {
        const QTransform oldTransform = painter->transform();

        if (surfaceFormat().scanLineDirection() == QVideoSurfaceFormat::BottomToTop) {
           painter->scale(1, -1);
           painter->translate(0, -m_widget->height());
        }

        QImage image(
                m_currentFrame.bits(),
                m_currentFrame.width(),
                m_currentFrame.height(),
                m_currentFrame.bytesPerLine(),
                m_imageFormat);

        painter->drawImage(m_targetRect, image, m_sourceRect);

        painter->setTransform(oldTransform);

        m_currentFrame.unmap();
    }
}

void VideoFrameGrabber::calcAvgRgb(QImage frame)
{
    double avgRed = 0;
    double avgGreen = 0;
    double avgBlue = 0;

    for(int w = 0; w < frame.width(); w++) {
        for(int h = 0; h< frame.height(); h++) {
            QRgb rgb = frame.pixel(w,h);
            avgRed += qRed(rgb);
            avgGreen += qGreen(rgb);
            avgBlue += qBlue(rgb);
        }
    }
    int size = frame.width() * frame.height();

    avgRed /= size;
    avgGreen /= size;
    avgBlue /= size;

    QByteArray avgRgb("(" + QByteArray::number(avgRed) + ", "+ QByteArray::number(avgGreen) + ", "+ QByteArray::number(avgBlue) + ")");

    //qDebug() << avgRgb;
    emit avgRgbAvailable(avgRgb);
}

void VideoFrameGrabber::setAvgRgbMode(unsigned int avgRgbMode)
{
    m_avgRgbMode = avgRgbMode;
}
