#include "tbiweld_buttjointtrackingcontainer.h"

TBIWeld_ButtJointTrackingContainer::TBIWeld_ButtJointTrackingContainer(QObject *parent) : QObject(parent)
{

}

TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t TBIWeld_ButtJointTrackingContainer::extractTrackingPoint(TBILine &_tsl, TBIClass_OpenCVMatContainer &_mats)
{
    m_tracking_point = _tsl.getMidPointofLine();

}
