#include "tbidataset.h"

void TBIDataSet::drawToMat(cv::Mat &_dst)
{
    if(_dst.type() != CV_8UC1) return;
    if(m_dataset_size == 0) return;

    uint8_t *_data = (uint8_t*)_dst.data;
    const int _dstrows = _dst.rows;
    const int _dstcols = _dst.cols;
    int _dataindex=0;
    int _pntsindex=0;

    do
    {
        if((m_pnts[_pntsindex].m_x < _dstcols) && (m_pnts[_pntsindex].m_y < _dstrows))
        {
            _dataindex = (m_pnts[_pntsindex].m_y * _dstcols) + m_pnts[_pntsindex].m_x;
            _data[_dataindex] = 255;
        }
        ++_pntsindex;
    }while(_pntsindex < m_dataset_size);
}

void TBIDataSet::drawToMat(cv::Mat &_dst, cv::Scalar _color)
{
    if(_dst.type() != CV_8UC3) return;
    if(m_dataset_size == 0) return;

    uint8_t * _matdata = _dst.data;
    const int _matchannels = _dst.channels();
    int _pntindex = 0;
    int _matindexc1 = 0;
    int _matindexc2 = 0;
    int _matindexc3 = 0;

    //value = pImgData[img.channels() * (img.cols * row + column) + channel];
    do
    {
        _matindexc1 = _matchannels * (_dst.cols * m_pnts[_pntindex].m_y + m_pnts[_pntindex].m_x) + 0;
        _matindexc2 = _matchannels * (_dst.cols * m_pnts[_pntindex].m_y + m_pnts[_pntindex].m_x) + 1;
        _matindexc3 = _matchannels * (_dst.cols * m_pnts[_pntindex].m_y + m_pnts[_pntindex].m_x) + 2;
        _matdata[_matindexc1] = _color[2];
        _matdata[_matindexc2] = _color[1];
        _matdata[_matindexc3] = _color[0];
        ++_pntindex;
    }while(_pntindex < m_dataset_size);

}

void TBIDataSet::eraseFromMat(cv::Mat &_dst)
{
    if(m_dataset_size == 0) return;

    if(_dst.type() == CV_8UC1)
    {
        uint8_t *_data = (uint8_t*)_dst.data;
        const int _dstrows = _dst.rows;
        const int _dstcols = _dst.cols;
        int _dataindex=0;
        int _pntsindex=0;

        do
        {
            if((m_pnts[_pntsindex].m_x < _dstcols) && (m_pnts[_pntsindex].m_y < _dstrows))
            {
                _dataindex = (m_pnts[_pntsindex].m_y * _dstcols) + m_pnts[_pntsindex].m_x;
                _data[_dataindex] = 0;
            }
            ++_pntsindex;
        }while(_pntsindex < m_dataset_size);
    }
    if(_dst.type() == CV_8UC3)
    {
        uint8_t * _matdata = _dst.data;
        const int _matchannels = _dst.channels();
        int _pntindex = 0;
        int _matindexc1 = 0;
        int _matindexc2 = 0;
        int _matindexc3 = 0;

        //value = pImgData[img.channels() * (img.cols * row + column) + channel];
        do
        {
            _matindexc1 = _matchannels * (_dst.cols * m_pnts[_pntindex].m_y + m_pnts[_pntindex].m_x) + 0;
            _matindexc2 = _matchannels * (_dst.cols * m_pnts[_pntindex].m_y + m_pnts[_pntindex].m_x) + 1;
            _matindexc3 = _matchannels * (_dst.cols * m_pnts[_pntindex].m_y + m_pnts[_pntindex].m_x) + 2;
            _matdata[_matindexc1] = 0;
            _matdata[_matindexc2] = 0;
            _matdata[_matindexc3] = 0;
            ++_pntindex;
        }while(_pntindex < m_dataset_size);
    }
}

