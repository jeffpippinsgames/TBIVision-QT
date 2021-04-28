#ifndef MAX_H
#define MAX_H

#include <QObject>
#include <QImage>
#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/imgproc/imgproc.hpp"
#include <QImage>
#include <QQuickItem>
#include <QString>
#include <QElapsedTimer>


using namespace cv;

class MaxPixelColumnContainer
{
public:
    MaxPixelColumnContainer();
    ~MaxPixelColumnContainer();
};



/**************************************************************
Max

Max Is the Welder.
He Decides Where to Place the Weld.

Description:
   The seam tracking functionality is
  encapsulated inside Max.
 **************************************************************/
class Max : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString timeinloop READ getTimeinLoop NOTIFY timeInLoopChanged)
public:
    explicit Max(QObject *parent = nullptr);
    ~Max();
    QString getTimeinLoop(){return m_timeinloop;}

private:
    QString m_timeinloop;
    QElapsedTimer m_timer;

public slots:
    void recieveNewCVMat(const cv::Mat& _mat_frame);
signals:

    void timeInLoopChanged(QString _timinloop);
    void newFrameProcessed(const QImage& _qimage);
    void newRawMatProcessed(const cv::Mat& _raw_frame);
    void newBlurMatProcessed(const cv::Mat& _blur_frame);
    void newThresholdMatProcessed(const cv::Mat& _threshold_frame);
    void processingComplete();
    void viewportChanged();
    void aboutToDestroy();

};

#endif // MAX_H
