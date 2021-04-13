#include "QImageQMLViewer.h"


QImageQMLViewer::QImageQMLViewer(QObject *parent)
{
    m_toby = nullptr;
    m_is_toby_set = false;
}

QImageQMLViewer::~QImageQMLViewer()
{

}

void QImageQMLViewer::declareQML()
{
    qmlRegisterType<QImageQMLViewer>("tbi.vision.components", 1, 0, "QImageQMLViewer");
}

void QImageQMLViewer::setToby(Toby* _toby)
{
    if(_toby != nullptr)
    {
        m_toby = _toby;
        m_is_toby_set = true;
        emit tobySet();
        QObject::connect(m_toby, SIGNAL(newFrameGrabbed(QImage &frame)), this, SLOT(newCameraFrameHandler(QImage &frame)));
    }
}

void QImageQMLViewer::newCameraFrameHandler(const QImage &frame)
{
    m_frame = frame.scaledToWidth(1440);
    update(); //redraw
    emit newFrameGrabbed();
}

void QImageQMLViewer::paint(QPainter *painter)
{
    painter->drawImage(0,0,m_frame);
}
