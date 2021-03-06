#ifndef TBICORE_MAX_H
#define TBICORE_MAX_H

#include "tbicore_constants.h"
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
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
#include "tbiparameterclass_motioncontrolparams.h"
#include "tbicore_gary.h"
#include "tbiweld_vgroove.h"
#include <QDataStream>
#include "tbiweld_vgroovetrackingcontainer.h"
#include "tbiweld_buttjointtrackingcontainer.h"
#include "tbiweld_buttjoint.h"
#include "tbiclass_microcontrollerstatuspacket.h"
#include "tbiweld_enumerator.h"


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
    Q_PROPERTY(bool emitExtraMats READ getEmitExtraMats WRITE setEmitExtraMats NOTIFY emitExtraMatsChanged)
    Q_PROPERTY(bool validtrackingpoint READ getValidTrackingPoint NOTIFY validTrackingPointChanged)
    Q_PROPERTY(bool validtracktopoint READ getValidTrackToPoint NOTIFY validTrackToPointChanged)
    Q_PROPERTY(int trackingpoint_x READ getTrackingPointX NOTIFY trackingPointXChanged)
    Q_PROPERTY(int trackingpoint_y READ getTrackingPointY NOTIFY trackingPointYChanged)
    Q_PROPERTY(int tracktopoint_x READ getTrackToPointX NOTIFY trackToPointXChanged)
    Q_PROPERTY(int tracktopoint_y READ getTrackToPointY NOTIFY trackToPointYChanged)

public:

    //Constructor and Deconstructor
    explicit Max(QObject *parent = nullptr);
    ~Max();

    //Get Methods
    QString getTimeinLoop(){return m_timeinloop;}
    Q_INVOKABLE bool getEmitExtraMats(){return m_emitextramats;}
    Q_INVOKABLE void attemptToToggleControlState(){if(m_gary->isLaserOn()) m_attempt_to_toggle_control_state = true;}
    bool getValidTrackingPoint(){return m_tracking_point_valid;}
    bool getValidTrackToPoint(){return m_track_to_point_valid;}
    int getTrackingPointX(){return m_tracking_x;}
    int getTrackingPointY(){return m_tracking_y;}
    int getTrackToPointX(){return m_track_to_x;}
    int getTrackToPointY(){return m_track_to_y;}

    //Set Methods
    void setEmitExtraMats(bool _flag){m_emitextramats = _flag; emit emitExtraMatsChanged();}
    void setGary(Gary *_gy){m_gary = _gy;}
    void setRootQMLContextProperties(QQmlApplicationEngine &_engine);

    //Save File Methods
    void setDefautValues();
    void saveToFile(QDataStream &_filedatastream);
    void loadFromFile(QDataStream &_filedatastream);

private:

    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t processVGroovePipeline(TBIClass_OpenCVMatContainer &_mats, MicroControllerStatusPacket &_micro_status_packet);
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t processButtJointPipeline(TBIClass_OpenCVMatContainer &_mats, MicroControllerStatusPacket &_micro_status_packet);


    const bool m_showdebug = false;
    Gary *m_gary;
    bool m_in_proccesing_loop;

    TBIMotionControlParameters m_motion_control_params;
    TBIWeld_VGroove m_vgroove;
    TBIWeld_VGrooveTrackingContainer m_vgroove_tracking_container;

    bool m_tracking_point_valid;
    bool m_track_to_point_valid;
    int  m_tracking_x;
    int m_tracking_y;
    int m_track_to_x;
    int m_track_to_y;

    TBIWeld_ButtJoint m_buttjoint;
    TBIWeld_ButtJointTrackingContainer m_buttjoint_tracking_container;

    TBIWeld_ProcessingPipeLineReturnType m_pipelineresult;
    TBIWeldType_Enumerator m_joint_type;

    bool m_attempt_to_toggle_control_state;

    //Elements For GUI Display----------------------------------------
    QString m_timeinloop;
    QElapsedTimer m_timer;
    bool m_emitextramats;

    //Public Slots----------------------------------------------------------
public slots:
    void recieveNewCVMat(const cv::Mat& _mat_frame);

    //Signals----------------------------------------------------------------
signals:
    //QML Signals--------------------------------------------------------
    void timeInLoopChanged(QString _timinloop);
    void aboutToDestroy();
    //Mat Signals-----------------------------------------------
    void newFrameProcessed(const QImage& _qimage);
    void newRawMatProcessed(const cv::Mat& _raw_frame);
    void newPreBlurMatProcessed(const cv::Mat& _blur_frame);
    void newErodeMatProcessed(const cv::Mat& _erode_frame);
    void newEdgeMatProcessed(const cv::Mat& _edge_frame);
    void newPostBlurMatProcessed(const cv::Mat& _blur_frame);
    void newThresholdMatProcessed(const cv::Mat& _threshold_frame);
    void newPixelColumnMatProcessed(const cv::Mat& _pixel_column_frame);
    void newInlierDataSetMatProcessed(const cv::Mat& _inlierds_frame);
    void newRansacMatProcessed(const cv::Mat& _ransac_frame);
    void newGeoConstructionMatProcessed(const cv::Mat& _ransac_frame);
    void newOperationMatProcessed(const cv::Mat &_operation_frame);
    //Ulility Signals-----------------------------------------
    void processingComplete();
    void emitExtraMatsChanged();
    void validTrackingPointChanged();
    void validTrackToPointChanged();
    void trackingPointXChanged();
    void trackingPointYChanged();
    void trackToPointXChanged();
    void trackToPointYChanged();

};

#endif // TBICORE_MAX_H
