#include "tbiweld_vgroove.h"
#include "tbiclass_ransac.h"

TBIWeld_VGroove::TBIWeld_VGroove()
{
    m_left_inlier_tsl_ds = new TBIDataSet();
    m_left_inlier_bwl_ds = new TBIDataSet();
    m_right_inlier_tsl_ds = new TBIDataSet();
    m_right_inlier_bwl_ds = new TBIDataSet();
    m_gausiandecluster_leftside_ds = new TBIDataSet();
    m_gausiandecluster_rightside_ds = new TBIDataSet();
    m_joint_ds = new TBIDataSet();
    m_dummy_set1 = new TBIDataSet();
    m_dummy_set2 = new TBIDataSet();
}

TBIWeld_VGroove::~TBIWeld_VGroove()
{
    delete m_left_inlier_tsl_ds;
    delete  m_left_inlier_bwl_ds;
    delete m_right_inlier_tsl_ds;
    delete m_right_inlier_bwl_ds;
    delete m_gausiandecluster_leftside_ds;
    delete m_gausiandecluster_rightside_ds;
    delete m_joint_ds;
    delete m_dummy_set1;
    delete m_dummy_set2;
}

TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t TBIWeld_VGroove::doSeamTracking(cv::Mat &_rawmat, cv::Mat &_blurmat, cv::Mat &_thresholdmat, cv::Mat &_gausiandeclustermat, cv::Mat &_ransacmat, cv::Mat &_inliersmat, cv::Mat &_geometricconstructionmat, cv::Mat &_operationsmat)
{
    this->clearDataSets();
    this->clearRansacLines();
    this->clearGeometricEntities();
    this->clearTrackingPoints();

    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t _result;

    //Stage 1 Build Gausian DeCluster DataSet.
    _result = this->doDeGausianClustering(_rawmat, _blurmat, _thresholdmat, _gausiandeclustermat);
    if(_result != TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
    {
        return _result;
    }

    //Stage 2a Build Inlier Ransacs
    _result = this->doConstructInlierRansacs(_ransacmat, _gausiandeclustermat);
    if(_result != TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
    {
        return _result;
    }

    //Stage 2b Build Inlier DataSets

    //Stage3 Build Geometric Entities

    //Stage4 Find Tracking Points.


    return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK;
}

TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t TBIWeld_VGroove::doConstructInlierRansacs(cv::Mat &_ransacmat, cv::Mat &_gausiandeclustermat)
{
    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Copy The Gausian DeCluster Set to the Ransac Mat
    cv::cvtColor(_gausiandeclustermat, _ransacmat, cv::COLOR_GRAY2BGR);
    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Get The Break Index So the Distribution Set can be Split into two parts Left and right.
    //This helps to determine the Ransacs.
    m_break_index = this->getVGrooveBreakIndex();
    if(m_break_index == -1)
    {
        qDebug("TBIWeld_VGroove::doConstructInlierRansacs() A Valid Break Index Was Not Found.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_COULDNOTFINDVALIDBREAKINDEXFORVGROOVEDATASET;
    }
    //Draw the Break Index to the Ransac Mat
    this->drawVGrooveBreakPointIndex(_ransacmat, m_break_index);
    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Build the Left Side Gausian Distro Set
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t _result;
    _result = this->getGausianDeClusterSubSet(*m_gausiandecluster_leftside_ds, 0, m_break_index);
    if(_result != TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
    {
        qDebug("TBIWeld_VGroove::doConstructInlierRansacs() Could Not Build Left Gausian Decluster Data SubSet.");
        return _result;
    }
    //Build Right Side DeCluser SubSet
    _result = this->getGausianDeClusterSubSet(*m_gausiandecluster_rightside_ds, m_break_index, this->getGausianDeclusterDataSetMaxIndex());
    if(_result != TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
    {
        qDebug("TBIWeld_VGroove::doConstructInlierRansacs() Could Not Build Right Gausian Decluster Data SubSet.");
        return _result;
    }
    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Build The Left TSL Ransac Line.
    TBIRansac::doRansac(m_left_tsl_ransac_line, m_left_tsl_ransac_params, *m_gausiandecluster_leftside_ds);
    if(!m_left_tsl_ransac_line.isValid())
    {
        qDebug("TBIWeld_VGroove::doConstructInlierRansacs() Left TSL Ransac Is Invalid.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDUNABLETOFINDINLIERRANSACLINE;
    }
    //Draw The Left TSL Ransac
    m_left_tsl_ransac_line.remakeLine(0, _ransacmat.cols);
    m_left_tsl_ransac_line.drawOnMat(_ransacmat, CV_RGB(0,0,255));
    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Build The Right TSL Ransac Line
    TBIRansac::doRansac(m_right_tsl_ransac_line, m_right_tsl_ransac_params, *m_gausiandecluster_rightside_ds);
    if(!m_right_tsl_ransac_line.isValid())
    {
        qDebug("TBIWeld_VGroove::doConstructInlierRansacs() Right TSL Ransac Is Invalid.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDUNABLETOFINDINLIERRANSACLINE;
    }
    //Draw The Right TSL Ransac
    m_right_tsl_ransac_line.remakeLine(0, _ransacmat.cols);
    m_right_tsl_ransac_line.drawOnMat(_ransacmat, CV_RGB(255,255,0));
    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Build Joint DataSet to Make Finding the BWL Ransacs Easier
    _result = this->extractVGrooveJointDataSet(*m_joint_ds, m_left_tsl_ransac_line, m_right_tsl_ransac_line, 5.0);
    if(_result != TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
    {
        qDebug("TBIWeld_VGroove::doConstructInlierRansacs() Could Not Build VGroove Joint DataSet.");
        return _result;
    }
    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Adjust the BWL Ransac Params So That The Angle to Horizon is Adjusted To The Top Surface Lines
    TBIRansacParameter _adjusted_left_bwl_param;
    TBIRansacParameter _adjusted_right_bwl_param;
    m_left_bwl_ransac_params.makeEqual(_adjusted_left_bwl_param);
    m_right_bwl_ransac_params.makeEqual(_adjusted_right_bwl_param);
    _adjusted_left_bwl_param.setIdealAngle(m_left_bwl_ransac_params.getIdealAngle() - m_left_tsl_ransac_line.angleFromHorizontal());
    _adjusted_right_bwl_param.setIdealAngle(m_right_bwl_ransac_params.getIdealAngle() - m_right_tsl_ransac_line.angleFromHorizontal());
    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Find Left BWL Ransac Line
    TBIRansac::doRansac(m_left_bwl_ransac_line, _adjusted_left_bwl_param, *m_joint_ds);
    if(!m_left_bwl_ransac_line.isValid())
    {
        qDebug("TBIWeld_VGroove::doConstructInlierRansacs() Left BWL Ransac Is Invalid.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDUNABLETOFINDINLIERRANSACLINE;
    }
    //Draw Ransac to Mat
    m_left_bwl_ransac_line.remakeLine(0, _ransacmat.cols);
    m_left_bwl_ransac_line.drawOnMat(_ransacmat, CV_RGB(0,255,0));
    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Find Right BWL Ransac Line
    TBIRansac::doRansac(m_right_bwl_ransac_line, _adjusted_right_bwl_param, *m_joint_ds);
    if(!m_right_bwl_ransac_line.isValid())
    {
        qDebug("TBIWeld_VGroove::doConstructInlierRansacs() Right BWL Ransac Is Invalid.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDUNABLETOFINDINLIERRANSACLINE;
    }
    //Draw Ransac to Mat
    m_right_bwl_ransac_line.remakeLine(0, _ransacmat.cols);
    m_right_bwl_ransac_line.drawOnMat(_ransacmat, CV_RGB(0,255,255));

    return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK;
}

void TBIWeld_VGroove::clearDataSets()
{
    m_left_inlier_tsl_ds->clear();
    m_left_inlier_bwl_ds->clear();
    m_right_inlier_tsl_ds->clear();
    m_right_inlier_bwl_ds->clear();
    m_gausiandecluster_leftside_ds->clear();
    m_gausiandecluster_rightside_ds->clear();
    m_joint_ds->clear();
    m_dummy_set1->clear();
    m_dummy_set2->clear();
}

void TBIWeld_VGroove::clearRansacLines()
{
    m_left_bwl_ransac_line.clear();
    m_left_tsl_ransac_line.clear();
    m_right_tsl_ransac_line.clear();
    m_right_bwl_ransac_line.clear();
}

void TBIWeld_VGroove::clearGeometricEntities()
{
    m_left_tsl_geo_line.clear();
    m_left_bwl_geo_line.clear();
    m_right_tsl_geo_line.clear();
    m_right_bwl_geo_line.clear();
}

void TBIWeld_VGroove::clearTrackingPoints()
{
    m_left_tracking_point.invalidatePoint();
    m_right_tracking_point.invalidatePoint();
    m_root_tracking_point.invalidatePoint();
    m_joint_centroid.invalidatePoint();
}
