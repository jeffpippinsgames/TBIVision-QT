#ifndef TBIWELD_VGROOVE_H
#define TBIWELD_VGROOVE_H

#include "tbiweld_base.h"
#include "tbiclass_line.h"
#include "tbiparameterclass_ranscaparms.h"

class TBIWeld_VGroove : public TBIWeld_Base
{
public:
    //Constructor and Destructor
    TBIWeld_VGroove();
    ~TBIWeld_VGroove();


    void setRansacLineParameters(TBIRansacParameter &_left_tsl_param, TBIRansacParameter &_left_bwl_param,
                                 TBIRansacParameter &_right_tsl_param, TBIRansacParameter &_right_bwl_param);


    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t doSeamTracking(cv::Mat &_rawmat, cv::Mat &_blurmat, cv::Mat &_thresholdmat,
                                                                              cv::Mat &_gausiandeclustermat, cv::Mat &_ransacmat, cv::Mat &_inliersmat,
                                                                              cv::Mat &_geometricconstructionmat, cv::Mat &_operationsmat);

private:

    //Private Seam Tracking Methods
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t doConstructInlierRansacs(cv::Mat &_ransacmat, cv::Mat &_gausiandeclustermat);
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t doBuildInlierDataSets();
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t doConstructGeometricEntities();
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t doFindValidTrackingPoints();

    //Motion Commands
    int calcXMotion();
    int calcZMotion();

    //Misc Helper Methods
    void clearDataSets();
    void clearRansacLines();
    void clearGeometricEntities();
    void clearTrackingPoints();

    //VGroove Inlier Feature Datasets
    TBIDataSet *m_left_inlier_tsl_ds;
    TBIDataSet *m_left_inlier_bwl_ds;
    TBIDataSet *m_right_inlier_tsl_ds;
    TBIDataSet *m_right_inlier_bwl_ds;
    TBIDataSet *m_gausiandecluster_leftside_ds;
    TBIDataSet *m_gausiandecluster_rightside_ds;
    TBIDataSet *m_joint_ds;

    //Dummy Data Sets
    TBIDataSet *m_dummy_set1; //Used For Static Storage. So Other Functions Dont Have To Recreate Them in the Loops.
    TBIDataSet *m_dummy_set2; //Used For Static Storage. So Other Functions Dont Have To Recreate Them in the Loops.

    //Ransac Lines For Finding Inliers
    TBILine m_left_tsl_ransac_line;
    TBILine m_left_bwl_ransac_line;
    TBILine m_right_tsl_ransac_line;
    TBILine m_right_bwl_ransac_line;

    //Ransac Parameters
    TBIRansacParameter m_left_tsl_ransac_params;
    TBIRansacParameter m_left_bwl_ransac_params;
    TBIRansacParameter m_right_tsl_ransac_params;
    TBIRansacParameter m_right_bwl_ransac_params;

    //Geometric Entities
    TBILine m_left_tsl_geo_line;
    TBILine m_left_bwl_geo_line;
    TBILine m_right_tsl_geo_line;
    TBILine m_right_bwl_geo_line;

    //Tracking Point
    TBIPoint_Int m_left_tracking_point;
    TBIPoint_Int m_right_tracking_point;
    TBIPoint_Int m_root_tracking_point;
    TBIPoint_Int m_joint_centroid;

    //Tracking To Point
    TBIPoint_Int m_left_track_to_point;
    TBIPoint_Int m_right_track_to_point;
    TBIPoint_Int m_root_track_to_point;
    TBIPoint_Int m_joint_track_to_centroid;

    //internal control variables
    int m_break_index;
};

#endif // TBIWELD_VGROOVE_H
