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
#include "tbilinearransac.h"

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


    //Scan Data Flattening Processing
    bool doImageFlattening(cv::Mat &_src, std::vector<TBIPoint> &_data, quint64 *_tii, const quint64 _max_tii, const quint64 _min_tii);
    void drawFlattenedImageDataToMat(cv::Mat &_dst, const std::vector<TBIPoint> &_data);
    void drawFlattenedImageDataToMat(cv::Mat &_dst, const std::vector<TBIPoint> &_data, const cv::Scalar _color);

    void copyFlattenedData(const std::vector<TBIPoint> &_src, std::vector<TBIPoint> &_dst);

    void removeInliersFromFlattenedData(std::vector<TBIPoint> &_dstdata, const TBILine &_line, const float _distance_threshold_top, const float _distance_threshold_bottom);
    void buildFlattenDataFromInliers(const std::vector<TBIPoint> &_srcdata, std::vector<TBIPoint> &_dstdata, const TBILine &_line, const float _distance_threshold_top, const float _distance_threshold_bottom);
    bool buildFlattenDataFromBegining(const std::vector<TBIPoint> &_srcdata, std::vector<TBIPoint> &_dstdata, const int _consequetiverows);
    bool buildFlattenDataFromEnd(const std::vector<TBIPoint> &_srcdata, std::vector<TBIPoint> &_dstdata, const int _consequetiverows);


    //Scan Data Ransac Processing
    bool doRansacLineProcessing(TBILine &_line, const TBILinearRansac &_ransac, const std::vector<TBIPoint> &_vector);
    bool doRansacLineProcessing(TBILine &_line, const TBILinearRansac &_ransac, const std::vector<TBIPoint> &_vector, const int _startvectorindex, const int _endvectorindex);


   //Pixel Column Processing
    bool doPixelColumnProcessing(cv::Mat &_src, cv::Mat &_dst, PixelColumnClass* _pixel_column_array, quint64 *_tii,
                                 quint64 _max_tii, quint64 _min_tii, int _min_cluster_size, int _max_cluster_size,
                                 int _max_clusters_in_column);

    //Skeleton Processing
    bool doSkeletonProcessing(cv::Mat &_dst, PixelColumnClass *_pixel_column_array, float *_skel_array, float _continuity_threshold);

    void deleteSkelPointsAbovePZL(TBILine &_lpzl, TBILine &_rpzl, float *_skel_array);


    //Ransac Type Processing
    bool doRansacLineProcessing(cv::Mat &_dst, TBILine &_line, TBILinearRansac &_ransac, float* _skeletalarray, int _start_index, int _end_index, cv::Scalar _line_color);


    bool doRansacVertexProcessing(TBILine &_ltsl, TBILine &_rtsl, float *_skel_array, TBIPoint &_lv, TBIPoint &_rv, const float _left_vertex_dist_threshold, const float _right_vertex_dist_threshold);


    //Split Merge Processing
    bool doSplitMergeProcesssing(float *_skel_array, int _max_index, std::vector<TBILine> &_line_vectors,
                                 float _min_distance_threshold);

    bool setProjectedRansacLines(TBILine &_src_tsl_left, TBILine &_src_tsl_right,
                                 TBILine &_src_bwl_left, TBILine &_src_swl_right,
                                 TBILine &_dst_tsl_left, TBILine &_dst_tsl_right,
                                 TBILine &_dst_bwl_left, TBILine &_dst_swl_right);

    bool updateFlattenedMembers(cv::Mat &_src); //Almost All Other Functions Require this to be run first.


    //Misc Functions
    int randomInt(int _min, int _max);






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
    std::vector<TBIPoint> m_flattened_scan_data;
    std::vector<TBIPoint> m_flattened_left_tsl_data;
    std::vector<TBIPoint> m_flattened_right_tsl_data;
    std::vector<TBIPoint> m_flattened_left_bwl_data;
    std::vector<TBIPoint> m_flattened_right_bwl_data;
    std::vector<TBIPoint> m_flattened_bevel_wall_data;
    quint64 m_total_image_intensity;
    PixelColumnClass m_cluster_columns[Mat_Max_Width];

    //Skeletal Phase Data Variables-------------------------------------
    float m_skeletal_line_array[Mat_Max_Width];
    int m_allowable_discontinuities;

    //Ransac. Left and Right Top Surface Lines and Bevel Wall Lines(Voting Algorythms)-------
    TBILine m_left_tsl;
    TBILinearRansac m_left_tsl_ransac;

    TBILine m_right_tsl;
    TBILinearRansac m_right_tsl_ransac;

    TBILine m_left_bwl;
    TBILinearRansac m_left_bwl_ransac;

    TBILine m_right_bwl;
    TBILinearRansac m_right_bwl_ransac;

    TBIPoint m_right_ransac_vertex;
    TBIPoint m_left_ransac_vertex;

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

    void onLeftTSLIdealAngle(float _idealangle){m_left_tsl_ransac.setIdealAngle(_idealangle);}
    void onLeftTSLAllowedAngleVariance(float _allowedanglevariance){m_left_tsl_ransac.setAllowedAngleVariance(_allowedanglevariance);}
    void onLeftTSLMinVotes(int _minvotes){m_left_tsl_ransac.setMinVotes(_minvotes);}
    void onLeftTSLDistanceThreshold(float _distthreshold){m_left_tsl_ransac.setDistanceThreshold(_distthreshold);}
    void onLeftTSLIterations(int _iterations){m_left_tsl_ransac.setIterations(_iterations);}

    void onRightTSLIdealAngle(float _idealangle){m_right_tsl_ransac.setIdealAngle(_idealangle);}
    void onRightTSLAllowedAngleVariance(float _allowedanglevariance){m_right_tsl_ransac.setAllowedAngleVariance(_allowedanglevariance);}
    void onRightTSLMinVotes(int _minvotes){m_right_tsl_ransac.setMinVotes(_minvotes);}
    void onRightTSLDistanceThreshold(float _distthreshold){m_right_tsl_ransac.setDistanceThreshold(_distthreshold);}
    void onRightTSLIterations(int _iterations){m_right_tsl_ransac.setIterations(_iterations);}

    void onLeftBWLIdealAngle(float _idealangle){m_left_bwl_ransac.setIdealAngle(_idealangle);}
    void onLeftBWLAllowedAngleVariance(float _allowedanglevariance){m_left_bwl_ransac.setAllowedAngleVariance(_allowedanglevariance);}
    void onLeftBWLMinVotes(int _minvotes){m_left_bwl_ransac.setMinVotes(_minvotes);}
    void onLeftBWLDistanceThreshold(float _distthreshold){m_left_bwl_ransac.setDistanceThreshold(_distthreshold);}
    void onLeftBWLIterations(int _iterations){m_left_bwl_ransac.setIterations(_iterations);}

    void onRightBWLIdealAngle(float _idealangle){m_right_bwl_ransac.setIdealAngle(_idealangle);}
    void onRightBWLAllowedAngleVariance(float _allowedanglevariance){m_right_bwl_ransac.setAllowedAngleVariance(_allowedanglevariance);}
    void onRightBWLMinVotes(int _minvotes){m_right_bwl_ransac.setMinVotes(_minvotes);}
    void onRightBWLDistanceThreshold(float _distthreshold){m_right_bwl_ransac.setDistanceThreshold(_distthreshold);}
    void onRightBWLIterations(int _iterations){m_right_bwl_ransac.setIterations(_iterations);}

    void onSplitDistance(float _distance){m_sm_distance_threshold = _distance;}
    void onSplitLength(float _length){m_sm_length_threshold = _length;}

//Signals----------------------------------------------------------------
signals:
    //QML Signals--------------------------------------------------------
    void timeInLoopChanged(QString _timinloop);
    void viewportChanged();
    void aboutToDestroy();
    void totalImageIntensityChanged(quint64 _tii);
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
    void failedRansacVertexProcesssing();
    void failedFlattenImageData();
    void emitExtraMatsChanged();


};

#endif // MAX_H
