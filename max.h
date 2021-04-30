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

private:
    bool FlattenMat(cv::Mat& _src, cv::Mat &_dst);

public:
    explicit Max(QObject *parent = nullptr);
    ~Max();
    QString getTimeinLoop(){return m_timeinloop;}

private:
    QString m_timeinloop;
    QElapsedTimer m_timer;
    int m_blur;
    int m_thresholdmin;
    int m_thresholdmax;
    unsigned long m_max_image_intensity;
    unsigned long m_min_image_intensity;
    unsigned long m_current_image_intensity;

    uint8_t *m_flattened_pixels;
    int m_rows;
    int m_cols;
    int m_stride;
    unsigned long m_total_image_intensity;




public slots:
    void recieveNewCVMat(const cv::Mat& _mat_frame);
    void onBlurChange(int _blur);
    void onThresholdMinChange(int _min);
    void onThresholdMaxChange(int _max);

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
