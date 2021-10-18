#include "tbicore_max.h"
#include <QDebug>
#include "tbiclass_pixelfundamental.h"
#include "tbiparameterclass_ransacvotingstruct.h"
#include <QRandomGenerator>
#include <QThread>
#include <vector>
#include <random>
#include "tbiclass_pointint.h"
#include "tbiclass_ransac.h"



//Constructors and Destructor--------------------------------------------------------
Max::Max(QObject *parent) : QObject(parent)
{

    this->setDefautValues();
    if(m_showdebug) qDebug()<<"Max::Max() Max Object Created.";
    m_joint_type.setJointType(TBIWeldType_Enumerator::TBI_VGROOVE_WELD);

    m_motion_control_params.setXStepsPerPixel(1214.37037);
    m_motion_control_params.setZStepsPerPixel(1214.37037);
    m_tracking_point_valid = false;
    m_track_to_point_valid = false;
}

Max::~Max()
{
    emit this->aboutToDestroy();
    if(m_showdebug) qDebug()<<"Max::~Max() Max Object Destroyed";
}

//The Recieve New CV::Mat Method
//This Slot Handles A New Camera Frame
void Max::recieveNewCVMat(const Mat &_mat)
{
    if(m_in_proccesing_loop) return;
    m_in_proccesing_loop = true;
    m_timer.start();
    //--------------------------------

    //Instantiate and initialize cv::Mats
    TBIClass_OpenCVMatContainer _mats;
    _mats.initMats(_mat);

    //Do Whichever PipeLine You Need. Feed the Result  Into The PipelineResult Variable.
    switch(m_joint_type.getJointType())
    {
    case TBIWeldType_Enumerator::TBI_VGROOVE_WELD:
        m_pipelineresult.setStatus(processVGroovePipeline(_mats, *m_gary->getMicroControllerStatusPacketPointer()));
        break;
    case TBIWeldType_Enumerator::TBI_BUTTJOINT_WELD:
        m_pipelineresult.setStatus(processButtJointPipeline(_mats, *m_gary->getMicroControllerStatusPacketPointer()));
        break;
    case TBIWeldType_Enumerator::TBI_FILLET_WELD:
        m_pipelineresult.setStatus(TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_SELECTEDJOINTTYPENOTIMPLEMENTED);
        break;
    }

    //Emit the Mats
    emit newOperationMatProcessed(_mats.m_operation);
    if(m_emitextramats)
    {
        emit newRawMatProcessed(_mats.m_raw);
        emit newPreBlurMatProcessed(_mats.m_pre_blurr);
        emit newPostBlurMatProcessed(_mats.m_post_blurr);
        emit newEdgeMatProcessed(_mats.m_edge);
        emit newErodeMatProcessed(_mats.m_erode);
        emit newThresholdMatProcessed(_mats.m_threshold);
        emit newPixelColumnMatProcessed(_mats.m_gausiandecluster);
        emit newRansacMatProcessed(_mats.m_ransac);
        emit newInlierDataSetMatProcessed(_mats.m_inliers);
        emit newGeoConstructionMatProcessed(_mats.m_geometricconstruction);
    }
    //--------------------------------
    m_timeinloop = "Time in Loop: " + QString::number(m_timer.elapsed());
    emit timeInLoopChanged(m_timeinloop);
    \
    //Sample The
    m_in_proccesing_loop = false;
    emit processingComplete(); //Must Be Last Signal Sent

}

void Max::setRootQMLContextProperties(QQmlApplicationEngine &_engine)
{
    m_vgroove.setRootQMLContextProperties(_engine);
    m_buttjoint.setRootQMLContextProperties(_engine);
    _engine.rootContext()->setContextProperty("MotionControlParams", &m_motion_control_params);
    _engine.rootContext()->setContextProperty("ProcessingPipeLineStatus", &m_pipelineresult);
    _engine.rootContext()->setContextProperty("WeldJointType", &m_joint_type);
}

void Max::setDefautValues()
{
    m_attempt_to_toggle_control_state = false;
    m_emitextramats = false;
    m_in_proccesing_loop = false;
    m_timeinloop = "Error:";
    m_vgroove.setDefautValues();
    m_motion_control_params.setDefautValues();

}

void Max::saveToFile(QDataStream &_filedatastream)
{
    m_vgroove.saveToFile(_filedatastream);
    m_buttjoint.saveToFile(_filedatastream);
    m_motion_control_params.saveToFile(_filedatastream);

}

void Max::loadFromFile(QDataStream &_filedatastream)
{
    m_vgroove.loadFromFile(_filedatastream);
    m_buttjoint.loadFromFile(_filedatastream);
    m_motion_control_params.loadFromFile(_filedatastream);
    m_motion_control_params.setXStepsPerPixel(1214.37037);
    m_motion_control_params.setZStepsPerPixel(1214.37037);
}

TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t Max::processVGroovePipeline(TBIClass_OpenCVMatContainer &_mats, MicroControllerStatusPacket &_micro_status_packet)
{
    m_tracking_point_valid = false;
    m_track_to_point_valid = false;

    //Copy The MicroController Status Packet.
    MicroControllerStatusPacket _stat_packet;
    _micro_status_packet.copyStatusPacketTo(_stat_packet);
    GaryControlMode::ControlMode_t _control_mode = _stat_packet.getControlMode();

    //Do the VGroove Pipeline
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t _result;

    _result = m_vgroove.processPipeline(_mats);
    if(_stat_packet.getLaserStatus()==GaryLaserStatus::TBI_LASER_STATUS_OFF) _result = TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDLASERPOWEROFF;
    if(_result == TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK) //Set Tracking Point Info
    {
            m_tracking_point_valid = true;
            m_tracking_x = (int)round(m_vgroove_tracking_container.getTrackingPoint().m_x);
            m_tracking_y = (int)round(m_vgroove_tracking_container.getTrackingPoint().m_y);
    }

    //--------------------------------------------------------------------------------
    // VGroove Specific Secondary Operations After PipeLine Has Run. Tracking Point Checks and Motor Movement
    //---------------------------------------------------------------------------------
    if(_result == TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
    {
        //Change Control State Or Process Different Control Modes.
        if(m_attempt_to_toggle_control_state) //Process Control Mode State Change
        {
            if(_result == TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
            {
                //Now Act According to the  Control State of Gary
                switch(_control_mode)
                {
                case GaryControlMode::TBI_CONTROL_MODE_MANUAL_MODE:
                    m_vgroove_tracking_container.setTrackToPoints();
                    m_gary->setControlModeToFullAuto();
                    break;
                case GaryControlMode::TBI_CONTROL_MODE_FULLAUTO_MODE:
                    m_vgroove_tracking_container.setTrackToPoints();
                    m_gary->setControlModeToHeightOnly();
                    break;
                case GaryControlMode::TBI_CONTROL_MODE_HEIGHTONLY:
                    m_gary->setControlModeToManual();
                    break;
                default:
                    break;
                }
            }
            m_attempt_to_toggle_control_state = false;
        }
        else//No Change To Control State is Needed. Perform The Rest of The Tracking Sequence Here
        {

            //Now Act According to the  Control State of Gary
            switch(_control_mode)
            {
            case GaryControlMode::TBI_CONTROL_MODE_MANUAL_MODE:
                m_vgroove_tracking_container.drawTrackingPointstoMat(_mats);
                break;
            case GaryControlMode::TBI_CONTROL_MODE_FULLAUTO_MODE:
                m_vgroove_tracking_container.drawTrackToPointstoMat_FullAuto(_mats);
                m_track_to_point_valid = true;
                m_track_to_x = (int)round(m_vgroove_tracking_container.getTrackToPoint().m_x);
                m_track_to_y = (int)round(m_vgroove_tracking_container.getTrackToPoint().m_y);
                m_vgroove_tracking_container.CalcJointDefinitionBoundaryPoints();
                _result = m_vgroove_tracking_container.CheckJointBoundry();

                if(_result == TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
                {
                    m_vgroove_tracking_container.drawJointDefinitionBoundaryToMat(_mats);
                    m_vgroove_tracking_container.drawTrackingPointstoMat(_mats);
                    if(_stat_packet.isXMotionStatusIdle())
                    {
                        int _xsteps = m_vgroove_tracking_container.getXMovement(m_motion_control_params);
                        if(_xsteps != 0) m_gary->autoMoveXAxis(_xsteps);
                    }
                    if(_stat_packet.isZMotionStatusIdle())
                    {
                        int _zsteps = m_vgroove_tracking_container.getZMovement(m_motion_control_params);
                        if(_zsteps != 0) m_gary->autoMoveZAxis(_zsteps);

                    }
                }

                break;
            case GaryControlMode::TBI_CONTROL_MODE_HEIGHTONLY:
                m_vgroove_tracking_container.drawTrackToPointstoMat_HeightOnly(_mats);
                m_track_to_point_valid = true;
                m_track_to_x = (int)round(m_vgroove_tracking_container.getTrackToPoint().m_x);
                m_track_to_y = (int)round(m_vgroove_tracking_container.getTrackToPoint().m_y);
                m_vgroove_tracking_container.CalcJointDefinitionBoundaryPoints();
                _result = m_vgroove_tracking_container.CheckJointBoundry();
                if(_result == TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
                {
                    m_vgroove_tracking_container.drawJointDefinitionBoundaryToMat(_mats);
                    m_vgroove_tracking_container.drawTrackingPointstoMat(_mats);
                    if(_stat_packet.isZMotionStatusIdle())
                    {
                        int _zsteps = m_vgroove_tracking_container.getZMovement(m_motion_control_params);
                        if(_zsteps != 0) m_gary->autoMoveZAxis(_zsteps);
                    }
                }
                break;
            default:
                break;
            }
        }
    }
    else
    {
        m_attempt_to_toggle_control_state = false;
    }

    emit trackingPointXChanged();
    emit trackingPointYChanged();
    emit validTrackingPointChanged();
    emit trackToPointXChanged();
    emit trackToPointYChanged();
    emit validTrackToPointChanged();

    //Update Tracking Info For
    return _result;
    //---------------------------------------------------------------------------------

}

TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t Max::processButtJointPipeline(TBIClass_OpenCVMatContainer &_mats, MicroControllerStatusPacket &_micro_status_packet)
{

    m_tracking_point_valid = false;
    m_track_to_point_valid = false;

    MicroControllerStatusPacket _stat_packet;
    _micro_status_packet.copyStatusPacketTo(_stat_packet);

    //Do the Butt Joint Pipeline
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t _result;
    _result = m_buttjoint.processPipeline(_mats, m_buttjoint_tracking_container);
    if(_micro_status_packet.getLaserStatus()==GaryLaserStatus::TBI_LASER_STATUS_OFF) _result = TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDLASERPOWEROFF;
    if(_result == TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK) //Set Tracking Point Info
    {
            m_tracking_point_valid = true;
            m_track_to_x = (int)round(m_vgroove_tracking_container.getTrackingPoint().m_x);
            m_track_to_y = (int)round(m_vgroove_tracking_container.getTrackingPoint().m_y);
    }

    //--------------------------------------------------------------------------------
    // Butt Joint Specific Secondary Operations After PipeLine Has Run. Tracking Point Checks and Motor Movement
    //---------------------------------------------------------------------------------
    if(_result == TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
    {
    if(m_attempt_to_toggle_control_state) //Deal With a Change in Control State
    {
        //Now Act According to the  Control State of Gary
        switch(_stat_packet.getControlMode())
        {
        case GaryControlMode::TBI_CONTROL_MODE_MANUAL_MODE:
            if(_result == TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
            {
                m_buttjoint_tracking_container.setTrackToPoints();
                m_gary->setControlModeToHeightOnly();
            }
            break;
        case GaryControlMode::TBI_CONTROL_MODE_FULLAUTO_MODE:
            break;
        case GaryControlMode::TBI_CONTROL_MODE_HEIGHTONLY:
            m_gary->setControlModeToManual();
            break;
        default:
            break;
        }
        m_attempt_to_toggle_control_state = false;
    }
    else //No Change To Control State is Needed. Perform The Rest of The Tracking Sequence Here
    {
        //Now Act According to the  Control State of Gary
        switch(_stat_packet.getControlMode())
        {
        case GaryControlMode::TBI_CONTROL_MODE_MANUAL_MODE:
            break;
        case GaryControlMode::TBI_CONTROL_MODE_FULLAUTO_MODE:
            break;
        case GaryControlMode::TBI_CONTROL_MODE_HEIGHTONLY:
            m_buttjoint_tracking_container.drawTrackToPointstoMat(_mats);
            m_track_to_point_valid = true;
            m_track_to_x = (int)round(m_vgroove_tracking_container.getTrackToPoint().m_x);
            m_track_to_y = (int)round(m_vgroove_tracking_container.getTrackToPoint().m_y);
            if((_result == TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK) && _stat_packet.isZMotionStatusIdle())
            {
                //Z Up Increases the Step Location
                //Which Means Z Must Add Positive Steps
                //A Positive _zdiff Means Move Down
                //So Send Negative Steps
                //m_motion_control_params.setZStepsPerPixel(1214.37037);

                int _zdiff = m_buttjoint_tracking_container.getZTrackingDifference();
                _zdiff = _zdiff * 1214.37037;
                if(_zdiff != 0)
                {
                    m_gary->autoMoveZAxis(-_zdiff);

                }
            }
            break;
        default:
            break;
        }
    }
    }
    else
    {
        m_attempt_to_toggle_control_state = false;
    }
    emit trackingPointXChanged();
    emit trackingPointYChanged();
    emit validTrackingPointChanged();
    emit trackToPointXChanged();
    emit trackToPointYChanged();
    emit validTrackToPointChanged();

    //---------------------------------------------------------------------------------
    return _result;
}







