#include "tbiclass_dataset.h"
#include "tbiclass_pixelfundamental.h"
#include <iostream>
using namespace std;

//Utility Functions
int TBIDataSet::getIndexofFirstXValue(int _xval)
{
    if(this->size() == 0) return -1;
    if(_xval < 0) return -1;

    int _index = 0;

    do
    {
        if(this->m_pnts[_index].m_x == _xval)
        {
            return _index;
        }
        ++_index;

    }while(_index < this->size());

    return -1;
}

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

int TBIDataSet::getIndexofHighestY()
{
    if(m_dataset_size==0) return -1;

    int _index=0;
    int _highestyvalue=0;
    int _highestyindex=-1;

    do
    {

        if(m_pnts[_index].m_y > _highestyvalue)
        {
            _highestyvalue = m_pnts[_index].m_y;
            _highestyindex = _index;
        }
        ++_index;
    }while(_index < m_dataset_size);

    return _highestyindex;

}

int TBIDataSet::getIndexofHighestY(const TBIDataDistributionSet &_distroset)
{
    if(m_dataset_size==0) return -1;

    int _index=0;
    int _highestyvalue=0;
    int _highestyindex=-1;

    do
    {

        if((m_pnts[_index].m_y > _highestyvalue) && (_distroset[m_pnts[_index].m_x] == 1))
        {
            _highestyvalue = m_pnts[_index].m_y;
            _highestyindex = _index;
        }
        ++_index;
    }while(_index < m_dataset_size);

    return _highestyindex;
}

int TBIDataSet::getIndexofLowestX()
{
    if(m_dataset_size==0) return -1;

    int _index=0;
    int _lowestvalue=TBIConstants::Max_Camera_Width;
    int _lowestindex=-1;

    do
    {

        if(m_pnts[_index].m_x < _lowestvalue)
        {
            _lowestvalue = m_pnts[_index].m_x;
            _lowestindex = _index;
        }
        ++_index;
    }while(_index < m_dataset_size);

    return _lowestindex;
}

int TBIDataSet::getIndexofHighestX()
{
    if(m_dataset_size==0) return -1;

    int _index=0;
    int _highestvalue=0;
    int _highestindex=-1;

    do
    {

        if(m_pnts[_index].m_x > _highestvalue)
        {
            _highestvalue = m_pnts[_index].m_x;
            _highestindex = _index;
        }
        ++_index;
    }while(_index < m_dataset_size);

    return _highestindex;
}

int TBIDataSet::getHighestX()
{
    if(m_dataset_size==0) return -1;

    int _index=0;
    int _highestxvalue=-1;
    do
    {

        if(m_pnts[_index].m_x > _highestxvalue)
        {
            _highestxvalue = m_pnts[_index].m_x;
        }
        ++_index;
    }while(_index < m_dataset_size);

    return _highestxvalue;
}

int TBIDataSet::getLowestX()
{
    if(m_dataset_size==0) return -1;

    int _index=0;
    int _lowestxvalue=TBIConstants::Max_Camera_Width;
    do
    {

        if(m_pnts[_index].m_x < _lowestxvalue)
        {
            _lowestxvalue = m_pnts[_index].m_x;
        }
        ++_index;
    }while(_index < m_dataset_size);

    return _lowestxvalue;
}

void TBIDataSet::remove(int _index)
{
    if(m_dataset_size == 0) return;
    if(_index >= Largest_DataSet_Size) return;
    if(_index >= m_dataset_size) return;
    if(_index < 0 ) return;
    int __index = _index;

    if(_index == (m_dataset_size - 1))
    {
        --m_dataset_size;
        return;
    }

    do
    {
        m_pnts[__index].m_x = m_pnts[__index+1].m_x;
        m_pnts[__index].m_y = m_pnts[__index+1].m_y;
        ++__index;
    }while(__index < m_dataset_size);
    --m_dataset_size;
}

