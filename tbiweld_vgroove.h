#ifndef TBIWELD_VGROOVE_H
#define TBIWELD_VGROOVE_H

#include "tbiweld_base.h"
#include "tbiclass_line.h"
#include "tbiparameterclass_ranscaparms.h"
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDataStream>
#include "tbiweld_vgroovetrackingcontainer.h"
#include "tbicore_gary.h"
#include "tbiweld_enumerator.h"
#include "tbiclass_ransac.h"

class TBIWeld_VGroove : public TBIWeld_Base
{
public:
    //Constructor and Destructor
    TBIWeld_VGroove();
    ~TBIWeld_VGroove();

    void setRootQMLContextProperties(QQmlApplicationEngine &engine);


    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t processPipeline(TBIClass_OpenCVMatContainer &_mats);
    //Save File Methods
    void setDefautValues();
    void saveToFile(QDataStream &_filedatastream);
    void loadFromFile(QDataStream &_filedatastream);

public slots:

private:
    const bool m_showdebug = false;
    //Color Constants
    const cv::Scalar m_master_tsl_cv_color = CV_RGB(0,255,0);
    const cv::Scalar m_break_index_cv_color = CV_RGB(255,255,0);
    const cv::Scalar m_left_tsl_cv_color = CV_RGB(0,0,255);
    const cv::Scalar m_left_bwl_cv_color = CV_RGB(125,125,255);
    const cv::Scalar m_right_tsl_cv_color = CV_RGB(255,0,0);
    const cv::Scalar m_right_bwl_cv_color = CV_RGB(255,125,125);

    //Private Seam Tracking Methods
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t buildRansacLine(TBIDataSet *_ds, TBILine &_line, TBIRansacParameter &_params);
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t extractInlierDataSet(TBIDataSet *_src, TBIDataSet *_dst, TBILine &m_ransac, float _distthreshold);
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t extractOutlierDataSet(TBIDataSet *_srcds, TBIDataSet *_inlierds, TBIDataSet *_outlierds);




    //Clearing Methods
    void clearDataSets();
    void clearRansacLines();
    void clearGeometricEntities();

    //VGroove Inlier Feature Datasets
    TBIDataSet *m_master_inlier_tsl_ds;
    TBIDataSet *m_left_inlier_tsl_ds;
    TBIDataSet *m_right_inlier_tsl_ds;
    TBIDataSet *m_break_index_ds;


    //Dummy Data Sets
    TBIDataSet *m_dummy_set1; //Used For Static Storage. So Other Functions Dont Have To Recreate Them in the Loops.
    TBIDataSet *m_dummy_set2; //Used For Static Storage. So Other Functions Dont Have To Recreate Them in the Loops.

    //Ransac Lines For Finding Inliers
    TBILine m_master_tsl_ransac_line;
    TBILine m_break_index_ransac_line;
    TBILine m_left_tsl_ransac_line;
    TBILine m_right_tsl_ransac_line;

    //Ransac Parameters
    TBIRansacParameter m_master_tsl_ransac_params;
    TBIRansacParameter m_break_index_ransac_params;
    TBIRansacParameter m_left_tsl_ransac_params;
    TBIRansacParameter m_right_tsl_ransac_params;

    //Geometric Entities
    TBILine m_left_tsl_geo_line;
    TBILine m_right_tsl_geo_line;
    TBIPoint_Int m_break_index_point;
    TBIPoint_Int m_left_tracking_point;
    TBIPoint_Int m_right_tracking_point;

    //internal control variables
    int m_break_index;
};

#endif // TBIWELD_VGROOVE_H
