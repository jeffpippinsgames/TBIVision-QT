#include "tbiweld_vgroovetrackingcontainer.h"

TBIWeld_VGrooveTrackingContainer::TBIWeld_VGrooveTrackingContainer(QObject *parent) : QObject(parent)
{
    m_movement_boundary = 5;
    m_joint_def_boundary = 10;
    m_excessive_change_boundry = 15;

    m_tracking_stability_timer = new QTimer(this);
    m_tracking_stability_timer->setInterval(1000);
    m_tracking_stability_timer->setSingleShot(true);
    m_tracking_stable = false;
    QObject::connect(m_tracking_stability_timer, SIGNAL(timeout()), this, SLOT(onTrackingStabilityTimerTimeout()));

}



TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t TBIWeld_VGrooveTrackingContainer::extractTrackingPointsFromGeometricEntities(TBILine &_left_tsl, TBILine &_right_tsl,
                                                                                                                                        TBILine &_left_bwl, TBILine &_right_bwl)
{
    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Extract Left Tracking Point
    if(!_left_tsl.findPointofIntersection(_left_bwl, m_left_tracking_point))
    {
        if(m_showdebug) qDebug("TBIWeld_VGrooveTrackingContainer::extractTrackingPointsFromGeometricEntities() Could Not Build Left Tracking Point.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOEXTRACTTRACKINGPOINT;
    }

    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Extract Right Tracking Point
    if(!_right_tsl.findPointofIntersection(_right_bwl, m_right_tracking_point))
    {
        if(m_showdebug) qDebug("TBIWeld_VGrooveTrackingContainer::extractTrackingPointsFromGeometricEntities() Could Not Build Right Tracking Point.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOEXTRACTTRACKINGPOINT;
    }

    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Extract Root Tracking Point
    if(!_left_bwl.findPointofIntersection(_right_bwl, m_root_point))
    {
        if(m_showdebug) qDebug("TBIWeld_VGrooveTrackingContainer::extractTrackingPointsFromGeometricEntities() Could Not Build Root Tracking Point.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOEXTRACTTRACKINGPOINT;
    }

    //Calculate Joint Centroid
    m_joint_centroid.m_x = (m_left_tracking_point.m_x+m_right_tracking_point.m_x+m_root_point.m_x)/3;
    m_joint_centroid.m_y = (m_left_tracking_point.m_y+m_right_tracking_point.m_y+m_root_point.m_y)/3;
    //Draw Joint Centroid


    //Build Center Joint Line
    m_tracking_joint_line = TBILine(m_left_tracking_point, m_right_tracking_point);
    m_tracking_line_center_point = m_tracking_joint_line.getFloatMidPointofLine();

    //Check Instability If The Excess Threshold is Met Flag Instable and Start Timer.
    //Every Unstable Iteration Will Restart Timer.

    //First Time Clearing.
    if(!m_tracking_stable && !m_tracking_stability_timer->isActive())
    {
        m_tracking_stability_timer->start();
    }

    if((int)m_last_tracking_point.distance(m_tracking_line_center_point) > m_excessive_change_boundry)
    {
        m_tracking_stable = false;
        m_tracking_stability_timer->start();
    }
    m_last_tracking_point = m_tracking_line_center_point;

    if(!m_tracking_stable) return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_TRACKINGPOINTINSTABILITYERROR;

    return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK;
}
int TBIWeld_VGrooveTrackingContainer::getXMovement(TBIMotionControlParameters &_motioncontrolparams)
{

    //X Right Increases the Step Location
    //Which Means X Must Add Positive Steps
    //A Positive _xdiff Means Move Left
    //So Send Negative Steps
    //m_motion_control_params.setXStepsPerPixel(1214.37037);
    int _xsteps = m_tracking_line_center_point.m_y - m_track_to_line_center_point.m_y;
    //We want a circular movement boundry find the distance bewteen Points
    int _distance = (int)m_tracking_line_center_point.distance(m_track_to_line_center_point);
    if( _distance <= m_movement_boundary) _xsteps = 0;
    _xsteps = _xsteps * _motioncontrolparams.getXStepsPerPixel();
    if(m_showdebug) qDebug() << "TBIWeld_VGrooveTrackingContainer::getXMovement: _xsteps = " << _xsteps << " _distance = " << _distance;
    return _xsteps;
}

int TBIWeld_VGrooveTrackingContainer::getZMovement(TBIMotionControlParameters &_motioncontrolparams)
{
    //Z Up Increases the Step Location
    //Which Means Z Must Add Positive Steps
    //A Positive _zdiff Means Move Down
    //So Send Negative Steps
    //m_motion_control_params.setZStepsPerPixel(1214.37037);
    int _zsteps = m_tracking_line_center_point.m_y - m_track_to_line_center_point.m_y;
    //We want a circular movement boundry find the distance bewteen Points
    int _distance = (int)m_tracking_line_center_point.distance(m_track_to_line_center_point);
    if( _distance <= m_movement_boundary) _zsteps = 0;
    _zsteps = _zsteps * _motioncontrolparams.getZStepsPerPixel();
    if(m_showdebug) qDebug() << "TBIWeld_VGrooveTrackingContainer::getZMovement: _zsteps = " << _zsteps << " _distance = " << _distance;
    return _zsteps;
}

void TBIWeld_VGrooveTrackingContainer::drawTrackingPointstoMat(TBIClass_OpenCVMatContainer &_mats)
{
    cv::drawMarker(_mats.m_operation, m_left_tracking_point.toCVPoint(), m_tracking_point_cv_color,cv::MARKER_CROSS, 15, 1, cv::LINE_AA);
    cv::drawMarker(_mats.m_operation, m_right_tracking_point.toCVPoint(), m_tracking_point_cv_color,cv::MARKER_CROSS, 15, 1, cv::LINE_AA);
    cv::drawMarker(_mats.m_operation, m_joint_centroid.toCVPoint(), m_tracking_point_cv_color,cv::MARKER_CROSS, 15, 1, cv::LINE_AA);
    cv::drawMarker(_mats.m_operation, m_root_point.toCVPoint(), m_tracking_point_cv_color,cv::MARKER_CROSS, 15, 1, cv::LINE_AA);
    m_tracking_joint_line.drawOnMat(_mats.m_operation, m_tracking_point_cv_color);
    cv::drawMarker(_mats.m_operation, m_tracking_line_center_point.toCVPoint(), m_tracking_point_cv_color,cv::MARKER_CROSS, 15, 1, cv::LINE_AA);
}


void TBIWeld_VGrooveTrackingContainer::setTrackToPoints()
{
    m_left_track_to_point = m_left_tracking_point;
    m_right_track_to_point = m_right_tracking_point;
    m_root_track_to_point = m_root_point;
    m_joint_track_to_centroid = m_joint_centroid;
    m_track_to_joint_line = m_tracking_joint_line;
    m_track_to_line_center_point = m_tracking_line_center_point;

    m_track_to_line_center_point.m_x += m_movement_boundary + 5;
    m_track_to_line_center_point.m_y += m_movement_boundary + 5;

    m_left_joint_def = m_left_tracking_point;
    m_right_joint_def = m_right_tracking_point;
    m_root_joint_def = m_root_point;
    m_joint_centroid_def = m_joint_centroid;
}

void TBIWeld_VGrooveTrackingContainer::drawTrackToPointstoMat_FullAuto(TBIClass_OpenCVMatContainer &_mats)
{
    //cv::circle(_mats.m_operation, m_left_track_to_point.toCVPoint(), m_movement_boundary, m_tracking_to_point_cv_color, 1, cv::LINE_AA);
    //cv::drawMarker(_mats.m_operation, m_left_track_to_point.toCVPoint(), m_tracking_to_point_cv_color,cv::MARKER_DIAMOND, 20, 1, cv::LINE_AA);
    //cv::circle(_mats.m_operation, m_right_track_to_point.toCVPoint(), m_movement_boundary, m_tracking_to_point_cv_color, 1, cv::LINE_AA);
    //cv::drawMarker(_mats.m_operation, m_right_track_to_point.toCVPoint(), m_tracking_to_point_cv_color,cv::MARKER_DIAMOND, 20, 1, cv::LINE_AA);
    cv::drawMarker(_mats.m_operation, m_track_to_line_center_point.toCVPoint(), m_tracking_to_point_cv_color,cv::MARKER_CROSS, 40, 1, cv::LINE_AA);
    cv::circle(_mats.m_operation, m_track_to_line_center_point.toCVPoint(), m_movement_boundary, m_tracking_to_point_cv_color, 1, cv::LINE_AA);
}

void TBIWeld_VGrooveTrackingContainer::drawTrackToPointstoMat_HeightOnly(TBIClass_OpenCVMatContainer &_mats)
{
    cv::Point _p1(m_track_to_line_center_point.m_x-150, m_track_to_line_center_point.m_y);
    cv::Point _p2(m_track_to_line_center_point.m_x+150, m_track_to_line_center_point.m_y);

    cv::Point _p3(m_track_to_line_center_point.m_x-70, m_track_to_line_center_point.m_y+m_movement_boundary);
    cv::Point _p4(m_track_to_line_center_point.m_x+70, m_track_to_line_center_point.m_y+m_movement_boundary);

    cv::Point _p5(m_track_to_line_center_point.m_x-70, m_track_to_line_center_point.m_y-m_movement_boundary);
    cv::Point _p6(m_track_to_line_center_point.m_x+70, m_track_to_line_center_point.m_y-m_movement_boundary);

    cv::line(_mats.m_operation, _p1, _p2, m_tracking_to_point_cv_color, 1, cv::LINE_AA, 0);
    cv::line(_mats.m_operation, _p3, _p4, m_tracking_to_point_cv_color, 1, cv::LINE_AA, 0);
    cv::line(_mats.m_operation, _p5, _p6, m_tracking_to_point_cv_color, 1, cv::LINE_AA, 0);
    cv::circle(_mats.m_operation, m_track_to_line_center_point.toCVPoint(), m_movement_boundary, m_tracking_to_point_cv_color, 1, cv::LINE_AA);
}

void TBIWeld_VGrooveTrackingContainer::drawJointDefinitionBoundaryToMat(TBIClass_OpenCVMatContainer &_mats)
{
    TBILine _line1o(m_left_outside_joint_def, m_right_outside_joint_def);
    TBILine _line2o(m_left_outside_joint_def, m_root_outside_joint_def);
    TBILine _line3o(m_right_outside_joint_def, m_root_outside_joint_def);
    TBILine _line1i(m_left_inside_joint_def, m_right_inside_joint_def);
    TBILine _line2i(m_left_inside_joint_def, m_root_inside_joint_def);
    TBILine _line3i (m_right_inside_joint_def, m_root_inside_joint_def);

    _line1o.drawOnMat(_mats.m_operation, m_boundary_fill_cv_color);
    _line2o.drawOnMat(_mats.m_operation, m_boundary_fill_cv_color);
    _line3o.drawOnMat(_mats.m_operation, m_boundary_fill_cv_color);
    _line1i.drawOnMat(_mats.m_operation, m_boundary_fill_cv_color);
    _line2i.drawOnMat(_mats.m_operation, m_boundary_fill_cv_color);
    _line3i.drawOnMat(_mats.m_operation, m_boundary_fill_cv_color);

}

float TBIWeld_VGrooveTrackingContainer::areaofThreeVertexTriangle(TBIPoint_Float _vertex1, TBIPoint_Float _vertex2, TBIPoint_Float _vertex3)
{
    //abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0)

    return abs((_vertex1.m_x*(_vertex2.m_y-_vertex3.m_y) + _vertex2.m_x*(_vertex3.m_y-_vertex1.m_y) + _vertex3.m_x*(_vertex1.m_y-_vertex2.m_y))/2);
}

bool TBIWeld_VGrooveTrackingContainer::isPointInsideThreeVertexTriangle(TBIPoint_Float _vertex1, TBIPoint_Float _vertex2, TBIPoint_Float _vertex3, TBIPoint_Float _point)
{
    float _areaoftriangle = this->areaofThreeVertexTriangle(_vertex1,_vertex2,_vertex3);
    float _a1 = this->areaofThreeVertexTriangle(_point, _vertex1, _vertex2);
    float _a2 = this->areaofThreeVertexTriangle(_point, _vertex2, _vertex3);
    float _a3 = this->areaofThreeVertexTriangle(_point, _vertex1, _vertex3);

    float _diff = abs(_areaoftriangle - (_a1 + _a2 + _a3));
    if(m_showdebug) qDebug() << "TBIWeld_VGrooveTrackingContainer::isPointInsideThreeVertexTriangle: _diff = " << _diff;
    if(_diff < .01) return true;
    return false;
}

void TBIWeld_VGrooveTrackingContainer::onTrackingStabilityTimerTimeout()
{
    m_tracking_stable = true;
}

void TBIWeld_VGrooveTrackingContainer::CalcJointDefinitionBoundaryPoints()
{
    //This All Happens in Pixel Space.
    //Remember Y Value is Negative

    //Set The Outside and Inside Centroids To Be Equal
    m_joint_outside_centroid_def = m_joint_centroid_def;
    m_joint_inside_centroid_def = m_joint_centroid_def;

    TBIPoint_Float _dir_vect;
    TBIPoint_Float _unit_vect;
    float _magnitude;

    //m_left_outside_joint_def and
    _dir_vect = m_left_joint_def - m_joint_centroid_def;
    _magnitude = _dir_vect.vectorMagnitude(); //Get Direction Vector Magnitude
    _unit_vect = _dir_vect/_magnitude;
    _magnitude += m_joint_def_boundary;
    _unit_vect = _unit_vect * _magnitude;
    m_left_outside_joint_def.m_x = m_joint_centroid.m_x + _unit_vect.m_x;
    m_left_outside_joint_def.m_y = m_joint_centroid.m_y + _unit_vect.m_y;

    //m_right_outside_joint_def
    _dir_vect = m_right_joint_def - m_joint_centroid_def; //Get Direction Vector Of the Correct Definition
    _magnitude = _dir_vect.vectorMagnitude(); //Get Direction Vector Magnitude
    _unit_vect = _dir_vect/_magnitude; //Get Unit Vector of the Direction Vector.
    _magnitude += m_joint_def_boundary;
    _unit_vect = _unit_vect * _magnitude;
    m_right_outside_joint_def.m_x = m_joint_centroid.m_x + _unit_vect.m_x;
    m_right_outside_joint_def.m_y = m_joint_centroid.m_y + _unit_vect.m_y;

    //m_root_outside_joint_def
    _dir_vect = m_root_joint_def - m_joint_centroid_def; //Get Direction Vector Of the Correct Definition
    _magnitude = _dir_vect.vectorMagnitude(); //Get Direction Vector Magnitude
    _unit_vect = _dir_vect/_magnitude; //Get Unit Vector of the Direction Vector.
    _magnitude += m_joint_def_boundary;
    _unit_vect = _unit_vect * _magnitude;
    m_root_outside_joint_def.m_x = m_joint_centroid.m_x + _unit_vect.m_x;
    m_root_outside_joint_def.m_y = m_joint_centroid.m_y + _unit_vect.m_y;

    //m_left_inside_joint_def and
    _dir_vect = m_left_joint_def - m_joint_centroid_def; //Get Direction Vector Of the Correct Definition
    _magnitude = _dir_vect.vectorMagnitude(); //Get Direction Vector Magnitude
    _unit_vect = _dir_vect/_magnitude; //Get Unit Vector of the Direction Vector.
    _magnitude -= m_joint_def_boundary;
    _unit_vect = _unit_vect * _magnitude;
    m_left_inside_joint_def.m_x = m_joint_centroid.m_x + _unit_vect.m_x; //Transform
    m_left_inside_joint_def.m_y = m_joint_centroid.m_y + _unit_vect.m_y;

    //m_right_inside_joint_def
    _dir_vect = m_right_joint_def - m_joint_centroid_def;//Get Direction Vector Of the Correct Definition
    _magnitude = _dir_vect.vectorMagnitude(); //Get Direction Vector Magnitude
    _unit_vect = _dir_vect/_magnitude; //Get Unit Vector of the Direction Vector.
    _magnitude -= m_joint_def_boundary;
    _unit_vect = _unit_vect * _magnitude;
    m_right_inside_joint_def.m_x = m_joint_centroid.m_x + _unit_vect.m_x;
    m_right_inside_joint_def.m_y = m_joint_centroid.m_y + _unit_vect.m_y;

    //m_root_inside_joint_def
    _dir_vect = m_root_joint_def - m_joint_centroid_def; //Get Direction Vector Of the Correct Definition
    _magnitude = _dir_vect.vectorMagnitude(); //Get Direction Vector Magnitude
    _unit_vect = _dir_vect/_magnitude; //Get Unit Vector of the Direction Vector.
    _magnitude -= m_joint_def_boundary;
    _unit_vect = _unit_vect * _magnitude;
    m_root_inside_joint_def.m_x = m_joint_centroid.m_x + _unit_vect.m_x;
    m_root_inside_joint_def.m_y = m_joint_centroid.m_y + _unit_vect.m_y;


}

TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t TBIWeld_VGrooveTrackingContainer::CheckJointBoundry()
{


    //If Left Point Is Outside the Outer Fail It.
    if(!this->isPointInsideThreeVertexTriangle(m_left_outside_joint_def, m_right_outside_joint_def, m_root_outside_joint_def, m_left_tracking_point))
    {
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_SCANNEDVGROOVEJOINTFAILEDBOUNDRYCHECK;
    }
    //If Left Point is Inside the Inner Fail It
    if(this->isPointInsideThreeVertexTriangle(m_left_inside_joint_def, m_right_inside_joint_def, m_root_inside_joint_def, m_left_tracking_point))
    {
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_SCANNEDVGROOVEJOINTFAILEDBOUNDRYCHECK;
    }
    //Is Right is Outside the Outer Fail It
    if(!this->isPointInsideThreeVertexTriangle(m_left_outside_joint_def, m_right_outside_joint_def, m_root_outside_joint_def, m_right_tracking_point))
    {
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_SCANNEDVGROOVEJOINTFAILEDBOUNDRYCHECK;
    }
    //If Right is Inside the Inner Fail It.
    if(this->isPointInsideThreeVertexTriangle(m_left_inside_joint_def, m_right_inside_joint_def, m_root_inside_joint_def, m_right_tracking_point))
    {
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_SCANNEDVGROOVEJOINTFAILEDBOUNDRYCHECK;
    }
    return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK;
}