void TBIDataSet::removeBlock(int _startindex, int _endindex)
{
    if(m_dataset_size == 0) return;
    if(_startindex >= _endindex) return;
    if(_endindex >= Largest_DataSet_Size) return;
    if(_endindex >= m_dataset_size) return;
    if(_startindex < 0) return;

    if(_endindex == (m_dataset_size - 1))
    {
        m_dataset_size -= (_endindex - _startindex + 1);
        return;
    }

    int _indexoffset = _endindex - _startindex + 1;
    int _index = _startindex;

    do
    {
        m_pnts[_index].m_x = m_pnts[_index+_indexoffset].m_x;
        m_pnts[_index].m_y = m_pnts[_index+_indexoffset].m_y;
        ++_index;
    }while((_index + _indexoffset) < m_dataset_size);
    m_dataset_size -= (_indexoffset);
}

TBIDataSetReturnType TBIDataSet::buildGausianClusterDataSet(cv::Mat &_thresholdmat, TBIGausianDeclusteringParameters &_declusterparameter)
{

    this->clear();
    _declusterparameter.clearTotalImageIntensity();

    if(_thresholdmat.type() != CV_8UC1)
    {
        return TBIDataSetReturnType::FailedFunctionPassedInWrongMatType;
    }

    uint8_t *_data = (uint8_t *) _thresholdmat.data;
    int _dataindex;
    const int _max_x = _thresholdmat.cols;
    const int _max_y = _thresholdmat.rows;
    int _x = 0;
    int _y = 0;
    int _intensity;
    quint64 _tii = 0;

    TBIPixelFundamental _pixels[_max_y];
    int _pixelarraysize=0;
    int _pixelarrayindex = -1; //start out of bounds. Initialization of Pixel Index must be -1
    bool _inintensifingphase = true;
    bool _clustercomplete = false;
    int _clustersincolumn = 0;

    //Note:
    // The Gausian Distribution Phase Exsists In Two Phases
    // 1. The Intensifing and Equal Phase.
    // Where Intensity Levels are Increasing or Equal.
    // 2. The Detensivifing Phase.
    // Where Intensity Level are Decling or Equal.

    do
    {
        _dataindex = (_y * _max_x) + _x;
        _intensity = _data[_dataindex];

        //Process Total Image Intensity
        _tii += _intensity;


        //---------------------------------

        //Add Pixel to the Array According to a Gausian Distrobution

        //A Pixel Was Found in the Intensifing Stage
        if((_intensity > 0) && _inintensifingphase)
        {
            //First Pixel of List Add It
            if(_pixelarraysize == 0)
            {
                ++_pixelarrayindex; //Make Pixel Array Index 0
                _pixels[_pixelarrayindex].col = _x;
                _pixels[_pixelarrayindex].row = _y;
                _pixels[_pixelarrayindex].intensity = _intensity;
            }
            else //There Are Already Pixels in the List Check For Gausian Phase Change and Add Pixel
            {
                //Determine If We Need To Change The Gausian Phase
                if(_intensity < _pixels[_pixelarrayindex].intensity)
                {
                    _inintensifingphase = false;
                }
                //Add The Pixel
                ++_pixelarrayindex; //Increase For The Next Index
                _pixels[_pixelarrayindex].col = _x;
                _pixels[_pixelarrayindex].row = _y;
                _pixels[_pixelarrayindex].intensity = _intensity;
            }
            ++_pixelarraysize;
        }
        //The Detensifing Phase
        else if((_intensity > 0) && !_inintensifingphase)
        {
            //Make Sure We Don't have a complete cluster
            if(_intensity > _pixels[_pixelarrayindex].intensity)
            {
                _clustercomplete = true;
            }
            if(!_clustercomplete) //Still In Phase. Add Pixel
            {
                ++_pixelarrayindex; //Increase For The Next Index
                _pixels[_pixelarrayindex].col = _x;
                _pixels[_pixelarrayindex].row = _y;
                _pixels[_pixelarrayindex].intensity = _intensity;
                ++_pixelarraysize;
            }
        }
        //Handle If Cluster Complete Is Triggered
        if(_clustercomplete || (_intensity == 0))
        {
            //If There are pixels in the array Handle Them
            if(_pixelarraysize > 0)
            {
                //Find The Centroid By Weighted Average
                //And Record In The DataSet
                if((_pixelarraysize >= _declusterparameter.getMinClusterSize()) && (_pixelarraysize <= _declusterparameter.getMaxClusterSize()) && (_clustersincolumn < _declusterparameter.getMaxClustersInColumn()))
                {
                    float _weight_sum = 0.0;
                    float _weight_data_product_sum = 0.0;
                    int _element = 0;
                    do
                    {
                        _weight_sum += _pixels[_element].intensity;
                        _weight_data_product_sum += (_pixels[_element].row * _pixels[_element].intensity);
                        ++_element;
                    }while(_element < _pixelarraysize);
                    TBIPoint_Int __pnt(_x, (int)(_weight_data_product_sum/_weight_sum));
                    this->insert(__pnt);
                    ++_clustersincolumn;
                }
                /*
                else //Add The Whole List
                {
                    int _element = 0;
                    do
                    {
                        TBIPoint_Int __pnt(_x, _pixels[_element].row);
                        this->insert(__pnt);
                        ++_element;
                    }while(_element < _pixelarraysize);
                }
                */
                if(_clustercomplete) //Handle If Cluster Was Complete
                {
                    _clustercomplete = false;
                    _inintensifingphase = true;
                    _pixelarraysize = 1;
                    _pixelarrayindex = 0;
                    //Handle The Point That Was Never Assigned To The List
                    _pixels[0].col = _x;
                    _pixels[0].row = _y;
                    _pixels[0].intensity = _intensity;
                }
                else //Came in to the Loop Thru _intensity == 0.
                {
                    _clustercomplete = false;
                    _inintensifingphase = true;
                    _pixelarraysize = 0;
                    _pixelarrayindex = -1;
                }
            }

        }



        //---------------------------------
        //Process Is Over Move On To Next x,y
        ++_y;
        if(_y == _max_y)
        {
            //If There are stray pixels in the array handle Them
            if(_pixelarraysize > 0)
            {
                //Find The Centroid By Weighted Average
                //And Record In The DataSet
                if((_pixelarraysize >= _declusterparameter.getMinClusterSize()) && (_pixelarraysize <= _declusterparameter.getMaxClusterSize()) && (_clustersincolumn < _declusterparameter.getMaxClustersInColumn()))
                {
                    float _weight_sum = 0.0;
                    float _weight_data_product_sum = 0.0;
                    int _element = 0;
                    do
                    {
                        _weight_sum += _pixels[_element].intensity;
                        _weight_data_product_sum += (_pixels[_element].row * _pixels[_element].intensity);
                        ++_element;
                    }while(_element < _pixelarraysize);
                    TBIPoint_Int __pnt(_x, (int)(_weight_data_product_sum/_weight_sum));
                    this->insert(__pnt);
                }
                /*
                else //Add The Whole List
                {
                    int _element = 0;
                    do
                    {
                        TBIPoint_Int __pnt(_x, _pixels[_element].row);
                        this->insert(__pnt);
                        ++_element;
                    }while(_element < _pixelarraysize);
                }
                */

                _clustercomplete = false;
                _inintensifingphase = true;
                _pixelarraysize = 0;
                _pixelarrayindex = -1;
            }
            _y=0;
            ++_x;
            _clustersincolumn = 0;
        }
    }while(_x < _max_x);

    _declusterparameter.setTotalImageIntensity(_tii);
    if(_declusterparameter.totalImageIntensityToHigh()) return TBIDataSetReturnType::FailedTotalImageIntensityTooHigh;

    if(_declusterparameter.totalImageInstenisyToLow()) return TBIDataSetReturnType::FailedTotalImageIntensityTooLow;

    this->m_dataset_type = TBIDataSetType::GausianDeclusterType;
    return TBIDataSetReturnType::Ok;
}

