#include "tbiweld_base.h"


TBIWeld_Base::TBIWeld_Base(QObject *parent) : QObject(parent)
{
    //Instantiate DataSets
    m_gausian_decluster_ds = new TBIDataSet();
    m_filtered_gausian_decluster_ds = new TBIDataSet();
    //Instatiate Distrobution Set
    m_gausian_decluster_distro = new TBIDataDistributionSet();

}

TBIWeld_Base::~TBIWeld_Base()
{
    delete m_gausian_decluster_ds;
    delete m_filtered_gausian_decluster_ds;
    delete m_gausian_decluster_distro;
}

int TBIWeld_Base::getVGrooveBreakIndex()
{
    return m_gausian_decluster_ds->getIndexofHighestY(*m_gausian_decluster_distro);
}

void TBIWeld_Base::drawVGrooveBreakPointIndex(TBIClass_OpenCVMatContainer &_mats, int _breakindex)
{
    cv::drawMarker(_mats.m_ransac, m_gausian_decluster_ds->getPoint(_breakindex).toCVPoint(), CV_RGB(255,125,125), cv::MARKER_CROSS, 20);
}



TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t TBIWeld_Base::getGausianDeClusterSubSet(TBIDataSet &_dst, int _startindex, int _endindex)
{
    TBIDataSetReturnType _result;

    _result = m_gausian_decluster_ds->extractDataSubSet(_dst, _startindex, _endindex);
    if(_result != TBIDataSetReturnType::Ok)
    {
        _dst.clear();
        if(m_showdebug) qDebug("TBIWeld_Base::getGausianDeClusterSubSet() Failed To Extract Gausian DeCluster Subset.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOGETGAUSIANDECLUSTERSUBSET;
    }
    return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK;
}

TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t TBIWeld_Base::extractVGrooveJointDataSet(TBIDataSet &_dst, const TBILine &_lefttsl, const TBILine &_righttsl, const float _inlierdistancethreshold)
{
    TBIDataSetReturnType _result;
    _result = m_gausian_decluster_ds->extractVGrooveJointDataSet(_dst, _lefttsl, _righttsl, _inlierdistancethreshold);
    if(_result != TBIDataSetReturnType::Ok)
    {
        _dst.clear();
        if(m_showdebug) qDebug("TBIWeld_Base::extractVGrooveJointDataSet() Failed To Extract V Groove Joint DataSet.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOEXTRACTVGROOVEJOINTDATASET;
    }
    return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK;
}

void TBIWeld_Base::saveGausianDeClusterParamsToFile(QDataStream &_filedatastream)
{
    m_gausiandecluster_params.saveToFile(_filedatastream);
}

void TBIWeld_Base::loadGausianDeClusterParamsFromFile(QDataStream &_filedatastream)
{
    m_gausiandecluster_params.loadFromFile(_filedatastream);
}


//The Mats Must Be Clear and Ready To Use.
TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t TBIWeld_Base::doDeGausianClustering(TBIClass_OpenCVMatContainer &_mats)
{

    //Make Sure Mats Are Ok.
    if(_mats.m_raw.channels() != 1)
    {
        if(m_showdebug) qDebug()<<"TBIWeld_Base::doDeGausianClustering() did not recieve a single Channel Mat";
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_RAWMATCHANNELNOTEQUALTOONE;
    }
    if(!_mats.m_raw.isContinuous())
    {
        if(m_showdebug) qDebug()<<"TBIWeld_Base::doDeGausianClustering() did not recieve a continuous Mat";
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_RAWMATCHANNELNOTCONTINOUS;
    }

    //Clear DataSets
    m_gausian_decluster_ds->clear();
    m_filtered_gausian_decluster_ds->clear();
    m_filtered_gausian_decluster_ds->clear();

    //Draw Raw Mat onto Operations Mat
    cv::cvtColor(_mats.m_raw, _mats.m_operation, cv::COLOR_GRAY2BGR);

    //Do OpenCV Proccesses
    cv::GaussianBlur(_mats.m_raw, _mats.m_blurr, cv::Size(m_gausiandecluster_params.getBlurValue(), m_gausiandecluster_params.getBlurValue()), 0);
    cv::threshold(_mats.m_blurr, _mats.m_threshold, m_gausiandecluster_params.getMinThresholdValue(), m_gausiandecluster_params.getMaxThresholdValue(), cv::THRESH_TOZERO);

    //build DeCluster Data Set and Respond to failures.
    TBIDataSetReturnType _retresult = m_gausian_decluster_ds->buildGausianClusterDataSet(_mats.m_threshold, m_gausiandecluster_params);
    switch(_retresult)
    {
        case TBIDataSetReturnType::FailedTotalImageIntensityTooHigh:
            if(m_showdebug) qDebug()<<"TBIWeld_Base::doDeGausianClustering() Failed To Build Gausian Decluster Set. Total Image Intensity To High";
            return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_TOTALIMAGEINTENSITYTOHIGH;
            break;
        case TBIDataSetReturnType::FailedTotalImageIntensityTooLow:
            if(m_showdebug) qDebug()<<"TBIWeld_Base::doDeGausianClustering() Failed To Build Gausian Decluster Set. Total Image Intensity To Low";
            return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_TOTALIMAGEINTENSITYTOLOW;
            break;
        default:
            break;
    }

    //Draw Decluster Set to Mat
    m_gausian_decluster_ds->drawToMat(_mats.m_gausiandecluster);

    //Build Decluster Distrobution Set
    _retresult = m_gausian_decluster_ds->extractDistributionSet(*m_gausian_decluster_distro);
    if(_retresult != TBIDataSetReturnType::Ok)
    {
        if(m_showdebug) qDebug()<<"TBIWeld_Base::doDeGausianClustering() Failed To Build Gausian Decluster Distrobution Set.";
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOBUILDDECLUSTERDISTROBUTIONSET;
    }

    //Make Sure Standard Deviation of Distrobution Set is ok.
    if(m_gausian_decluster_distro->standardDeviation() > m_gausiandecluster_params.getDeclusterDeviation())
    {
        if(m_showdebug) qDebug()<<"TBIWeld_Base::doDeGausianClustering() Gausian Decluster Distrobution Standard Deviation Exceeds Allowed Value.";
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_GAUSIANDISTROBUTIONDATASETEXCEEDSALLOWEDSTANDARDDEVIATION;
    }

    //Draw The DeClusterSet Mat to the Inliers and Geo Construction and ransac
    cv::cvtColor(_mats.m_gausiandecluster, _mats.m_inliers, cv::COLOR_GRAY2BGR);
    cv::cvtColor(_mats.m_gausiandecluster, _mats.m_geometricconstruction, cv::COLOR_GRAY2BGR);
    cv::cvtColor(_mats.m_gausiandecluster, _mats.m_ransac, cv::COLOR_GRAY2BGR);

    return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK;
}


