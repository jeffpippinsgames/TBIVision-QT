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
#include "tbiparameterclass_motioncontrolparams.h"


/*
 class TBIWeld_VGrooveTrackingContainer
    Is the container class that houses the V Groove Joint Tracking Extensions.
    It is part of the Processing Pipeline That is responsible of making sure the
    processed joint points make sense. This is done after the Geometric Construction
    of the image is handled.

    The Full Auto Joint Set Are Represented By Triangles.
    Once Successful Tracking Points are Found The System
    Will Compare it with the definition aquired at the set
    track to time. The Definition is an overlayed bounded triangle
    that determines the points define an accurate joint.
    The system will then create a movement to assert to the
    track to  points.




 */

class TBIWeld_VGrooveTrackingContainer : public QObject
{
    Q_OBJECT
public:
    explicit TBIWeld_VGrooveTrackingContainer(QObject *parent = nullptr);

    void setTrackToPoints();
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t extractTrackingPointsFromGeometricEntities(TBILine &_left_tsl, TBILine &_right_tsl, TBILine &_left_bwl, TBILine &_right_bwl, TBIClass_OpenCVMatContainer &_mats);
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t checkTracking();
    int getXMovement(TBIMotionControlParameters &_motioncontrolparams);
    int getZMovement(TBIMotionControlParameters &_motioncontrolparams);



    void drawTrackToPointstoMat_FullAuto(TBIClass_OpenCVMatContainer &_mats);
    void drawTrackToPointstoMat_HeightOnly(TBIClass_OpenCVMatContainer &_mats);
    void drawTrackingPointsToMat_FullAuto(TBIClass_OpenCVMatContainer &_mats);
    void drawTrackingPointsToMat_HeightOnly(TBIClass_OpenCVMatContainer &_mats);
    void setJointDefinitionBoundaryOffset(int _offset){m_joint_def_boundary = _offset;}
    void setMotionRequirementBoundary(int _offset){m_movement_boundary = _offset;}

private:
        void setJointDefinition();


private:
    const bool m_showdebug = false;
    TBIPoint_Int m_left_tracking_point;
    TBIPoint_Int m_right_tracking_point;
    TBIPoint_Int m_root_point;
    TBIPoint_Int m_joint_centroid;

    //Joint Track To Point
    TBIPoint_Int m_left_track_to_point;
    TBIPoint_Int m_right_track_to_point;
    TBIPoint_Int m_root_track_to_point;
    TBIPoint_Int m_joint_track_to_centroid;

    //Joint Definition. Set At Set Time
    TBIPoint_Int m_left_joint_def;
    TBIPoint_Int m_right_joint_def;
    TBIPoint_Int m_root_joint_def;
    TBIPoint_Int m_joint_centroid_def;

    int m_joint_def_boundary;
    int m_movement_boundary;

    //Height Only Parameters
    TBILine m_top_surface_line;
    TBIPoint_Int m_tsl_mid_point;

    const cv::Scalar m_tracking_point_cv_color =CV_RGB(0,200,255);
    const cv::Scalar m_tracking_to_point_cv_color =CV_RGB(75,75,255);
    const cv::Scalar m_boundary_fill_cv_color = CV_RGB(25,200,25);

signals:

};

#endif // TBIWELD_VGROOVETRACKINGCONTAINER_H
