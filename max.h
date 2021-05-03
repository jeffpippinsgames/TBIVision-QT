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
#include <QList>
#include <vector>
#include "pixelcolumnclass.h"

using namespace cv;

/**************************************************************
Max
Max Is the Welder.
He Decides Where to Place the Weld.
He Deciphers The Images and Performs the Seam Tracking

Description:
   The seam tracking functionality is
  encapsulated inside Max.
 **************************************************************/
class Max : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString timeinloop READ getTimeinLoop NOTIFY timeInLoopChanged)
    Q_PROPERTY(quint64 total_image_intensity READ getTotalImageIntensity NOTIFY totalImageIntensityChanged)

private:
    void blankProcessingArrays();
    bool fillFlattenedArray(cv::Mat &_src);
    bool buildPixelClusterList(cv::Mat &_src, cv::Mat &_dst);
    void fillColumnClusterList();
    quint64 getTotalImageIntensity(){return m_total_image_intensity;}

public:
    explicit Max(QObject *parent = nullptr);
    ~Max();
    QString getTimeinLoop(){return m_timeinloop;}

private:
    //The Maximum Frame Size For The Camera
    static const int Mat_Max_Width = 720;
    static const int Mat_Max_Height = 540;

    //Elements For GUI Display----------------------------------------
    QString m_timeinloop;
    QElapsedTimer m_timer;

    //CV Processing Variables-----------------------------------------
    int m_blur;
    int m_thresholdmin;
    int m_thresholdmax;
    //Flattening Phase Processing Variables----------------------------
    quint64 m_max_image_intensity;
    quint64 m_min_image_intensity;
    int m_min_cluster_size;
    int m_max_cluster_size;

    //Geometric Construction Processing Variables----------------------

    //Flattening Phase Data Variables-----------------------------------
    int m_flattened_pixel_array[Mat_Max_Width][Mat_Max_Height];
    int m_flattened_rows;
    int m_flattened_cols;
    quint64 m_total_image_intensity;
    std::vector<PixelColumnClass> m_column_cluster_list;
    float m_skeletal_line_array[Mat_Max_Width];
    //Geometric Construction Phase Data Variables-----------------------



//Public Slots----------------------------------------------------------
public slots:
    void recieveNewCVMat(const cv::Mat& _mat_frame);
    void onBlurChange(int _blur);
    void onThresholdMinChange(int _min);
    void onThresholdMaxChange(int _max);


//Signals----------------------------------------------------------------
signals:
    void timeInLoopChanged(QString _timinloop);
    void newFrameProcessed(const QImage& _qimage);
    void newRawMatProcessed(const cv::Mat& _raw_frame);
    void newBlurMatProcessed(const cv::Mat& _blur_frame);
    void newThresholdMatProcessed(const cv::Mat& _threshold_frame);
    void newPixelColumnMatProcessed(const cv::Mat& _pixel_column_frame);
    void processingComplete();
    void viewportChanged();
    void aboutToDestroy();
    void totalImageIntensityExceeded();
    void totalImageIntensityChanged(quint64 _tii);

};

#endif // MAX_H