TBIDataSetReturnType TBIDataSet::buildLeftTSLRansacDataSet(const TBIDataSet &_gausiansrcds, const int _endingsrcindex)
{
    this->clear();
    this->m_dataset_type = TBIDataSetType::LeftTSLForRansacType;
    if(_gausiansrcds.size() < 300) return TBIDataSetReturnType::FailedPassedInDataSetWrongSize;
    if(_endingsrcindex >= _gausiansrcds.size()) return TBIDataSetReturnType::FailedPassedInParameterMakesNoSense;

    return TBIDataSetReturnType::Ok;


}



//Extract Functions
TBIDataSetReturnType TBIDataSet::extractDataSetForLeftTSL(TBIDataSet &_dst, unsigned int _uniquexvalues)
{
    _dst.clear();
    if(m_dataset_size == 0) return TBIDataSetReturnType::FailedPassedInDataSetWrongSize;

    unsigned int _uniquex = 0;
    int _lastxvalue;
    int _pntindex = 0;

    _dst.insert(m_pnts[_pntindex]);
    _lastxvalue = m_pnts[_pntindex].m_x;
    _uniquex = 1;
    ++_pntindex;
    if(m_dataset_size == 1) return TBIDataSetReturnType::FailedPassedInDataSetWrongSize;

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

    return TBIDataSetReturnType::Ok;
}