void TBIDataSet::buildDataSetForLeftTSL(TBIDataSet &_dst, unsigned int _uniquexvalues)
{
    _dst.clear();
    if(m_dataset_size == 0) return;

    unsigned int _uniquex = 0;
    int _lastxvalue;
    int _pntindex = 0;

    _dst.insert(m_pnts[_pntindex]);
    _lastxvalue = m_pnts[_pntindex].m_x;
    _uniquex = 1;
    ++_pntindex;
    if(m_dataset_size == 1) return;

    do
    {
        _dst.insert(m_pnts[_pntindex]);
        if(_lastxvalue != m_pnts[_pntindex].m_x)
        {
            _lastxvalue = m_pnts[_pntindex].m_x;
            ++_uniquex;
        }

        ++_pntindex;
    }while((_pntindex < m_dataset_size) && (_uniquex <= _uniquexvalues));

}

void TBIDataSet::buildDataSetForRightTSL(TBIDataSet &_dst, unsigned int _uniquexvalues)
{
    //Note DataSet For Right TSL is Backwards.
    _dst.clear();
    if(m_dataset_size == 0) return;

    unsigned int _uniquex = 0;
    int _lastxvalue;
    int _pntindex = m_dataset_size-1;

    _dst.insert(m_pnts[_pntindex]);
    _lastxvalue = m_pnts[_pntindex].m_x;
    _uniquex = 1;
    --_pntindex;
    if(m_dataset_size == 1) return;

    do
    {
        _dst.insert(m_pnts[_pntindex]);
        if(_lastxvalue != m_pnts[_pntindex].m_x)
        {
            _lastxvalue = m_pnts[_pntindex].m_x;
            ++_uniquex;
        }

        --_pntindex;
    }while((_pntindex >= 0) && (_uniquex <= _uniquexvalues));

}

void TBIDataSet::buildDataSetForJoint(TBIDataSet &_dst, const TBILine &_lefttsl, const TBILine &_righttsl, const float _inlierdistancethreshold)
{
    _dst.clear();
    if(m_dataset_size == 0) return;
    if(!_lefttsl.isValid()) return;
    if(!_righttsl.isValid()) return;

    int _index = 0;
    float _distanceleft;
    float _distanceright;

    do
    {
        _distanceleft = _lefttsl.distanceAbs(m_pnts[_index]);
        _distanceright = _righttsl.distanceAbs(m_pnts[_index]);
        if((_distanceleft > _inlierdistancethreshold) && (_distanceright > _inlierdistancethreshold))
        {
            _dst.insert(m_pnts[_index]);
        }
        ++_index;
    }while(_index < m_dataset_size);
}



void TBIDataSet::buildDataSetForInliers(TBIDataSet &_dst, const TBILine &_line, const float _distancethreshold)
{
    _dst.clear();
    if(m_dataset_size == 0) return;
    if(!_line.isValid()) return;
    int _index = 0;
    float _distance;

    do
    {
        _distance = _line.distanceAbs(m_pnts[_index]);
        if(_distance <= _distancethreshold)
        {
            _dst.insert(m_pnts[_index]);
        }
        ++_index;
    }while(_index < m_dataset_size);

}

void TBIDataSet::buildLeastSquareLine(TBILine &_line)
{
    _line.clear();
    if(m_dataset_size == 0) return;
    float _xmean = 0;
    float _ymean = 0;
    float _slope;
    float _intercept;
    int _index = 0;
    float _xyminusmeansum = 0;
    float _xminusmeansquaresum = 0;

    do
    {
        _xmean += (float)m_pnts[_index].m_x;
        _ymean += (float)m_pnts[_index].m_y;
        ++_index;
    }while(_index < m_dataset_size);

    _xmean = _xmean/(float)m_dataset_size;
    _ymean = _ymean/(float)m_dataset_size;

    _index = 0;
    do
    {
        int _xminusmean = (float)m_pnts[_index].m_x - _xmean;
        int _yminusmean = (float)m_pnts[_index].m_y - _ymean;
        _xyminusmeansum += _xminusmean * _yminusmean;
        _xminusmeansquaresum += _xminusmean * _xminusmean;
        ++_index;
    }while(_index < m_dataset_size);

    if(_xminusmeansquaresum != 0)
    {
        _slope = _xyminusmeansum/_xminusmeansquaresum;
        _intercept = _ymean - (_xmean*_slope);
        _line = TBILine(0.0, _intercept, 720.0, (_slope*720.0 + _intercept));
    }


}
