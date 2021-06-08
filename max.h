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
#include <QRandomGenerator>

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
    Q_PROPERTY(bool emitExtraMats READ getEmitExtraMats WRITE setEmitExtraMats NOTIFY emitExtraMatsChanged)


private:
    void blankProcessingArrays();

    bool doPixelColumnProcessing(cv::Mat &_src, cv::Mat &_dst, PixelColumnClass* _pixel_column_array, quint64 *_tii,
                                 quint64 _max_tii, quint64 _min_tii, int _min_cluster_size, int _max_cluster_size,
                                 int _max_clusters_in_column);
    bool doSkeletonProcessing(cv::Mat &_dst, PixelColumnClass *_pixel_column_array, float *_skel_array, float _continuity_threshold);

    bool doRansacLineProcessing(cv::Mat &_dst, TBILine &_line, int _total_iterations, int _vote_threshold,
                                float _distance_threshold, float _min_angle_to_horizon, float _max_angle_to_horizon,
                                float* _skeletalarray, int _start_index, int _end_index, cv::Scalar _line_color);

    bool doSplitMergeProcesssing(float *_skel_array, int _max_index, std::vector<TBILine> &_line_vectors,
                                 float _min_distance_threshold);

    bool setProjectedRansacLines(TBILine &_src_tsl_left, TBILine &_src_tsl_right,
                                 TBILine &_src_bwl_left, TBILine &_src_swl_right,
                                 TBILine &_dst_tsl_left, TBILine &_dst_tsl_right,
                                 TBILine &_dst_bwl_left, TBILine &_dst_swl_right);

    bool updateFlattenedMembers(cv::Mat &_src); //Almost All Other Functions Require this to be run first.


    void trimRansacTopSurfaceLinesForVGroove(TBILine &_src_tsl_left, TBILine &_src_tsl_right, float *_skel_array, float _cutoff_threshold);





public:
    explicit Max(QObject *parent = nullptr);
    ~Max();
    QString getTimeinLoop(){return m_timeinloop;}
    quint64 getTotalImageIntensity(){return m_total_image_intensity;}
    Q_INVOKABLE bool getEmitExtraMats(){return m_emitextramats;}
    void setEmitExtraMats(bool _flag){m_emitextramats = _flag; emit emitExtraMatsChanged();}

private:
    //The Maximum Frame Size For The Camera

    static const int Mat_Max_Width = 728;
    static const int Mat_Max_Height = 544;
    bool m_in_proccesing_loop;


    //Elements For GUI Display----------------------------------------
    QString m_timeinloop;
    QElapsedTimer m_timer;
    bool m_emitextramats;

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
    float m_flattened_hrv; //Highest Row Value

    //Pixel Column Phase Processing Variables--------------------------
    quint64 m_total_image_intensity;
    PixelColumnClass m_cluster_columns[Mat_Max_Width];

    //Skeletal Phase Data Variables-------------------------------------
    float m_skeletal_line_array[Mat_Max_Width];
    int m_allowable_discontinuities;

    //Ransac. Left and Right Top Surface Lines and Bevel Wall Lines(Voting Algorythms)-------
    TBILine m_left_tsl;

    float m_max_tslleft_angle;
    float m_min_tslleft_angle;
    int m_min_tslleft_votes;
    int m_tslleft_iterations;
    float m_tslleft_distance_threshold;

    TBILine m_right_tsl;
    float m_max_tslright_angle;
    float m_min_tslright_angle;
    int m_min_tslright_votes;
    int m_tslright_iterations;
    float m_tslright_distance_threshold;

    TBILine m_left_bwl;
    float m_max_bwlleft_angle;
    float m_min_bwlleft_angle;
    int m_min_bwlleft_votes;
    int m_bwlleft_iterations;
    float m_bwlleft_distance_threshold;

    TBILine m_right_bwl;
    float m_max_bwlright_angle;
    float m_min_bwlright_angle;
    int m_min_bwlright_votes;
    int m_bwlright_iterations;
    float m_bwlright_distance_threshold;

    TBILine m_ransac_projection_lefttsl;
    TBILine m_ransac_projection_righttsl;
    TBILine m_ransac_projection_leftbwl;
    TBILine m_ransac_projection_rightbwl;

    //Linear Topography. (Split and Merge Algorythms)-------------------
    std::vector<TBILine> m_topography_lines;
    float m_sm_distance_threshold;
    float m_sm_length_threshold;


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

    void onLeftTSLMinAngle(float _minangle){m_min_tslleft_angle = _minangle;}
    void onLeftTSLMaxAngle(float _maxangle){m_max_tslleft_angle = _maxangle;}
    void onLeftTSLMinVotes(int _minvotes){m_min_tslleft_votes = _minvotes;}
    void onLeftTSLDistanceThreshold(float _distthreshold){m_tslleft_distance_threshold = _distthreshold;}
    void onLeftTSLIterations(int _iterations){m_tslleft_iterations = _iterations;}

    void onRightTSLMinAngle(float _minangle){m_min_tslright_angle = _minangle;}
    void onRightTSLMaxAngle(float _maxangle){m_max_tslright_angle = _maxangle;}
    void onRightTSLMinVotes(int _minvotes){m_min_tslright_votes = _minvotes;}
    void onRightTSLDistanceThreshold(float _distthreshold){m_tslright_distance_threshold = _distthreshold;}
    void onRightTSLIterations(int _iterations){m_tslright_iterations = _iterations;}

    void onLeftBWLMinAngle(float _minangle){m_min_bwlleft_angle = _minangle;}
    void onLeftBWLMaxAngle(float _maxangle){m_max_bwlleft_angle = _maxangle;}
    void onLeftBWLMinVotes(int _minvotes){m_min_bwlleft_votes = _minvotes;}
    void onLeftBWLDistanceThreshold(float _distthreshold){m_bwlleft_distance_threshold = _distthreshold;}
    void onLeftBWLIterations(int _iterations){m_bwlleft_iterations = _iterations;}

    void onRightBWLMinAngle(float _minangle){m_min_bwlright_angle = _minangle;}
    void onRightBWLMaxAngle(float _maxangle){m_max_bwlright_angle = _maxangle;}
    void onRightBWLMinVotes(int _minvotes){m_min_bwlright_votes = _minvotes;}
    void onRightBWLDistanceThreshold(float _distthreshold){m_bwlright_distance_threshold = _distthreshold;}
    void onRightBWLIterations(int _iterations){m_bwlright_iterations = _iterations;}

    void onSplitDistance(float _distance){m_sm_distance_threshold = _distance;}
    void onSplitLength(float _length){m_sm_length_threshold = _length;}

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
    void newRansacMatProcessed(const cv::Mat& _ransac_frame);
    void newSplitMergeMatProcessed(const cv::Mat& _ransac_frame);
    void newOperationMatProcessed(const cv::Mat &_operation_frame);
    //ProccessingComplete Signal-----------------------------------------
    void processingComplete();
    //Processing Failure Signals-----------------------------------------
    void failedTIICheck(); //Total Image Intensity
    void failedDiscontinuityCheck();
    void failedRansacCheck(); //Voting Lines
    void failedSplitMergeCheck();
    void emitExtraMatsChanged();


};

#endif // MAX_H