TBIDataSetReturnType TBIDataSet::extractDataSetForRansacLeftTSL(TBIDataSet &_dst, int _breakindex)
{
    _dst.clear();
    if(m_dataset_size == 0) return TBIDataSetReturnType::FailedPassedInDataSetWrongSize;
    if(_breakindex < 1) return TBIDataSetReturnType::FailedPassedInParameterMakesNoSense;
    if(_breakindex >=  m_dataset_size) return TBIDataSetReturnType::FailedPassedInParameterMakesNoSense;

    int _pntindex = 0;
    do
    {

        _dst.insert(m_pnts[_pntindex]);

        ++_pntindex;
    }while(_pntindex < _breakindex);

    return TBIDataSetReturnType::Ok;
}

TBIDataSetReturnType TBIDataSet::extractDataSetForRightTSL(TBIDataSet &_dst, unsigned int _uniquexvalues)
{
    //Note DataSet For Right TSL is Backwards.
    _dst.clear();
    if(m_dataset_size == 0) return TBIDataSetReturnType::FailedPassedInDataSetWrongSize;

    unsigned int _uniquex = 0;
    int _lastxvalue;
    int _pntindex = m_dataset_size-1;

    _dst.insert(m_pnts[_pntindex]);
    _lastxvalue = m_pnts[_pntindex].m_x;
    _uniquex = 1;
    --_pntindex;
    if(m_dataset_size == 1) return TBIDataSetReturnType::FailedPassedInDataSetWrongSize;

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

    return TBIDataSetReturnType::Ok;
}

TBIDataSetReturnType TBIDataSet::extractDataSetForRansacRightTSL(TBIDataSet &_dst, int _startindex)
{
    _dst.clear();
    if(m_dataset_size == 0) return TBIDataSetReturnType::FailedPassedInDataSetWrongSize;
    if(_startindex >= m_dataset_size-1) return TBIDataSetReturnType::FailedPassedInParameterMakesNoSense;
    if((m_dataset_size-1-_startindex) < 0) return TBIDataSetReturnType::FailedPassedInParameterMakesNoSense;

    int _pntindex = _startindex;
    do
    {

        _dst.insert(m_pnts[_pntindex]);


        ++_pntindex;
    }while(_pntindex < m_dataset_size);

    return TBIDataSetReturnType::Ok;
}

