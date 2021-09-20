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

    //Do Whichever PipeLine You Need
    //processVGroovePipeline(_mats);
    processButtJointPipeline(_mats, *m_gary->getMicroControllerStatusPacketPointer());

    //Emit the Mats
    //mats.m_testmatgrey = _mats.m_blurr.clone();
    //cv::Sobel(_mats.m_blurr, _mats.m_testmatgrey, -1, 1,0,3,3,0,cv::BORDER_CONSTANT);
    //cv::cvtColor(_mats.m_testmatgrey, _mats.m_operation, COLOR_GRAY2BGR);

    emit newOperationMatProcessed(_mats.m_operation);

    if(m_emitextramats)
    {
        emit newRawMatProcessed(_mats.m_raw);
        emit newBlurMatProcessed(_mats.m_blurr);
        emit newThresholdMatProcessed(_mats.m_threshold);
        emit newPixelColumnMatProcessed(_mats.m_gausiandecluster);
        emit newRansacMatProcessed(_mats.m_ransac);
        emit newInlierDataSetMatProcessed(_mats.m_inliers);
        emit newGeoConstructionMatProcessed(_mats.m_geometricconstruction);
    }
    //--------------------------------
    m_timeinloop = "Time in Loop: " + QString::number(m_timer.elapsed());
    emit timeInLoopChanged(m_timeinloop);
    m_in_proccesing_loop = false;
    emit processingComplete(); //Must Be Last Signal Sent

}

void Max::setRootQMLContextProperties(QQmlApplicationEngine &_engine)
{
    m_vgroove.setRootQMLContextProperties(_engine);
    m_buttjoint.setRootQMLContextProperties(_engine);
    _engine.rootContext()->setContextProperty("MotionControlParams", &m_motion_control_params);
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
}

void Max::processVGroovePipeline(TBIClass_OpenCVMatContainer &_mats, MicroControllerStatusPacket &_micro_status_packet)
{
    MicroControllerStatusPacket _stat_packet;
    _micro_status_packet.copyStatusPacketTo(_stat_packet);

    //Do the VGroove Pipeline
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t _result;
    _result = m_vgroove.processPipeline(_mats, m_vgroove_tracking_container);

    //---------------------------------------------------------------------------------
    //Deal With a Change in Control State
    if(m_attempt_to_toggle_control_state)
    {
        //Now Act According to the  Control State of Gary
        switch(_stat_packet.getControlMode())
        {
        case GaryControlMode::TBI_CONTROL_MODE_MANUAL_MODE:
            if(_result == TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
            {
                m_vgroove_tracking_container.setTrackToPoints();
                m_gary->setControlModeToFullAuto();
            }
            break;
        case GaryControlMode::TBI_CONTROL_MODE_FULLAUTO_MODE:
            m_gary->setControlModeToHeightOnly();
            break;
        case GaryControlMode::TBI_CONTROL_MODE_HEIGHTONLY:
            m_gary->setControlModeToFullAuto();
            break;
        default:
            break;

        }
        m_attempt_to_toggle_control_state = false;
    }
    //No Change To Control State is Needed. Perform The Rest of The Tracking Sequence Here
    else
    {

        //Now Act According to the  Control State of Gary
        switch(_stat_packet.getControlMode())
        {
            case GaryControlMode::TBI_CONTROL_MODE_MANUAL_MODE:
                break;
            case GaryControlMode::TBI_CONTROL_MODE_FULLAUTO_MODE:
                m_vgroove_tracking_container.drawTrackToPointstoMat(_mats);
                if(_result == TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
                {

                }
                break;
            case GaryControlMode::TBI_CONTROL_MODE_HEIGHTONLY:
                m_vgroove_tracking_container.drawTrackToPointstoMat(_mats);
                if(_result == TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
                {

                }
                break;
            default:
                break;
         }

    }
    //---------------------------------------------------------------------------------

}

void Max::processButtJointPipeline(TBIClass_OpenCVMatContainer &_mats, MicroControllerStatusPacket &_micro_status_packet)
{
    MicroControllerStatusPacket _stat_packet;


    //Do the Butt Joint Pipeline
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t _result;
    _result = m_buttjoint.processPipeline(_mats, m_buttjoint_tracking_container);

     _micro_status_packet.copyStatusPacketTo(_stat_packet);
    //---------------------------------------------------------------------------------
    //Deal With a Change in Control State
    if(m_attempt_to_toggle_control_state)
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
            break;
        default:
            break;
        }
         m_attempt_to_toggle_control_state = false;
    }
    //No Change To Control State is Needed. Perform The Rest of The Tracking Sequence Here
    else
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
    //---------------------------------------------------------------------------------

}







