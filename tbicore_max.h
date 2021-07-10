#ifndef TBICORE_MAX_H
#define TBICORE_MAX_H

#include "tbicore_constants.h"
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
#include "tbiclass_line.h"
#include <QRandomGenerator>
#include "tbiparameterclass_ranscaparms.h"
#include "tbiclass_dataset.h"
#include "tbiclass_datadistributionset.h"
#include "tbiparameterclass_gausiandecluster.h"
#include "tbiparameterclass_imageintensity.h"
#include "tbiclass_threepointtrackingmanager.h"
#include "tbicore_gary.h"
#include "tbicore_mary.h"


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

/**************************************************************
Motor_Calibration_Sequence_t
Description:
    A enumerated type used for determining the phase of auto
    motor calibration.
 **************************************************************/
class Motor_Calibration_Sequence: public QObject
{
    Q_OBJECT
public:
    Motor_Calibration_Sequence() : QObject() {}

    enum MotorCalibration_t {NOT_CALIBRATING = 0x01,
                             GET_PNT_1 = 0x02,
                             MOVE_FOR_PNT2 = 0x03,
                             WAIT_FOR_MOTION_STATUS_IDLE = 0x04,
                             GET_PNT_2 = 0x05,
                             SET_CALIBRATION_PARAMS = 0x06,
                             MOVE_TO_PNT1 = 0x07,
                             WAIT_FOR_MOTIONSTATUS_IDLE2 = 0x08,
                             FINISH_CALIBRATION = 0x09};
    Q_ENUMS(MotorCalibration_t)
    static void declareQML()
    {
        qmlRegisterType<Motor_Calibration_Sequence>("tbi.vision.components", 1, 0, "Motor_Calibration_Sequence");
    }
    MotorCalibration_t m_calibration_status;
};


/***************************************************************
 A Note on the Processing Sequence

 The Processing Sequence is initiated inside the slot recieveNewCVMat().

 When This Mat is Recieved into Max A Sequence of Events Happens.

 1. The OpenCV Blur is Applied.
 2. The OpenCV Threshold is Applied.
    This ends the Use of the OpenCV Library.
 3. The Pixel Column Processing Routines are Applied.
    Total Image Intensity is Calculated Here.
 4. The inlierdataset Processing Routines are Applied.
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
    void processMotorCalibration(TBIThreePointTrackingContainer &_trackedpoint);






public:
    explicit Max(QObject *parent = nullptr);
    ~Max();
    QString getTimeinLoop(){return m_timeinloop;}
    quint64 getTotalImageIntensity(){return m_total_image_intensity;}

    Q_INVOKABLE bool getEmitExtraMats(){return m_emitextramats;}
    void setEmitExtraMats(bool _flag){m_emitextramats = _flag; emit emitExtraMatsChanged();}
    void setGary(Gary *_gy){m_gary = _gy;}
    void setMary(Mary *_my){m_mary = _my;}

private:
    //The Maximum Frame Size For The Camera

    Gary *m_gary;
    Mary *m_mary;
    bool m_in_proccesing_loop;

    //Elements For GUI Display----------------------------------------
    QString m_timeinloop;
    QElapsedTimer m_timer;
    bool m_emitextramats;

    //CV Processing Variables-----------------------------------------
    int m_blur;
    int m_thresholdmin;
    int m_thresholdmax;

    //Image Intensity Parameters
    TBIImageIntensityParameters m_image_intensity_parameters;
    //Pixel Column Phase Processing Variables----------------------------
    TBIGausianDeclusteringParameters m_gausian_decluster_parameters;

    //Geometric Construction Processing Variables----------------------

    //Flattening Phase Data Variables-----------------------------------
    //These are Used Throught The Class.
    //You Must Call
    int m_flattened_rows;
    int m_flattened_cols;
    int m_flattened_iohrv; //IndexOfHighestRowValue i.e index of the inlierdataset array with bottom pixel value
    float m_flattened_hrv; //Highest Row Value

    //Pixel Column Phase Processing Variables--------------------------
    quint64 m_total_image_intensity;


    //Data Sets and Distribution Sets-------------------------------------------------------------------------
    TBIDataSet *m_gausian_decluster_ds;
    TBIDataSet *m_filtered_gausian_decluster_ds;
    TBIDataDistributionSet *m_gausian_decluster_distro;
    TBIDataSet *m_left_ransac_tsg_ds;
    TBIDataSet *m_left_inlier_tsg_ds;
    TBIDataSet *m_right_ransac_tsg_ds;
    TBIDataSet *m_right_inlier_tsg_ds;
    TBIDataSet *m_left_ransac1_bwl_ds;
    TBIDataSet *m_left_ransac2_bwl_ds;
    TBIDataSet *m_left_inlier_bwl_ds;
    TBIDataSet *m_right_ransac1_bwl_ds;
    TBIDataSet *m_right_ransac2_bwl_ds;
    TBIDataSet *m_right_inlier_bwl_ds;
    TBIDataSet *m_joint_ds;



    TBIDataSet *m_dummy_set1; //Used For Static Storage. So Other Functions Dont Have To Recreate Them in the Loops.
    TBIDataSet *m_dummy_set2; //Used For Static Storage. So Other Functions Dont Have To Recreate Them in the Loops.

    //inlierdataset Phase Data Variables-------------------------------------
    int m_allowable_discontinuities;

    //Ransac. Left and Right Top Surface Lines and Bevel Wall Lines(Voting Algorythms)-------
    TBILine m_ransac_left_tsl;
    TBIRansacParameter m_left_tsl_ransac;
    TBILine m_ransac_right_tsl;
    TBIRansacParameter m_right_tsl_ransac;
    TBILine m_ransac_left_bwl;
    TBIRansacParameter m_left_bwl_ransac;
    TBILine m_ransac_right_bwl;
    TBIRansacParameter m_right_bwl_ransac;
    //Geometric Construction Phase Data Variables-----------------------
    TBILine m_geo_left_tsl;
    TBILine m_geo_right_tsl;
    TBILine m_geo_left_bwl;
    TBILine m_geo_right_bwl;

    //Tracking Point Manager
    TBIThreePointTrackingManager m_three_point_tracking_manager;
    TBIThreePointTrackingContainer m_motor_cal_pnt1;
    TBIThreePointTrackingContainer m_motor_cal_pnt2;
    Motor_Calibration_Sequence::MotorCalibration_t m_motor_cal_seq;



    //Public Slots----------------------------------------------------------
public slots:

    void recieveNewCVMat(const cv::Mat& _mat_frame);
    void processVGrooveTracking(const cv::Mat _mat_frame);

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
    void newInlierDataSetMatProcessed(const cv::Mat& _inlierds_frame);
    void newRansacMatProcessed(const cv::Mat& _ransac_frame);
    void newGeoConstructionMatProcessed(const cv::Mat& _ransac_frame);
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
    void failedInlierDSMat();
    void emitExtraMatsChanged();
    void onControlModeChanged(GaryControlMode::ControlMode_t _controlmode);



};

#endif // TBICORE_MAX_H
