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
    qDebug()<<"Max::Max() Max Object Created.";
}

Max::~Max()
{
    emit this->aboutToDestroy();
    qDebug()<<"Max::~Max() Max Object Destroyed";
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
    //Do the VGroove Pipeline
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t _result;
    _result = m_vgroove.processPipeline(_mats);

    //Emit the Mats
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
    _engine.rootContext()->setContextProperty("MotionControlParams", &m_motion_control_params);
}

void Max::setDefautValues()
{
    m_emitextramats = false;
    m_in_proccesing_loop = false;
    m_timeinloop = "Error:";
    m_vgroove.setDefautValues();
    m_motion_control_params.setDefautValues();
}

void Max::saveToFile(QDataStream &_filedatastream)
{
    m_vgroove.saveToFile(_filedatastream);
    m_motion_control_params.saveToFile(_filedatastream);
}

void Max::loadFromFile(QDataStream &_filedatastream)
{
    m_vgroove.loadFromFile(_filedatastream);
    m_motion_control_params.loadFromFile(_filedatastream);
}







