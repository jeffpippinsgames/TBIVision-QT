#ifndef TBIWELD_BUTTJOINT_H
#define TBIWELD_BUTTJOINT_H

#include "tbiweld_base.h"
#include "tbiclass_line.h"


class TBIWeld_ButtJoint : public TBIWeld_Base
{
public:
    //Constructor and Destructor
    TBIWeld_ButtJoint();
    ~TBIWeld_ButtJoint();


    //Seam Tracking Methods
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t doSeamTracking();


private:

    //Private Seam Tracking Entites

    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t doConstructDataSets();
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t doConstructGeometricEntities();
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t doFindValidTrackingPoint();
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t doFindTSLRansac();
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t doBuildInlierDataSet();


    //DataSets
    TBIDataSet *m_tsl_inlier_ds;

    //Geometric Entities

    //Ransac Settings





signals:


};

#endif // TBIWELD_BUTTJOINT_H