TBIDataSetReturnType TBIDataSet::extractDataSetForInliers(TBIDataSet &_dst, const TBILine &_line, const float _distancethreshold)
{
    _dst.clear();
    if(m_dataset_size == 0) return TBIDataSetReturnType::FailedPassedInDataSetWrongSize;
    if(!_line.isValid()) return TBIDataSetReturnType::FailedPassedInParameterMakesNoSense;
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

    return TBIDataSetReturnType::Ok;
}

TBIDataSetReturnType TBIDataSet::extractDataSetForInliers(TBIDataSet &_dst, const TBILine &_ransacline, const float _distancethreshold, const int _start_index, const int _end_index)
{
    _dst.clear();
    if(m_dataset_size == 0) return TBIDataSetReturnType::FailedPassedInDataSetWrongSize;
    if(!_ransacline.isValid()) return TBIDataSetReturnType::FailedPassedInParameterMakesNoSense;
    if(_start_index >= _end_index) return TBIDataSetReturnType::FailedPassedInParameterMakesNoSense;
    if(_end_index >= m_dataset_size) return TBIDataSetReturnType::FailedPassedInParameterMakesNoSense;
    int _index = _start_index;
    float _distance;

    do
    {
        _distance = _ransacline.distanceAbs(m_pnts[_index]);
        if(_distance <= _distancethreshold)
        {
            _dst.insert(m_pnts[_index]);
        }
        ++_index;
    }while(_index <= _end_index);

    return TBIDataSetReturnType::Ok;
}

TBIDataSetReturnType TBIDataSet::extractInlierDataSet(TBIDataSet &_dst, const TBIDataDistributionSet &_srcdistro)
{
    _dst.clear();
    if(m_dataset_size == 0) return TBIDataSetReturnType::FailedPassedInDataSetWrongSize;
    int _srcindex = 0;

    do
    {
        if(_srcdistro[m_pnts[_srcindex].m_x] == 1)
        {
            _dst.insert(m_pnts[_srcindex]);
        }
        ++_srcindex;
    }while(_srcindex < m_dataset_size);

    return TBIDataSetReturnType::Ok;
}

TBIDataSetReturnType TBIDataSet::extractOutlierDataSet(TBIDataSet &_inlierds, TBIDataSet &_outlierds)
{
    if(this->size() == 0) return TBIDataSetReturnType::FailedPassedInDataSetWrongSize;
    if(_inlierds.size() == 0) return TBIDataSetReturnType::FailedPassedInDataSetWrongSize;
    if(_inlierds.size() > this->size()) return TBIDataSetReturnType::FailedPassedInDataSetWrongSize;

    _outlierds.clear();

    int _index = 0;
    do
    {
        if(!_inlierds.containsPoint(this->getPoint(_index)))
        {
            TBIPoint_Int _pnt = this->getPoint(_index);
            _outlierds.insert(_pnt);
        }
        ++_index;

    }while(_index < this->size());

    return TBIDataSetReturnType::Ok;
}

TBIDataSetReturnType TBIDataSet::extractDistributionSet(TBIDataDistributionSet &_distroset)
{


    //Clear The Distribution Set
    _distroset.clear();
    if(m_dataset_size == 0) return TBIDataSetReturnType::FailedPassedInDataSetWrongSize;



    //There is At least 1 element.
    //Set The initial Values
    _distroset.incrementIndex(m_pnts[0].m_x);
    if(m_dataset_size == 1) return TBIDataSetReturnType::FailedPassedInDataSetWrongSize;

    //Start Incrementing the distrobution set.
    int _index = 1;
    do
    {
        _distroset.incrementIndex(m_pnts[_index].m_x);
        ++_index;
    }while(_index < m_dataset_size);

    return TBIDataSetReturnType::Ok;
}

