#ifndef TBIWELD_BUTTJOINT_H
#define TBIWELD_BUTTJOINT_H

#include "tbiweld_base.h"
#include "tbiclass_line.h"
#include "tbiweld_buttjointtrackingcontainer.h"
#include "tbiparameterclass_ranscaparms.h"
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "tbiclass_ransac.h"


class TBIWeld_ButtJoint : public TBIWeld_Base
{
public:
    //Constructor and Destructor
    TBIWeld_ButtJoint();
    ~TBIWeld_ButtJoint();

    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t processPipeline(TBIClass_OpenCVMatContainer &_mats, TBIWeld_ButtJointTrackingContainer &_tracking_container);
    void setRootQMLContextProperties(QQmlApplicationEngine &_engine);

    //Save File Methods
    void setDefautValues();
    void saveToFile(QDataStream &_filedatastream);
    void loadFromFile(QDataStream &_filedatastream);


private:

    //Clearing Methods
    void clearDataSets();
    void clearRansacLines();
    void clearGeometricEntities();

    //Private Seam Tracking Entites
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t doConstructGeometricEntities(TBIClass_OpenCVMatContainer &_mats);
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t doConstructInlierRansacs(TBIClass_OpenCVMatContainer &_mats);
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t doBuildInlierDataSets(TBIClass_OpenCVMatContainer &_mats);

    TBIDataSet *m_tsl_inlier_ds;
    TBIRansacParameter m_tsl_ransac_params;
    TBILine m_tsl_ransac_line;
    TBILine m_tsl_geo_line;

    const cv::Scalar m_left_tsl_cv_color = CV_RGB(0,0,255);

signals:


};

#endif // TBIWELD_BUTTJOINT_H
