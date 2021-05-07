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
#include "tbiline.h"

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


/***************************************************************
 A Note on the Processing Sequence

 The Processing Sequence is initiated inside the slot recieveNewCVMat().

 When This Mat is Recieved into Max A Sequence of Events Happens.

 1. The OpenCV Blur is Applied.
 2. The OpenCV Threshold is Applied.
    This ends the Use of the OpenCV Library.
 3. The Pixel Column Processing Routines are Applied.
    Total Image Intensity is Calculated Here.
 4. The Skeletal Processing Routines are Applied.
    The Flattened Members are Updated Here.
    It is important that the flattened members be updated
    here as the future functions will use them to complete properly.
 5. The Top Surface Lines and Bevel Wall Lines are Generated.
 6. The Topography Sequential Line List is Generated.
 7. Tracking Points are Generated

 The QML GUI.
 The Mary Class For the most part interacts thru signal slots
 with the QML GUI.
 Max is wired into Mary, Toby and Gary.
 **************************************************************/

class Max : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString timeinloop READ getTimeinLoop NOTIFY timeInLoopChanged)
    Q_PROPERTY(quint64 total_image_intensity READ getTotalImageIntensity NOTIFY totalImageIntensityChanged)

private:
    void blankProcessingArrays();
    bool doPixelColumnProcessing(cv::Mat &_src, cv::Mat &_dst);
    bool doSkeletonProcessing(cv::Mat &_dst);
    bool doTSLProcessing(cv::Mat &_dst);
    bool doBWLProcessing(cv::Mat &_dst);
    bool fillBevelWallLines(cv::Mat &_dst);

    bool updateFlattenedMembers(cv::Mat &_src); //Almost All Other Functions Require this to be run first.




public:
    explicit Max(QObject *parent = nullptr);
    ~Max();
    QString getTimeinLoop(){return m_timeinloop;}
    quint64 getTotalImageIntensity(){return m_total_image_intensity;}

private:
    //The Maximum Frame Size For The Camera
    static const int Mat_Max_Width = 720;
    static const int Mat_Max_Height = 540;
    bool m_in_proccesing_loop;

    //Elements For GUI Display----------------------------------------
    QString m_timeinloop;
    QElapsedTimer m_timer;

    //CV Processing Variables-----------------------------------------
    int m_blur;
    int m_thresholdmin;
    int m_thresholdmax;
    //Pixel Column Phase Processing Variables----------------------------
    quint64 m_max_image_intensity;
    quint64 m_min_image_intensity;
    int m_min_cluster_size;
    int m_max_cluster_size;
    int m_max_clusterincol;
    //Geometric Construction Processing Variables----------------------

    //Flattening Phase Data Variables-----------------------------------
    //These are Used Throught The Class.
    //You Must Call
    int m_flattened_rows;
    int m_flattened_cols;
    int m_flattened_iohrv; //IndexOfHighestRowValue i.e index of the skeletal array with bottom pixel value

    //Pixel Column Phase Processing Variables--------------------------
    quint64 m_total_image_intensity;
    PixelColumnClass m_cluster_columns[Mat_Max_Width];
    std::vector<PixelColumnClass> m_column_cluster_list;

    //Skeletal Phase Data Variables-------------------------------------
    float m_skeletal_line_array[Mat_Max_Width];
    int m_allowable_discontinuities;

    //RANSAC. Left and Right Top Surface Lines and Bevel Wall Lines(Voting Algorythms)-------
    TBILine m_left_tsl;
    TBILine m_right_tsl;
    float m_max_tslleft_angle;
    float m_min_tslleft_angle;
    float m_max_tslright_angle;
    float m_min_tslright_angle;
    int m_min_tsl_votes;
    int m_tsl_iterations;
    float m_tsl_distance_threshold;

    TBILine m_left_bwl;
    TBILine m_right_bwl;
    float m_max_bwlleft_angle;
    float m_min_bwlleft_angle;
    float m_max_bwlright_angle;
    float m_min_bwlright_angle;
    int m_min_bwl_votes;
    int m_bwl_iterations;
    float m_bwl_distance_threshold;

    //Linear Topography. (Split and Merge Algorythms)-------------------
    std::vector<TBILine> m_topography_lines;
    float m_sm_distance_threshold;
    float m_sm_length_requirement;

    //Geometric Construction Phase Data Variables-----------------------





//Public Slots----------------------------------------------------------
public slots:
    void recieveNewCVMat(const cv::Mat& _mat_frame);
    void onBlurChange(int _blur);
    void onThresholdMinChange(int _min);
    void onThresholdMaxChange(int _max);
    void onMaxTIIChange(quint64 _tii);
    void onMinTIIChange(quint64 _tii);
    void onMaxClusterSizeChange(int _size);
    void onMinClusterSizeChange(int _size);
    void onMaxClustersInColChange(int _size);
    void onMaxDiscontinuityChange(int _value);


//Signals----------------------------------------------------------------
signals:
    //QML Signals--------------------------------------------------------
    void timeInLoopChanged(QString _timinloop);
    void viewportChanged();
    void aboutToDestroy();
    void totalImageIntensityChanged(quint64 _tii);
    void skeletalArrayChanged(float _value, int _index);
    //Mat Delivery Signals-----------------------------------------------
    void newFrameProcessed(const QImage& _qimage);
    void newRawMatProcessed(const cv::Mat& _raw_frame);
    void newBlurMatProcessed(const cv::Mat& _blur_frame);
    void newThresholdMatProcessed(const cv::Mat& _threshold_frame);
    void newPixelColumnMatProcessed(const cv::Mat& _pixel_column_frame);
    void newSkeletalMatProcessed(const cv::Mat& _skel_frame);
    void newTSLMatProcessed(const cv::Mat& _tsl_frame);
    void newOperationMatProcessed(const cv::Mat &_operation_frame);
    //ProccessingComplete Signal-----------------------------------------
    void processingComplete();
    //Processing Failure Signals-----------------------------------------
    void failedTIICheck(); //Total Image Intensity
    void failedDiscontinuityCheck();
    void failedTSLCheck(); //Top Surface Lines


};

#endif // MAX_H
