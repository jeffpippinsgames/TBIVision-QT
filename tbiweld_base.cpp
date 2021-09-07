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

void TBIWeld_Base::drawVGrooveBreakPointIndex(cv::Mat &_ransacmat, int _breakindex)
{
    cv::drawMarker(_ransacmat, m_gausian_decluster_ds->getPoint(_breakindex).toCVPoint(), CV_RGB(255,125,125), cv::MARKER_CROSS, 20);
}

TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t TBIWeld_Base::getGausianDeClusterSubSet(TBIDataSet &_dst, int _startindex, int _endindex)
{
    TBIDataSetReturnType _result;

    _result = m_gausian_decluster_ds->extractDataSubSet(_dst, _startindex, _endindex);
    if(_result != TBIDataSetReturnType::Ok)
    {
        _dst.clear();
        qDebug("TBIWeld_Base::getGausianDeClusterSubSet() Failed To Extract Gausian DeCluster Subset.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOGETGAUSIANDECLUSTERSUBSET;
    }
    return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK;
}


//The Mats Must Be Clear and Ready To Use.
TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t TBIWeld_Base::doDeGausianClustering(cv::Mat &_rawmat, cv::Mat &_blurmat, cv::Mat &_thresholdmat, cv::Mat &_declustermat)
{

    //Make Sure Mats Are Ok.
    if(_rawmat.channels() != 1)
    {
        qDebug()<<"TBIWeld_Base::doDeGausianClustering() did not recieve a single Channel Mat";
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_RAWMATCHANNELNOTEQUALTOONE;
    }
    if(!_rawmat.isContinuous())
    {
        qDebug()<<"TBIWeld_Base::doDeGausianClustering() did not recieve a continuous Mat";
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_RAWMATCHANNELNOTCONTINOUS;
    }

    //Clear DataSets
    m_gausian_decluster_ds->clear();
    m_filtered_gausian_decluster_ds->clear();
    m_filtered_gausian_decluster_ds->clear();

    //Do OpenCV Proccesses
    cv::GaussianBlur(_rawmat, _blurmat, cv::Size(m_gausiandecluster_settings.m_blur_value, m_gausiandecluster_settings.m_blur_value), 0);
    cv::threshold(_blurmat, _thresholdmat, m_gausiandecluster_settings.m_threshold_min_value, m_gausiandecluster_settings.m_threshold_max_value, cv::THRESH_TOZERO);

    //build DeCluster Data Set and Respond to failures.
    TBIDataSetReturnType _retresult = m_gausian_decluster_ds->buildGausianClusterDataSet(_thresholdmat, m_gausiandecluster_settings);
    switch(_retresult)
    {
        case TBIDataSetReturnType::FailedTotalImageIntensityTooHigh:
            qDebug()<<"TBIWeld_Base::doDeGausianClustering() Failed To Build Gausian Decluster Set. Total Image Intensity To High";
            return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_TOTALIMAGEINTENSITYTOHIGH;
            break;
        case TBIDataSetReturnType::FailedTotalImageIntensityTooLow:
            qDebug()<<"TBIWeld_Base::doDeGausianClustering() Failed To Build Gausian Decluster Set. Total Image Intensity To Low";
            return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_TOTALIMAGEINTENSITYTOLOW;
            break;
        default:
            break;
    }

    //Draw Decluster Set to Mat
    m_gausian_decluster_ds->drawToMat(_declustermat);

    //Build Decluster Distrobution Set
    _retresult = m_gausian_decluster_ds->extractDistributionSet(*m_gausian_decluster_distro);
    if(_retresult != TBIDataSetReturnType::Ok)
    {
        qDebug()<<"TBIWeld_Base::doDeGausianClustering() Failed To Build Gausian Decluster Distrobution Set.";
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOBUILDDECLUSTERDISTROBUTIONSET;
    }

    //Make Sure Standard Deviation of Distrobution Set is ok.
    if(m_gausian_decluster_distro->standardDeviation() > m_gausiandecluster_settings.m_max_decluster_distro_deviation)
    {
        qDebug()<<"TBIWeld_Base::doDeGausianClustering() Gausian Decluster Distrobution Standard Deviation Exceeds Allowed Value.";
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_GAUSIANDISTROBUTIONDATASETEXCEEDSALLOWEDSTANDARDDEVIATION;
    }

    return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK;
}


