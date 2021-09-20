#include "tbiweld_buttjointtrackingcontainer.h"

TBIWeld_ButtJointTrackingContainer::TBIWeld_ButtJointTrackingContainer(QObject *parent) : QObject(parent)
{

}

TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t TBIWeld_ButtJointTrackingContainer::extractTrackingPoint(TBILine &_tsl, TBIClass_OpenCVMatContainer &_mats)
{
    m_tracking_point = _tsl.getMidPointofLine();
    cv::drawMarker(_mats.m_operation, m_tracking_point.toCVPoint(), m_tracking_point_cv_color,cv::MARKER_CROSS, 15, 1, cv::LINE_AA);

    return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK;

}

void TBIWeld_ButtJointTrackingContainer::setTrackToPoints()
{
    m_track_to_point = m_tracking_point;
    m_track_to_point.m_y = m_track_to_point.m_y + m_tracking_boundary + 1; //FOR DEBUG USE.
}

void TBIWeld_ButtJointTrackingContainer::drawTrackToPointstoMat(TBIClass_OpenCVMatContainer &_mats)
{
    cv::Point _p1(m_track_to_point.m_x-50, m_track_to_point.m_y);
    cv::Point _p2(m_track_to_point.m_x+50, m_track_to_point.m_y);

    cv::Point _p3(m_track_to_point.m_x-20, m_track_to_point.m_y+m_tracking_boundary);
    cv::Point _p4(m_track_to_point.m_x+20, m_track_to_point.m_y+m_tracking_boundary);

    cv::Point _p5(m_track_to_point.m_x-20, m_track_to_point.m_y-m_tracking_boundary);
    cv::Point _p6(m_track_to_point.m_x+20, m_track_to_point.m_y-m_tracking_boundary);

    cv::line(_mats.m_operation, _p1, _p2, m_tracking_to_point_cv_color, 1, cv::LINE_AA, 0);
    cv::line(_mats.m_operation, _p3, _p4, m_tracking_to_point_cv_color, 1, cv::LINE_AA, 0);
    cv::line(_mats.m_operation, _p5, _p6, m_tracking_to_point_cv_color, 1, cv::LINE_AA, 0);
    cv::drawMarker(_mats.m_operation, m_track_to_point.toCVPoint(), m_tracking_to_point_cv_color,cv::MARKER_DIAMOND, 2*m_tracking_boundary, 1, cv::LINE_AA);
}

int TBIWeld_ButtJointTrackingContainer::getZTrackingDifference()
{

    int _diff = m_tracking_point.m_y - m_track_to_point.m_y;
    if(m_showdebug) qDebug() << "TBIWeld_ButtJointTrackingContainer::getZTrackingDifference(): _diff = " << _diff;
    if(abs(_diff) < m_tracking_boundary) return 0;
    return _diff;
}
