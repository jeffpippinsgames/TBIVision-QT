#ifndef TBIQML_DISPLAY_H
#define TBIQML_DISPLAY_H

#include <QObject>
#include <QQuickItem>
#include <QtQuick/QQuickPaintedItem>
#include <tbicore_toby.h>
#include <QPainter>
#include "opencv4/opencv2/core.hpp"

class QmlTBIDisplay: public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(int scaleToWidth READ getScaleToWidth WRITE setScaleToWidth NOTIFY scaleToWidthChanged)


public:
    explicit QmlTBIDisplay(QObject *parent = nullptr);
    ~QmlTBIDisplay();
    static void declareQML();
    int getScaleToWidth(){return m_scale_to_width;}
    void setScaleToWidth(int _scaletowidth);

private:
    QImage m_frame;
    int m_scale_to_width;
    void paint(QPainter *painter) override;

signals:
    void newFrameRecieved();
    void scaleToWidthChanged();

public slots:
    void recieveQImage(const QImage &_img);
    void recieveCVMat(const cv::Mat &_mat);


};
#endif // TBIQML_DISPLAY_H
