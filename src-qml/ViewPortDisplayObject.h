//
#ifndef ViewPortDisplayObject_H
#define ViewPortDisplayObject_H

#include <QQuickPaintedItem>
#include <QQuickItem>
#include <QPainter>
#include <QImage>
#include <opencv2/opencv.hpp>



class ViewPortDisplayObject : public QQuickPaintedItem
{
    Q_OBJECT

public:
    //Q_PROPERTY(QString width READ Width WRITE setWidth NOTIFY widthChanged);


public:
    ViewPortDisplayObject();

signals:

    void WidthChanged();
    void HeightChanged();
    void NewFrameRecieved();

public slots:
    void NewFrameFromCVMatHandler(cv::Mat const& _matsrc);


private:
    void paint(QPainter *painter) override;

private:

private:
    QImage *m_frame;
};

#endif // ViewPortDisplayObject_H
