#include "tbiweld_vgroove.h"

TBIWeld_VGroove::TBIWeld_VGroove()
{
    m_left_inlier_tsl_ds = new TBIDataSet();
    m_left_inlier_bwl_ds = new TBIDataSet();
    m_right_inlier_tsl_ds = new TBIDataSet();
    m_right_inlier_bwl_ds = new TBIDataSet();
    m_gausiandecluster_leftside_ds = new TBIDataSet();
    m_gausiandecluster_rightside_ds = new TBIDataSet();
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
    delete m_dummy_set1;
    delete m_dummy_set2;
}

TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t TBIWeld_VGroove::doConstructInlierRansacs(cv::Mat &_ransacmat)
{
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

    //Build the Left Side Gausian Distro Set
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t _result;
    _result = this->getGausianDeClusterSubSet(*m_gausiandecluster_leftside_ds, 0, m_break_index);
    if(_result != TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
    {
        qDebug("TBIWeld_VGroove::doConstructInlierRansacs() Could Not Build Left Gausian Decluster Data SubSet.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOBUILDVGROOVELEFTGAUSIANDECLUSTERDATASUBSET;
    }
    //Build Right Side DeCluser SubSet
    _result = this->getGausianDeClusterSubSet(*m_gausiandecluster_rightside_ds, m_break_index, this->getGausianDeclusterDataSetMaxIndex());
    if(_result != TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
    {
        qDebug("TBIWeld_VGroove::doConstructInlierRansacs() Could Not Build Right Gausian Decluster Data SubSet.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOBUILDVGROOVERIGHTGAUSIANDECLUSTERDATASUBSET;
    }

    //Fill the Left TSL Ransac Line.



    return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK;

}
