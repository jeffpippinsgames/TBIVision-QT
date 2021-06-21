#ifndef MAX_H
#define MAX_H

#include "tbiconstants.h"
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
#include "tbidataset.h"
#include "tbidatadistributionset.h"


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


//This creates the data mat and is where the m_scan_ds dataset is built.
bool clusterProcessThresholdMat(cv::Mat &_src, cv::Mat &_dst, quint64 *_tii, const quint64 _max_tii, const quint64 _min_tii, const int _min_cluster_size,
                                    const int _max_cluster_size, const int _max_clusters_in_column, TBIDataSet &_scandataset);

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


    //Data Sets and Distribution Sets-------------------------------------------------------------------------
    TBIDataSet *m_scan_ds;
    TBIDataDistributionSet *m_scan_distro;
    TBIDataSet *m_skeletal_ds;
    TBIDataSet *m_roughing_left_tsl_ds;
    TBIDataSet *m_roughing_right_tsl_ds;
    TBIDataSet *m_left_tsl_inliers_ds;
    TBIDataDistributionSet *m_left_tsl_distro;
    TBIDataSet *m_right_tsl_inliers_ds;
    TBIDataDistributionSet *m_right_tsl_distro;
    TBIDataSet *m_joint_ds;
    TBIDataSet *m_left_bwl_inlier_ds;
    TBIDataSet *m_right_bwl_inlier_ds;
    TBIDataSet *m_dummy_set1; //Used For Static Storage. So Other Functions Dont Have To Recreate Them in the Loops.
    TBIDataSet *m_dummy_set2; //Used For Static Storage. So Other Functions Dont Have To Recreate Them in the Loops.

    //Skeletal Phase Data Variables-------------------------------------
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
    void newDataSetMatProcessed(const cv::Mat& _ransac_frame);
    void newOperationMatProcessed(const cv::Mat &_operation_frame);
    //ProccessingComplete Signal-----------------------------------------
    void processingComplete();
    //Processing Failure Signals-----------------------------------------
    void failedTIICheck(); //Total Image Intensity
    void failedDiscontinuityCheck();
    void failedRansacCheck(); //Voting Lines
    void failedDataSetCheck();
    void failedRansacVertexProcesssing();
    void failedFlattenImageData();
    void failedSkeletonizeMat();
    void emitExtraMatsChanged();


};

#endif // MAX_H
