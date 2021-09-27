#include "tbiweld_vgroovetrackingcontainer.h"

TBIWeld_VGrooveTrackingContainer::TBIWeld_VGrooveTrackingContainer(QObject *parent) : QObject(parent)
{
    m_tracking_boundary = 10;
}

TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t TBIWeld_VGrooveTrackingContainer::extractTrackingPointsFromGeometricEntities(TBILine &_left_tsl, TBILine &_right_tsl,
                                                                                                                                        TBILine &_left_bwl, TBILine &_right_bwl,
                                                                                                                                        TBIClass_OpenCVMatContainer &_mats)
{

    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Extract Left Tracking Point
    if(!_left_tsl.findPointofIntersection(_left_bwl, m_left_tracking_point))
    {
        if(m_showdebug) qDebug("TBIWeld_VGrooveTrackingContainer::extractTrackingPointsFromGeometricEntities() Could Not Build Left Tracking Point.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOEXTRACTTRACKINGPOINT;
    }
    //Draw To Mat
    cv::drawMarker(_mats.m_operation, m_left_tracking_point.toCVPoint(), m_tracking_point_cv_color,cv::MARKER_CROSS, 15, 1, cv::LINE_AA);

    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Extract Right Tracking Point
    if(!_right_tsl.findPointofIntersection(_right_bwl, m_right_tracking_point))
    {
        if(m_showdebug) qDebug("TBIWeld_VGrooveTrackingContainer::extractTrackingPointsFromGeometricEntities() Could Not Build Right Tracking Point.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOEXTRACTTRACKINGPOINT;
    }
    //Draw To Mat
    cv::drawMarker(_mats.m_operation, m_right_tracking_point.toCVPoint(), m_tracking_point_cv_color,cv::MARKER_CROSS, 15, 1, cv::LINE_AA);

    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Extract Root Tracking Point
    if(!_left_bwl.findPointofIntersection(_right_bwl, m_root_point))
    {
        if(m_showdebug) qDebug("TBIWeld_VGrooveTrackingContainer::extractTrackingPointsFromGeometricEntities() Could Not Build Root Tracking Point.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOEXTRACTTRACKINGPOINT;
    }
    //Draw To Mat
    cv::drawMarker(_mats.m_operation, m_root_point.toCVPoint(), m_tracking_point_cv_color,cv::MARKER_CROSS, 15, 1, cv::LINE_AA);


    //Calculate Joint Centroid
    m_joint_centroid.m_x = (m_left_tracking_point.m_x+m_right_tracking_point.m_x+m_root_point.m_x)/3;
    m_joint_centroid.m_y = (m_left_tracking_point.m_y+m_right_tracking_point.m_y+m_root_point.m_y)/3;
    //Draw Joint Centroid
    cv::drawMarker(_mats.m_operation, m_joint_centroid.toCVPoint(), m_tracking_point_cv_color,cv::MARKER_CROSS, 15, 1, cv::LINE_AA);

    return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK;
}

int TBIWeld_VGrooveTrackingContainer::getXMovement(TBIMotionControlParameters &_motioncontrolparams)
{
    return 0;
}

int TBIWeld_VGrooveTrackingContainer::getZMovement(TBIMotionControlParameters &_motioncontrolparams)
{
    return 0;
}


void TBIWeld_VGrooveTrackingContainer::setTrackToPoints()
{
    m_left_track_to_point = m_left_tracking_point;
    m_right_track_to_point = m_right_tracking_point;
    m_root_track_to_point = m_root_point;
    m_joint_track_to_centroid = m_joint_centroid;
}

void TBIWeld_VGrooveTrackingContainer::drawTrackToPointstoMat_FullAuto(TBIClass_OpenCVMatContainer &_mats)
{
    cv::circle(_mats.m_operation, m_left_track_to_point.toCVPoint(), m_movement_boundary, m_tracking_to_point_cv_color, 1, cv::LINE_AA);
    cv::drawMarker(_mats.m_operation, m_left_track_to_point.toCVPoint(), m_tracking_to_point_cv_color,cv::MARKER_DIAMOND, 40, 1, cv::LINE_AA);
    cv::circle(_mats.m_operation, m_right_track_to_point.toCVPoint(), m_movement_boundary, m_tracking_to_point_cv_color, 1, cv::LINE_AA);
    cv::drawMarker(_mats.m_operation, m_right_track_to_point.toCVPoint(), m_tracking_to_point_cv_color,cv::MARKER_DIAMOND, 40, 1, cv::LINE_AA);
}

void TBIWeld_VGrooveTrackingContainer::drawTrackToPointstoMat_HeightOnly(TBIClass_OpenCVMatContainer &_mats)
{
    cv::circle(_mats.m_operation, m_left_track_to_point.toCVPoint(), m_movement_boundary, m_tracking_to_point_cv_color, 1, cv::LINE_AA);
    cv::drawMarker(_mats.m_operation, m_left_track_to_point.toCVPoint(), m_tracking_to_point_cv_color,cv::MARKER_DIAMOND, 40, 1, cv::LINE_AA);
    cv::circle(_mats.m_operation, m_right_track_to_point.toCVPoint(), m_movement_boundary, m_tracking_to_point_cv_color, 1, cv::LINE_AA);
    cv::drawMarker(_mats.m_operation, m_right_track_to_point.toCVPoint(), m_tracking_to_point_cv_color,cv::MARKER_DIAMOND, 40, 1, cv::LINE_AA);
}

void TBIWeld_VGrooveTrackingContainer::drawTrackingPointsToMat_FullAuto(TBIClass_OpenCVMatContainer &_mats)
{

}

void TBIWeld_VGrooveTrackingContainer::drawTrackingPointsToMat_HeightOnly(TBIClass_OpenCVMatContainer &_mats)
{

}

void TBIWeld_VGrooveTrackingContainer::setJointDefinition()
{

}