TBIDataSetReturnType TBIDataSet::extractLeastSquareLine(TBILine &_line)
{
    _line.clear();
    if(m_dataset_size == 0) return TBIDataSetReturnType::FailedPassedInDataSetWrongSize;
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

    return TBIDataSetReturnType::Ok;

}

TBIDataSetReturnType TBIDataSet::extractFilteredGausianSet(TBIDataSet &_dst, int _breakindex)
{
    _dst.clear();
    if(_breakindex >= m_dataset_size) return TBIDataSetReturnType::FailedPassedInDataSetWrongSize;
    if(m_dataset_size < 2) return TBIDataSetReturnType::FailedPassedInDataSetWrongSize;

    int _index = 0;

    do
    {
        if(abs(m_pnts[_index].m_y - m_pnts[_breakindex].m_y) > 5)
        {
            _dst.insert(m_pnts[_index]);
        }
        ++_index;
    }while(_index < m_dataset_size);

    return TBIDataSetReturnType::Ok;
}

TBIDataSetReturnType TBIDataSet::extractDataSubSet(TBIDataSet &_dst, int _startindex, int _endindex)
{
    _dst.clear();
    if(_endindex >= m_dataset_size) return TBIDataSetReturnType::FailedPassedInParameterMakesNoSense;
    if(_startindex > _endindex) return TBIDataSetReturnType::FailedPassedInParameterMakesNoSense;
    if(m_dataset_size < 2) return TBIDataSetReturnType::FailedPassedInDataSetWrongSize;

    int _index = _startindex;
    do
    {
        _dst.insert(m_pnts[_index]);
        ++_index;
    }while(_index <= _endindex);
    return TBIDataSetReturnType::Ok;
}

bool TBIDataSet::containsPoint(TBIPoint_Int _pnt)
{
    if(this->size() == 0) return false;
    int _index = 0;
    do
    {
        if(this->m_pnts[_index] == _pnt) return true;
        ++_index;
    }while(_index < this->size());

    return false;

}


//This is Function Is Only Useful For Break Index DataSets.
int TBIDataSet::extractVGrooveBreakXValue(int _mindistancethreshold)
{
    if(this->size() < 10) return -1;
    if(this->size() > 2000) return -1;
    if(_mindistancethreshold < 0) return -1;

    int _index = 0;
    int _calcdist = 0;
    int _largestdist = 0;
    int _totalhits = 0;
    int _largestindex=0;

    do
    {
        _calcdist = (int)this->m_pnts[_index].distance(this->m_pnts[_index+1]);
        if(_calcdist > _largestdist)
        {
            _largestdist = _calcdist;
            _largestindex = _index;
        }
        if(_calcdist >= _mindistancethreshold) ++_totalhits;
        ++_index;
    }while(_index < (this->size() - 2));


    if(_totalhits > 1) return -1; //More than 1 Bounded Break Bad Data

    if(_totalhits == 0) //Might Not Be Bounded. Determine Left or Right Side
    {
        //Left Side Has More Distance to the Frame Edge. This is a Right Side Line
        //So Break at 0 index;
        if(this->m_pnts[0].m_x > (TBIConstants::Max_Camera_Width - this->m_pnts[this->size()-1].m_x))
        {
            return this->m_pnts[0].m_x;
        }
        return this->m_pnts[this->size()-1].m_x;
    }

    //All that is left now is the bounded find
    //We need to know how many points lie on the left side of _largestindex and how many on the right side of largest index

    int _pntsleft = _largestindex+1;
    int _pntsright = this->size()-(_largestindex+1);
    if(_pntsleft > _pntsright) //More Points on the Left. Left Side Line.
    {
        return this->m_pnts[_largestindex].m_x;
    }
    if(_pntsright > _pntsleft) //More Points on the Right. Right Side Line.
    {
        return this->m_pnts[_largestindex+1].m_x;
    }
    if(_pntsleft == _pntsright) //Equal Number of Points. Cant's Determine.
    {
        return -1;
    }


    return -1;
}



