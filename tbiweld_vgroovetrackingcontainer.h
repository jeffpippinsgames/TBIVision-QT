#ifndef TBIWELD_VGROOVETRACKINGCONTAINER_H
#define TBIWELD_VGROOVETRACKINGCONTAINER_H

#include <QObject>
#include "tbiclass_line.h"
#include "tbiclass_pointint.h"
#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/imgproc/imgproc.hpp"
#include "tbiweld_pipelineprocessingreturntype.h"
#include "tbiparameterclass_motioncontrolparams.h"
#include "tbicore_gary.h"
#include "tbiclass_opencvmatcontainer.h"

class TBIWeld_VGrooveTrackingContainer : public QObject
{
    Q_OBJECT
public:
    explicit TBIWeld_VGrooveTrackingContainer(QObject *parent = nullptr);

    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t extractTrackingPointsFromGeometricEntities(TBILine &_left_tsl, TBILine &_right_tsl, TBILine &_left_bwl, TBILine &_right_bwl, TBIClass_OpenCVMatContainer &_mats);
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t extractTrackingPointsForHeightOnly(TBILine &_tsl, TBIClass_OpenCVMatContainer &_mats);

    void setTrackToPoints(GaryControlMode::ControlMode_t _forcontrolmode = GaryControlMode::TBI_CONTROL_MODE_FULLAUTO_MODE);
    bool movementNeeded();
    int getXMovement(TBIMotionControlParameters &_motioncontrolparams);
    int getZMovement(TBIMotionControlParameters &_motioncontrolparams);
    void drawTrackToPointstoMat(TBIClass_OpenCVMatContainer &_mats, GaryControlMode::ControlMode_t _forcontrolmode = GaryControlMode::TBI_CONTROL_MODE_FULLAUTO_MODE);

private:
    const bool m_showdebug = false;
    TBIPoint_Int m_left_tracking_point;
    TBIPoint_Int m_right_tracking_point;
    TBIPoint_Int m_root_point;
    TBIPoint_Int m_joint_centroid;

    TBIPoint_Int m_left_track_to_point;
    TBIPoint_Int m_right_track_to_point;
    TBIPoint_Int m_root_track_to_point;
    TBIPoint_Int m_joint_track_to_centroid;

    TBIPoint_Int m_left_joint_def;
    TBIPoint_Int m_right_joint_def;
    TBIPoint_Int m_root_joint_def;
    TBIPoint_Int m_joint_centroid_def;

    int m_tracking_boundary;

    const cv::Scalar m_tracking_point_cv_color =CV_RGB(0,200,255);
    const cv::Scalar m_tracking_to_point_cv_color =CV_RGB(75,75,255);

signals:

};

#endif // TBIWELD_VGROOVETRACKINGCONTAINER_H
