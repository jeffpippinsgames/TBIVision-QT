#include "QmlTBIDisplay.h"


QmlTBIDisplay::QmlTBIDisplay(QObject *parent)
{
    m_scale_to_width = 720;
}

QmlTBIDisplay::~QmlTBIDisplay()
{

}

void QmlTBIDisplay::declareQML()
{
    qmlRegisterType<QmlTBIDisplay>("tbi.vision.components", 1, 0, "QmlTBIDisplay");
}

void QmlTBIDisplay::setScaleToWidth(int _scaletowidth)
{
    m_scale_to_width = _scaletowidth;
    emit scaleToWidthChanged();
}

void QmlTBIDisplay::recieveQImage(const QImage &frame)
{

    m_frame = frame.scaledToWidth(m_scale_to_width);
    update(); //redraw
    emit newFrameRecieved();
}

void QmlTBIDisplay::recieveCVMat(const cv::Mat& _mat)
{
    //cv::Mat _mat = __mat.clone();
    if(_mat.channels() == 1)
    {
        m_frame = QImage((uchar*)_mat.data, _mat.cols, _mat.rows, _mat.step, QImage::Format_Grayscale8).scaledToWidth(m_scale_to_width);
        update(); //redraw
        emit newFrameRecieved();
    }
    else if(_mat.channels() == 3)
    {
        m_frame = QImage((uchar*)_mat.data, _mat.cols, _mat.rows, _mat.step, QImage::Format_BGR888).scaledToWidth(m_scale_to_width);
        update(); //redraw
        emit newFrameRecieved();
    }

}

void QmlTBIDisplay::paint(QPainter *painter)
{
    if(m_frame.isNull()) return;
    painter->drawImage(0,0,m_frame);
}
