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

TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t TBIWeld_Base::buildBoundedLaserImage(TBIClass_OpenCVMatContainer &_mats, TBIGausianDeclusteringParameters &_params)
{
    //Make Sure Mats Are Ok.
    if(_mats.m_raw.channels() != 1)
    {
        if(m_showdebug) qDebug()<<"TBIWeld_Base::buildBoundedLaserImage() did not recieve a single Channel Mat";
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_RAWMATCHANNELNOTEQUALTOONE;
    }
    if(!_mats.m_raw.isContinuous())
    {
        if(m_showdebug) qDebug()<<"TBIWeld_Base::buildBoundedLaserImage() did not recieve a continuous Mat";
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_RAWMATCHANNELNOTCONTINOUS;
    }
    if(_mats.m_edge.channels() != 1)
    {
        if(m_showdebug) qDebug()<<"TBIWeld_Base::buildBoundedLaserImage() did not recieve a single Channel Mat";
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_RAWMATCHANNELNOTEQUALTOONE;
    }
    if(!_mats.m_edge.isContinuous())
    {
        if(m_showdebug) qDebug()<<"TBIWeld_Base::buildBoundedLaserImage() did not recieve a continuous Mat";
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_RAWMATCHANNELNOTCONTINOUS;
    }
    if(_mats.m_post_blurr .channels() != 1)
    {
        if(m_showdebug) qDebug()<<"TBIWeld_Base::buildBoundedLaserImage() did not recieve a single Channel Mat";
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_RAWMATCHANNELNOTEQUALTOONE;
    }
    if(!_mats.m_post_blurr.isContinuous())
    {
        if(m_showdebug) qDebug()<<"TBIWeld_Base::buildBoundedLaserImage() did not recieve a continuous Mat";
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_RAWMATCHANNELNOTCONTINOUS;
    }

    //Go Thru The Entire Edge Map And Look For Bounding values
    uint8_t *_data = (uint8_t *) _mats.m_edge.data;
    uint8_t *_data2 = (uint8_t *) _mats.m_post_blurr.data;
    uint8_t *_data3 = (uint8_t *) _mats.m_raw.data;
    int _dataindex;
    const int _max_x = _mats.m_edge.cols;
    const int _max_y = _mats.m_edge.rows;
    int _x = 0;
    int _y = 0;
    int _intensity;
    int _length = 0;
    bool _iscounting = false;
    int _dataindex2;
    int _y2;

    //The Edge Mat is a Binary Edge Either 255 or 0
    //Run Thru THe Edge Map And Copy The Mat From The Raw Mat to the Post Blur Mat
    do
    {
        _dataindex = (_y * _max_x) + _x;
        // _y = (_dataindex - _x)/_max_x
        _intensity = _data[_dataindex];

        if(_intensity > 0) //Hit a Wall
        {
            _iscounting = true;
            if(_length >= _params.getMinLaserBoundry() && _length <= _params.getMaxLaserBoundry())
            {
                _y2 = _y - _length;
                do
                {
                    _dataindex2 = (_y2 * _max_x) + _x;
                    _data2[_dataindex2] = _data3[_dataindex2];
                    //qDebug() << "Copied Boundry, Length = " << _length;
                    ++_y2;
                }while(_y2 <= _y);

            }
            _length = 0;
        }
        else
        {
            _data2[_dataindex] = 0;
        }


        if(_iscounting) ++_length;

        ++_y;
        if(_y == _max_y)
        {
            ++_x;
            _y = 0;
            _length = 0;
            _iscounting = false;
        }
    }while(_x < _max_x);




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
    //Do OpenCV Proccesses-------------------------------------------------------
    //1. Blurr The Raw Frame.
    //2. Erode The Raw Frame.
    //3. Edge Detect The Erroded Frame
    //4. Reconstruct The Laser Portion From The Edge Map and Blur.
    //5. Threshold The Post Blurr
    //6. Do the Declustering of the Threshohld
    cv::GaussianBlur(_mats.m_raw, _mats.m_pre_blurr, cv::Size(m_gausiandecluster_params.getPreBlurValue(), m_gausiandecluster_params.getPreBlurValue()), 0);
    cv::threshold(_mats.m_pre_blurr, _mats.m_pre_blurr, 0, 255, cv::THRESH_TOZERO);

    int _erodeiteration = 1;
    while(_erodeiteration <= this->getGausianDeclusterParametersPointer()->getErodeIterations())
    {
        if(_erodeiteration == 1) cv::erode(_mats.m_pre_blurr, _mats.m_erode, getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
        else cv::erode(_mats.m_erode, _mats.m_erode, getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
        ++_erodeiteration;
    }
    if(this->getGausianDeclusterParametersPointer()->getErodeIterations() == 0) _mats.m_erode = _mats.m_pre_blurr.clone();
    cv::Canny(_mats.m_erode, _mats.m_edge, this->getGausianDeclusterParametersPointer()->getEdgeMin(), this->getGausianDeclusterParametersPointer()->getEdgeMax());
    this->buildBoundedLaserImage(_mats, m_gausiandecluster_params);
    cv::GaussianBlur(_mats.m_post_blurr, _mats.m_post_blurr, cv::Size(m_gausiandecluster_params.getPostBlurValue(), m_gausiandecluster_params.getPostBlurValue()), 0);
    cv::threshold(_mats.m_post_blurr, _mats.m_threshold, m_gausiandecluster_params.getMinThresholdValue(), m_gausiandecluster_params.getMaxThresholdValue(), cv::THRESH_TOZERO);
    //End of Image Processing.--------------------------------------------------

    //Build DeCluster Data Set and Respond to failures.
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


