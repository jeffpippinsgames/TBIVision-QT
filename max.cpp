#include "max.h"
#include <QDebug>
#include "pixelcolumnclass.h"
#include "pixelclusterclass.h"
#include "pixelfundamental.h"
#include "tbilinearransacvotingstructure.h"
#include <QRandomGenerator>
#include <QThread>
#include <vector>
#include <random>
#include "tbipoint_int.h"
#include "tbiransac.h"

//Constructors and Destructor--------------------------------------------------------
Max::Max(QObject *parent) : QObject(parent)
{

    m_scan_ds = new TBIDataSet();
    m_roughing_left_tsl_ds = new TBIDataSet();
    m_roughing_right_tsl_ds = new TBIDataSet();
    m_left_tsl_inliers_ds = new TBIDataSet();
    m_right_tsl_inliers_ds = new TBIDataSet();
    m_joint_ds = new TBIDataSet();
    m_dummy_set1 = new TBIDataSet();
    m_dummy_set2 = new TBIDataSet();


    m_emitextramats = false;

    m_in_proccesing_loop = false;
    m_timeinloop = "Error:";
    m_blur = 3;
    m_thresholdmin = 100;
    m_thresholdmax = 255;

    m_max_image_intensity = 720*540*255*.3;
    m_min_image_intensity = 720*255;
    m_min_cluster_size = 6;
    m_max_cluster_size = 75;

    m_allowable_discontinuities = 20;



    m_left_tsl_ransac.setIdealAngle(0);
    m_left_tsl_ransac.setAllowedAngleVariance(10);
    m_left_tsl_ransac.setDistanceThreshold(2.0);
    m_left_tsl_ransac.setIterations(50);
    m_left_tsl_ransac.setMinVotes(20);

    m_right_tsl_ransac.setIdealAngle(0);
    m_right_tsl_ransac.setAllowedAngleVariance(10);
    m_right_tsl_ransac.setDistanceThreshold(2.0);
    m_right_tsl_ransac.setIterations(50);
    m_right_tsl_ransac.setMinVotes(20);

    m_left_bwl_ransac.setIdealAngle(-30);
    m_left_bwl_ransac.setAllowedAngleVariance(10);
    m_left_bwl_ransac.setDistanceThreshold(2.0);
    m_left_bwl_ransac.setIterations(50);
    m_left_bwl_ransac.setMinVotes(20);

    m_right_bwl_ransac.setIdealAngle(30);
    m_right_bwl_ransac.setAllowedAngleVariance(10);
    m_right_bwl_ransac.setDistanceThreshold(2.0);
    m_right_bwl_ransac.setIterations(50);
    m_right_bwl_ransac.setMinVotes(20);


    m_sm_distance_threshold = 5;
    m_sm_length_threshold = 20;


    emit timeInLoopChanged(m_timeinloop);
    qDebug()<<"Max::Max() Max Object Created.";
}

Max::~Max()
{
    delete m_scan_ds;
    delete m_roughing_left_tsl_ds;
    delete m_roughing_right_tsl_ds;
    delete m_left_tsl_inliers_ds;
    delete m_right_tsl_inliers_ds;
    delete m_joint_ds;
    delete m_dummy_set1;
    delete m_dummy_set2;
    emit this->aboutToDestroy();
    qDebug()<<"Max::~Max() Max Object Destroyed";
}

//Processing Methods------------------------------------------------------------------


//PreProcessing Methods--------------------------------------------------
void Max::blankProcessingArrays()
{


    m_left_tsl.clear();
    m_right_tsl.clear();
    m_right_bwl.clear();
    m_left_bwl.clear();
    m_topography_lines.clear();
    m_flattened_iohrv = 0;
    m_total_image_intensity = 0;

    m_scan_ds->clear();
    //m_left_tsl_inliers_ds.clear();
    // m_right_tsl_inliers_ds.clear();
    // m_joint_ds.clear();

    // m_flattened_bevel_wall_data.clear();
    // m_flattened_left_bevel_wall_data.clear();
    // m_flattened_right_bevel_wall_data.clear();
    // m_flattened_left_bwl_data.clear();
    // m_flattened_left_tsl_data.clear();
    // m_flattened_right_bwl_data.clear();
    // m_flattened_right_tsl_data.clear();
    // m_flattened_scan_data.clear();
}

int Max::randomInt(int _min, int _max)
{
    //C++ 11 Random Number Engine. #include <random>
    std::random_device seeder; //the random device that will seed the generator
    std::mt19937 engine(seeder()); //then make a mersenne twister engine
    std::uniform_int_distribution<int> dist(_min, _max); //then the easy part... the distribution
    return dist(engine); //Return the Number From The Distrobution
}

/*
//Flattned Data Functions
bool Max::doImageFlattening(Mat &_src, std::vector<TBIPoint_Float> &_data, quint64 *_tii, const quint64 _max_tii, const quint64 _min_tii)
{
    *_tii = 0;
    _data.clear();

    if(_src.type() != CV_8UC1)
    {
        emit failedFlattenImageData();
        return false;
    }

    int _row = 0;
    int _col = 0;
    int _dataindex = 0;
    uint8_t * _srcdata = _src.data;

    do
    {
        _dataindex = (_row * _src.cols) + _col;
        if(_srcdata[_dataindex] > 0)
        {

            _data.push_back(TBIPoint_Float((float)_col, (float)_row));
            *_tii += _srcdata[_dataindex];
        }

        if(*_tii > _max_tii)
        {
            emit failedTIICheck();
            return false;
        }

        ++_row;
        if(_row >= _src.rows)
        {
            ++_col;
            _row = 0;
        }
    }while(_col < _src.cols);

    if(*_tii < _min_tii)
    {
        emit failedTIICheck();
        return false;
    }

    if(_data.size() < 300)
    {
        emit failedFlattenImageData();
        return false;
    }

    return true;
}

void Max::drawFlattenedImageDataToMat(Mat &_dst, const std::vector<TBIPoint_Float> &_data)
{
    if(_dst.type() != CV_8UC1)
    {
        return;
    }

    if(_data.size() == 0)
    {
        return;
    }

    int _row = 0;
    int _col = 0;
    const int _vectorsize = _data.size();
    int _vectorindex = 0;
    uint8_t * _matdata = _dst.data;
    int _matindex = 0;

    do
    {

        _col = (int)_data[_vectorindex].m_x;
        _row = (int)_data[_vectorindex].m_y;
        _matindex = (_row * _dst.cols) + _col;
        _matdata[_matindex] = 255;
        ++_vectorindex;

    }while(_vectorindex < _vectorsize);

}

void Max::drawFlattenedImageDataToMat(Mat &_dst, const std::vector<TBIPoint_Float> &_data, const Scalar _color)
{
    if(_dst.type() != CV_8UC3)
    {
        return;
    }

    if(_data.size() == 0)
    {
        return;
    }



    uint8_t * _matdata = _dst.data;
    const int _matchannels = _dst.channels();

    int _row = 0;
    int _col = 0;
    const int _vectorsize = _data.size();
    int _vectorindex = 0;


    int _matindexc1 = 0;
    int _matindexc2 = 0;
    int _matindexc3 = 0;

    //float value = pImgData[img.channels() * (img.cols * row + column) + channel];
    do
    {
        _col = (int)_data[_vectorindex].m_x;
        _row = (int)_data[_vectorindex].m_y;
        _matindexc1 = _matchannels * (_dst.cols * _row + _col) + 0;
        _matindexc2 = _matchannels * (_dst.cols * _row + _col) + 1;
        _matindexc3 = _matchannels * (_dst.cols * _row + _col) + 2;
        _matdata[_matindexc1] = _color[2];
        _matdata[_matindexc2] = _color[1];
        _matdata[_matindexc3] = _color[0];
        ++_vectorindex;
    }while(_vectorindex < _vectorsize);

}

void Max::copyFlattenedData(const std::vector<TBIPoint_Float> &_src, std::vector<TBIPoint_Float> &_dst)
{

    _dst.clear();
    if(_src.size() == 0) return;

    int _index = 0;
    const int _srcsize = _src.size();
    do
    {
        TBIPoint_Float _pnt = _src[(_srcsize-1) - _index];
        _dst.push_back(_pnt);
        ++_index;
    }while(_index < _srcsize);
}

void Max::removeInliersFromFlattenedData(std::vector<TBIPoint_Float> &_dstdata, const TBILine &_line, const float _distance_threshold_top, const float _distance_threshold_bottom)
{
    if(_dstdata.size() == 0)
    {
        return;
    }

    if(!_line.isValid())
    {
        return;
    }

    int _index = _dstdata.size()-1;
    float _distance;


    do
    {
        _distance = _line.distanceAbs(_dstdata[_index]);
        if(_line.isPointOnLine(_dstdata[_index]))
        {
            _dstdata.erase(_dstdata.begin() + _index);
        }
        else if(_line.isPointAboveLine(_dstdata[_index]))
        {
            if(_distance <= _distance_threshold_top)
            {
                _dstdata.erase(_dstdata.begin() + _index);
            }
        }
        else if(_line.isPointBelowLine(_dstdata[_index]))
        {
            if(_distance <= _distance_threshold_bottom)
            {
                _dstdata.erase(_dstdata.begin() + _index);
            }
        }
        --_index;
    }while((_index >= 0));
}

void Max::buildFlattenDataFromInliers(const std::vector<TBIPoint_Float> &_srcdata, std::vector<TBIPoint_Float> &_dstdata, const TBILine &_line, const float _distance_threshold_top, const float _distance_threshold_bottom)
{
    _dstdata.clear();
    if(_srcdata.size() == 0) return;
    if(!_line.isValid()) return;

    int _index = 0;
    int _srcvector_size = _srcdata.size();
    float _distance;

    do
    {
        _distance = _line.distanceAbs(_srcdata[_index]);
        if(_line.isPointOnLine(_srcdata[_index]))
        {
            _dstdata.push_back(_srcdata[_index]);
        }
        else if(_line.isPointAboveLine(_srcdata[_index]))
        {
            if(_distance <= _distance_threshold_top)
            {
                _dstdata.push_back(_srcdata[_index]);
            }
        }
        else if(_line.isPointBelowLine(_srcdata[_index]))
        {
            if(_distance <= _distance_threshold_bottom)
            {
                _dstdata.push_back(_srcdata[_index]);
            }
        }

        ++_index;
    }while(_index < _srcvector_size);
}

bool Max::buildFlattenDataFromBegining(const std::vector<TBIPoint_Float> &_srcdata, std::vector<TBIPoint_Float> &_dstdata, const int _consequetiverows)
{
    _dstdata.clear();

    if(_srcdata[_srcdata.size() - 1].m_x < _consequetiverows)
    {
        return false;
    }

    int _newrowsencoutered = 0;
    int _currentrow = -1;
    int _index = 0;
    int _srcsize = _srcdata.size();

    do
    {
        if(_currentrow != _srcdata[_index].m_x)
        {
            _currentrow = _srcdata[_index].m_x;
            ++_newrowsencoutered;
        }
        _dstdata.push_back(_srcdata[_index]);
        ++_index;
    }while((_newrowsencoutered < _consequetiverows) && (_index < _srcsize ));

    if(_newrowsencoutered < _consequetiverows)
    {
        return false;
    }

    return true;
}

bool Max::buildFlattenDataFromEnd(const std::vector<TBIPoint_Float> &_srcdata, std::vector<TBIPoint_Float> &_dstdata, const int _consequetiverows)
{
    _dstdata.clear();

    if(_srcdata[_srcdata.size() - 1].m_x < _consequetiverows)
    {
        return false;
    }

    int _newrowsencoutered = 0;
    int _currentrow = -1;
    int _index = _srcdata.size()-1;
    int _srcsize = _srcdata.size();

    do
    {
        if(_currentrow != _srcdata[_index].m_x)
        {
            _currentrow = _srcdata[_index].m_x;
            ++_newrowsencoutered;
        }
        _dstdata.push_back(_srcdata[_index]);
        --_index;
    }while((_newrowsencoutered < _consequetiverows) && (_index > 0));

    if(_newrowsencoutered < _consequetiverows)
    {
        return false;
    }

    return true;
}



//Cluster Colulmn Processing
bool Max::doPixelColumnProcessing(Mat &_src, Mat &_dst, PixelColumnClass *_pixel_column_array, quint64 *_tii, quint64 _max_tii,
                                  quint64 _min_tii, int _min_cluster_size, int _max_cluster_size, int _max_clusters_in_column)
{
    *_tii = 0;
    int _column_index = 0;
    PixelColumnProcessingReturn_t _val;



    //Create The Column List.
    do
    {
        _val = _pixel_column_array[_column_index].pixelProccessColumn(_src, _column_index, _tii, _max_tii,
                                                                      _min_cluster_size, _max_cluster_size, _max_clusters_in_column);

        if(_val == PCP_FAILED_MAX_TII)
        {
            emit totalImageIntensityChanged(m_total_image_intensity);
            emit failedTIICheck();

            return false;
        }
        else if(_val == PCP_OK)
        {
            _pixel_column_array[_column_index].drawToMat(_dst);
        }
        ++_column_index;

    }while(_column_index < _src.cols);

    //Check For Min Total Image Intensity
    if(*_tii < _min_tii)
    {
        emit totalImageIntensityChanged(m_total_image_intensity);
        emit failedTIICheck();
        return false;
    }



    //start the right side continuity check.

    emit totalImageIntensityChanged(m_total_image_intensity);
    return true;

}

bool Max::clusterProcessThresholdMat(Mat &_src, Mat &_dst, quint64 *_tii, const quint64 _max_tii, const quint64 _min_tii,
                                     const int _min_cluster_size, const int _max_cluster_size, const int _max_clusters_in_column, std::vector<TBIPoint_Float> &_scan_data_vector)
{


    if(_src.type() != CV_8UC1)
    {
        emit failedSkeletonizeMat();
        return false;
    }

    _scan_data_vector.clear();
    *_tii = 0;

    int _x = 0;
    int _y = 0;
    uint8_t _value;
    int _clusterindex;
    PixelClusterClass _cluster;

    //Mat_Index = ((Y) * Mat.cols) + X
    uint8_t *_srcdata = _src.data;
    uint8_t *_dstdata = _dst.data;

    int _srcindex = 0;
    int _dstindex = 0;

    //Iterate Thru The Src Mat And Find Clusters
    do
    {
        _srcindex = (_y * _src.cols) + _x;
        _value = _srcdata[_srcindex];
        if(_value > 0)
        {
            *_tii += _value;
            if(*_tii > _max_tii)
            {
                emit failedTIICheck();
                return false;
            }
            PixelFundamental_t _pixel(_y, _x, _value);
            _cluster.pushPixelToBack(_pixel);
        }
        else if(_value == 0)
        {
            int _cluster_size = _cluster.size();
            if(_cluster_size > 0) //The Cluster Has Pixels in it.
            {
                if((_cluster_size >= _min_cluster_size) && (_cluster_size <= _max_cluster_size) && (_cluster.isGausian())) //The Cluster Needs a Centroid
                {
                      _cluster.setRowCentroid();
                      _dstindex = ((int)_cluster.getRowCentroid() * _dst.cols) + _x;
                      _dstdata[_dstindex] = 255;
                      _scan_data_vector.push_back(TBIPoint_Float((float)_x, _cluster.getRowCentroid()));

                }
                else //The Cluster Is Not Centroid Put Them all in the _dst mat
                {
                    _clusterindex = 0;
                    do
                    {
                       _dstindex = (_cluster[_clusterindex].row * _dst.cols) + _cluster[_clusterindex].col;
                       _dstdata[_dstindex] = 255;
                       _scan_data_vector.push_back(TBIPoint_Float((float)_cluster[_clusterindex].col, (float)_cluster[_clusterindex].row));
                        ++_clusterindex;
                    }while(_clusterindex < _cluster.size());
                }
            }
            _cluster.clear();
        }
        ++_y;
        if(_y == _src.rows)
        {

            int _cluster_size = _cluster.size();
            if(_cluster_size > 0) //The Cluster Has Pixels in it.
            {
                if((_cluster_size >= _min_cluster_size) && (_cluster_size <= _max_cluster_size) && (_cluster.isGausian())) //The Cluster Needs a Centroid
                {
                    _cluster.setRowCentroid();
                   _dstindex = ((int)_cluster.getRowCentroid() * _dst.cols) + _x;
                   _dstdata[_dstindex] = 255;
                   _scan_data_vector.push_back(TBIPoint_Float((float)_x, _cluster.getRowCentroid()));
                }
                else //The Cluster Is Not Centroid Put Them all in the _dst mat
                {
                    _clusterindex = 0;
                    do
                    {
                       _dstindex = (_cluster[_clusterindex].row * _dst.cols) + _cluster[_clusterindex].col;
                       _dstdata[_dstindex] = 255;
                       _scan_data_vector.push_back(TBIPoint_Float((float)_cluster[_clusterindex].col, (float)_cluster[_clusterindex].row));
                        ++_clusterindex;
                    }while(_clusterindex < _cluster.size());
                }
            }
            _cluster.clear();
            _y = 0;
            ++_x;
        }

    }while(_x < _src.cols);

    if(*_tii < _min_tii)
    {
        emit failedTIICheck();
        return false;
    }



    return true;
}
*/
bool Max::clusterProcessThresholdMat(Mat &_src, Mat &_dst, quint64 *_tii, const quint64 _max_tii,
                                     const quint64 _min_tii, const int _min_cluster_size,
                                     const int _max_cluster_size, const int _max_clusters_in_column, TBIDataSet &_scandataset)
{

    if(_src.type() != CV_8UC1)
    {
        emit failedSkeletonizeMat();
        return false;
    }

    _scandataset.clear();
    *_tii = 0;

    int _x = 0;
    int _y = 0;
    uint8_t _value;
    int _clusterindex;
    PixelClusterClass _cluster;

    //Mat_Index = ((Y) * Mat.cols) + X
    uint8_t *_srcdata = _src.data;
    uint8_t *_dstdata = _dst.data;

    int _srcindex = 0;
    int _dstindex = 0;

    //Iterate Thru The Src Mat And Find Clusters
    do
    {
        _srcindex = (_y * _src.cols) + _x;
        _value = _srcdata[_srcindex];
        if(_value > 0)
        {
            *_tii += _value;
            if(*_tii > _max_tii)
            {
                emit failedTIICheck();
                return false;
            }
            PixelFundamental_t _pixel(_y, _x, _value);
            _cluster.pushPixelToBack(_pixel);
        }
        else if(_value == 0)
        {
            int _cluster_size = _cluster.size();
            if(_cluster_size > 0) //The Cluster Has Pixels in it.
            {
                if((_cluster_size >= _min_cluster_size) && (_cluster_size <= _max_cluster_size)) //The Cluster Needs a Centroid
                {
                    _cluster.setRowCentroid();
                    _dstindex = ((int)_cluster.getRowCentroid() * _dst.cols) + _x;
                    _dstdata[_dstindex] = 255;
                    TBIPoint_Int ___pnt(_x, (int)_cluster.getRowCentroid());
                    _scandataset.insert(___pnt);
                }
                else //The Cluster Is Not Centroid Put Them all in the _dst mat
                {
                    _clusterindex = 0;
                    do
                    {
                        _dstindex = (_cluster[_clusterindex].row * _dst.cols) + _cluster[_clusterindex].col;
                        _dstdata[_dstindex] = 255;
                        TBIPoint_Int ___pnt(_cluster[_clusterindex].col, _cluster[_clusterindex].row);
                        _scandataset.insert(___pnt);
                        ++_clusterindex;
                    }while(_clusterindex < _cluster.size());
                }
            }
            _cluster.clear();
        }
        ++_y;
        if(_y == _src.rows)
        {

            int _cluster_size = _cluster.size();
            if(_cluster_size > 0) //The Cluster Has Pixels in it.
            {
                if((_cluster_size >= _min_cluster_size) && (_cluster_size <= _max_cluster_size)) //The Cluster Needs a Centroid
                {
                    _cluster.setRowCentroid();
                    _dstindex = ((int)_cluster.getRowCentroid() * _dst.cols) + _x;
                    _dstdata[_dstindex] = 255;
                    TBIPoint_Int ___pnt(_x, (int)_cluster.getRowCentroid());
                    _scandataset.insert(___pnt);
                }
                else //The Cluster Is Not Centroid Put Them all in the _dst mat
                {
                    _clusterindex = 0;
                    do
                    {
                        _dstindex = (_cluster[_clusterindex].row * _dst.cols) + _cluster[_clusterindex].col;
                        _dstdata[_dstindex] = 255;
                        TBIPoint_Int ___pnt(_cluster[_clusterindex].col, _cluster[_clusterindex].row);
                        _scandataset.insert(___pnt);
                        ++_clusterindex;
                    }while(_clusterindex < _cluster.size());
                }
            }
            _cluster.clear();
            _y = 0;
            ++_x;
        }

    }while(_x < _src.cols);

    if(*_tii < _min_tii)
    {
        emit failedTIICheck();
        return false;
    }



    return true;
}



/*
//Skeletal Functions
bool Max::doSkeletonProcessing(Mat &_dst, PixelColumnClass *_pixel_column_array, float *_skel_array, float _continuity_threshold)
{
    uint8_t* _data = (uint8_t*)_dst.data;
    int _empty_cols = 0;
    int _index = 0;
    int _dataindex;
    float _rowcentroid;
    float _highestrowvalue = 0.0;

    do
    {
        if(m_cluster_columns[_index].size() == 1) //One Cluster Pop it
        {
            _rowcentroid = m_cluster_columns[_index].getCentroidofCluster(0);
            m_skeletal_line_array[_index] = _rowcentroid;
            if(_rowcentroid > _highestrowvalue)
            {
                _highestrowvalue = _rowcentroid;
                m_flattened_hrv = _rowcentroid;
                m_flattened_iohrv = _index;
            }
            _dataindex = ((int)_rowcentroid * _dst.cols) + _index;
            _data[_dataindex] = 255;
        }
        else
        {
            ++_empty_cols;
        }
        ++_index;
    }while(_index < _dst.cols);

    if(_empty_cols > m_allowable_discontinuities)
    {
        emit failedDiscontinuityCheck();
        return false;
    }
    return true;
}

void Max::deleteSkelPointsAbovePZL(TBILine &_lpzl, TBILine &_rpzl, float *_skel_array)
{

    if(!_lpzl.isValid()) return;
    if(!_rpzl.isValid()) return;

    //GO Thru Every Skel data element and remove anything that is about the pzl left or right
    int _seperatedindex = (int)_lpzl.getPoint2X();

    int _currentindex = 0;
    float _linevalue;


    do
    {
        if(_skel_array[_currentindex] != -1)
        {
            //Get the Correct Line Value. Either Left or Right.
            if(_currentindex < _seperatedindex) //On the Left PZL
            {
                _linevalue = _lpzl.getYatX((float)_currentindex);
            }
            else if(_currentindex > _seperatedindex) //on the Right PZL
            {
                _linevalue = _rpzl.getYatX((float)_currentindex);
            }
            else//Must Determine which line to use.
            {
                if(_lpzl.getYatX((float)_currentindex) < _rpzl.getYatX((float)_currentindex)) //left is higher. Use left
                {
                    _linevalue = _lpzl.getYatX((float)_currentindex);
                }
                else
                {
                    _linevalue = _rpzl.getYatX((float)_currentindex);
                }
            }
            if(_linevalue > _skel_array[_currentindex]) //The Point is above the PZL
            {
                _skel_array[_currentindex] = -1;
            }
        }
        ++_currentindex;
    }while(_currentindex < Mat_Max_Width);

}
*/
bool Max::doSplitMergeProcesssing(float *_data_array, int _max_index, std::vector<TBILine> &_line_vectors, float _min_distance_threshold)
{
    //Clear The Line Vector
    _line_vectors.clear();

    int _index_of_most_distant=0;
    float _most_distant = 0.0;
    float _distance = 0.0;
    int _start_index = -1;
    int _end_index = -1;
    int _current_index;
    int _current_vector_index = 0;
    float _data_value;


    //Set the Start and Ending index
    _current_index = 0;
    do
    {
        if(_data_array[_current_index] != -1)
        {
            _start_index = _current_index;
            _current_index = _max_index;
        }
        ++_current_index;
    }while(_current_index < _max_index);
    //Set the Ending Index
    _current_index = _max_index;
    do
    {
        if(_data_array[_current_index] != -1)
        {
            _end_index= _current_index;
            _current_index = _start_index;
        }
        --_current_index;
    }while(_current_index > _start_index);

    //Make Sure The start and ending index is ok
    if((_start_index == -1) || (_end_index == -1))
    {
        return false;
    }

    //Add First Line.
    TBILine _line((float)_start_index, _data_array[_start_index], (float)_end_index, _data_array[_end_index]);
    _line_vectors.push_back(_line);

    //Get Distance From All Points start
    _current_index = _start_index;
    do
    {
        //Get Distance Of All Points From _start_index to _end_index
        _data_value = _data_array[_current_index];
        if(_data_value > 0.0)
        {
            _distance = _line_vectors[_current_vector_index].distanceAbs((float)_current_index, _data_value);
        }
        else
        {
            _distance = 0;
        }

        if(_distance >= _most_distant)
        {
            _most_distant = _distance;
            _index_of_most_distant = _current_index;
        }
        //Increment The Current Index
        ++_current_index;
        if(_current_index > _end_index) //All The Distant Points Are Checked. Do The Split Checks
        {
            if(_most_distant >= _min_distance_threshold) //The Line Can Be Split!
            {
                TBILine _line1((float)_start_index, _data_array[_start_index], (float)_index_of_most_distant, _data_array[_index_of_most_distant]);
                TBILine _line2((float)_index_of_most_distant, _data_array[_index_of_most_distant], (float)_end_index, _data_array[_end_index]);
                auto _iterator = _line_vectors.begin();
                _line_vectors[_current_vector_index] = _line1;
                _line_vectors.insert(_iterator+_current_vector_index+1, _line2);
                ++_current_vector_index;
            }
            else //The Line Cannot Be Split
            {
                --_current_vector_index;
            }
            //Adjust The Indexes and reset the distance variables
            //For The Next Line Iteration
            if(_current_vector_index >= 0)
            {
                _start_index = (int)_line_vectors[_current_vector_index].getPoint1X();
                _end_index = (int)_line_vectors[_current_vector_index].getPoint2X();
                _current_index = _start_index;
                _most_distant = 0;
                _index_of_most_distant = 0;
            }
        }

    }while(_current_vector_index >= 0);

    if(_line_vectors.size() < 4)
    {
        emit failedSplitMergeCheck();
        return false;
    }
    return true;
}




/*

//Ransac Functions
bool Max::doRansacLineProcessing(TBILine &_line, const TBILinearRansac &_ransac, const std::vector<TBIPoint_Float> &_vector)
{
    _line.clear();

    if(_vector.size() < 50)
    {
        emit failedRansacCheck();
        return false;
    }

    std::vector<TBILinearRansacVotingStructure> _linecandidates;

    int _iteration = 0;
    int _pnt1index;
    int _pnt2index;
    int _votingindex = 0;
    float _distance = 0;

    const int _vectorsize = _vector.size();
    const int _requiredvotes = _ransac.getMinVotes();
    const int _totaliterations = _ransac.getIteration();
    const float _distancethreshold = _ransac.getDistanceThreshold();
    float _minanglefromhorizon = _ransac.getIdealAngle() - _ransac.getAllowedAngleVariance();
    float _maxanglefromhorizon = _ransac.getIdealAngle() + _ransac.getAllowedAngleVariance();
    float _idealanglefromhorizon = _ransac.getIdealAngle();

    //Do the Ransac
    do
    {
        //Instantiate the Voting Container
        TBILinearRansacVotingStructure _linecandidate;
        //Start Getting Two Random Points
        _pnt1index = randomInt(0, _vectorsize-1);
        do
        {
            _pnt2index = randomInt(0, _vectorsize-1);
        }while(_pnt1index == _pnt2index);
        //Build The Line From The Two Data Points
        _linecandidate.m_line = TBILine(_vector[_pnt1index], _vector[_pnt2index]);
        //If Horizonal Angle Check is Good Do The Voting.
        _linecandidate.m_angletohorizontal = _linecandidate.m_line.angleFromHorizontal();
        if((_linecandidate.m_angletohorizontal >= _minanglefromhorizon) && (_linecandidate.m_angletohorizontal <= _maxanglefromhorizon))
        {
            //Horizon Angle Check is Good Do The Voting
            _votingindex = 0;
            do
            {
                _distance = _linecandidate.m_line.distanceAbs(_vector[_votingindex]);
                if(_distance <= _distancethreshold)
                {
                    ++_linecandidate.m_numvotes;
                }
                ++_votingindex;
            }while(_votingindex < _vectorsize);
            //Process The Voting And Add the Candidate If Ok
            if(_linecandidate.m_numvotes >= _requiredvotes)
            {
                _linecandidates.push_back(_linecandidate);
            }
        }
        ++_iteration;
    }while(_iteration < _totaliterations);

    //Evaluate The Candidate List and set the Line if Ok
    if(_linecandidates.size() == 0)
    {
        emit failedRansacCheck();
        return false;
    }

    //Choose the Line Candidate That has the Most Votes
    const int _candidatesize = (int)_linecandidates.size();
    int _candidateindex = 0;
    int _highestvotecount = -1;
    int _highestcandidateindex = -1;
    do
    {

        if(_highestvotecount < _linecandidates[_candidateindex].m_numvotes)
        {
            _highestvotecount = _linecandidates[_candidateindex].m_numvotes;
            _highestcandidateindex = _candidateindex;
        }
        ++_candidateindex;
    }while(_candidateindex < _candidatesize);
    if(_highestcandidateindex != -1)
    {
        _line = _linecandidates[_highestcandidateindex].m_line;
    }
    else
    {
        emit failedRansacCheck();
        return false;
    }


    return true;
}

bool Max::doRansacLineProcessing(TBILine &_line, const TBILinearRansac &_ransac, const std::vector<TBIPoint_Float> &_vector, const int _startvectorindex, const int _endvectorindex)
{
    _line.clear();

    if(_vector.size() < 50)
    {
        emit failedRansacCheck();
        return false;
    }


    std::vector<TBILinearRansacVotingStructure> _linecandidates;

    int _iteration = 0;
    int _pnt1index;
    int _pnt2index;
    int _votingindex = 0;
    float _distance = 0;

    const int _vectorsize = _vector.size();
    const int _requiredvotes = _ransac.getMinVotes();
    const int _totaliterations = _ransac.getIteration();
    const float _distancethreshold = _ransac.getDistanceThreshold();
    float _minanglefromhorizon = _ransac.getIdealAngle() - _ransac.getAllowedAngleVariance();
    float _maxanglefromhorizon = _ransac.getIdealAngle() + _ransac.getAllowedAngleVariance();
    float _idealanglefromhorizon = _ransac.getIdealAngle();

    if((_startvectorindex >= _vectorsize) || (_endvectorindex >= _vectorsize) || (_startvectorindex < 0) || (_endvectorindex <= _startvectorindex))
    {
        emit failedRansacCheck();
        return false;

    }

    //Do the Ransac
    do
    {
        //Instantiate the Voting Container
        TBILinearRansacVotingStructure _linecandidate;
        //Start Getting Two Random Points
        _pnt1index = randomInt(_startvectorindex, _endvectorindex);
        do
        {
            _pnt2index = randomInt(_startvectorindex, _endvectorindex);
        }while(_pnt1index == _pnt2index);
        //Build The Line From The Two Data Points
        _linecandidate.m_line = TBILine(_vector[_pnt1index], _vector[_pnt2index]);
        //If Horizonal Angle Check is Good Do The Voting.
        _linecandidate.m_angletohorizontal = _linecandidate.m_line.angleFromHorizontal();
        if((_linecandidate.m_angletohorizontal >= _minanglefromhorizon) && (_linecandidate.m_angletohorizontal <= _maxanglefromhorizon))
        {
            //Horizon Angle Check is Good Do The Voting
            _votingindex = 0;
            do
            {
                _distance = _linecandidate.m_line.distanceAbs(_vector[_votingindex]);
                if(_distance <= _distancethreshold)
                {
                    ++_linecandidate.m_numvotes;
                }
                ++_votingindex;
            }while(_votingindex < _vectorsize);
            //Process The Voting And Add the Candidate If Ok
            if(_linecandidate.m_numvotes >= _requiredvotes)
            {
                _linecandidates.push_back(_linecandidate);
            }
        }
        ++_iteration;
    }while(_iteration < _totaliterations);

    //Evaluate The Candidate List and set the Line if Ok
    if(_linecandidates.size() == 0)
    {
        emit failedRansacCheck();
        return false;
    }

    //Choose the Line Candidate That has the Most Votes
    const int _candidatesize = (int)_linecandidates.size();
    int _candidateindex = 0;
    int _highestvotecount = -1;
    int _highestcandidateindex = -1;
    do
    {

        if(_highestvotecount < _linecandidates[_candidateindex].m_numvotes)
        {
            _highestvotecount = _linecandidates[_candidateindex].m_numvotes;
            _highestcandidateindex = _candidateindex;
        }
        ++_candidateindex;
    }while(_candidateindex < _candidatesize);
    if(_highestcandidateindex != -1)
    {
        _line = _linecandidates[_highestcandidateindex].m_line;
    }
    else
    {
        emit failedRansacCheck();
        return false;
    }

}

bool Max::doRansacLineProcessing(Mat &_dst, TBILine &_line, TBILinearRansac &_ransac,
                                 float *_skeletalarray, int _start_index, int _end_index, Scalar _line_color)
{
    _line.clear();

    //std::vector<TBILineVotingStructure> _linecandidates;
    QList<TBILinearRansacVotingStructure> _linecandidates;
    int _index1;
    int _index2;
    float _distance;
    int _highestvote_index = 0;
    int _highest_vote_count = 0;
    int _iteration = 0;
    int _attempts=0;


    const int _total_iterations = _ransac.getIteration();
    const int _vote_threshold = _ransac.getMinVotes();
    const float _distance_threshold = _ransac.getDistanceThreshold();
    const float _min_angle_to_horizon = _ransac.getIdealAngle() - _ransac.getAllowedAngleVariance();
    const float _max_angle_to_horizon = _ransac.getIdealAngle() + _ransac.getAllowedAngleVariance();




    if(_dst.channels() != 3)
    {
        qDebug() << "Max::doRansacLineProcessing() requires a 3 channel Mat.";
        emit failedRansacCheck();
        return false;
    }

    //Find Line Candidates
    do
    {

        _attempts = 0;
        do
        {
            _index1 =  QRandomGenerator::system()->bounded(_start_index, _end_index);
            ++_attempts;
            if(_attempts == 1000)
            {
                emit failedRansacCheck();
                return false;
            }
        }while(_skeletalarray[_index1] == 0);
        _attempts = 0;
        do
        {
            _index2 = QRandomGenerator::system()->bounded(_start_index, _end_index);
            ++_attempts;
            if(_attempts == 1000)
            {
                emit failedRansacCheck();
                return false;
            }
        }while((_index2 == _index1) || (_skeletalarray[_index2]==0));


        _attempts = 0;
        TBILinearRansacVotingStructure _linecandidate;
        _linecandidate.m_line = TBILine((float)_index1, _skeletalarray[_index1], (float)_index2, _skeletalarray[_index2]);
        if(_linecandidate.m_line.isValid())
        {
            _linecandidate.m_angletohorizontal = _linecandidate.m_line.angleFromHorizontal();
            if(_linecandidate.m_angletohorizontal >= _min_angle_to_horizon)
            {
                if(_linecandidate.m_angletohorizontal <= _max_angle_to_horizon)
                {
                    _linecandidates.push_back(_linecandidate);

                }
            }
        }
        ++_iteration;
    }while(_iteration < _total_iterations);

    //Fail if There are No Candidates in VL List
    if(_linecandidates.size() == 0)
    {
        emit failedRansacCheck();
        return false;
    }


    //Count Votes
    _index1 = _start_index;
    _iteration = 0;
    do
    {
        TBIPoint_Float _pnt((float)_index1, m_skeletal_line_array[_index1]);
        _distance = _linecandidates[_iteration].m_line.distanceAbs(_pnt);
        if(_distance <= _distance_threshold)
        {
            ++_linecandidates[_iteration].m_numvotes;
            if(_linecandidates[_iteration].m_numvotes > _highest_vote_count)
            {
                _highest_vote_count = _linecandidates[_iteration].m_numvotes;
                _highestvote_index = _iteration;
            }
        }
        ++_index1;
        if(_index1 > _end_index)
        {
            _index1 = 0;
            ++_iteration;
        }
    }while(_iteration < (int)_linecandidates.size());

    //Fail if Vote Count Fails
    if(_highest_vote_count < _vote_threshold)
    {
        emit failedRansacCheck();
        return false;
    }

    //Set The Line
    _line = _linecandidates[_highestvote_index].m_line;
    _line.remakeLine(_start_index, _end_index);

    //Draw The Line
    _line.drawOnMat(_dst, _line_color, 1);

    return true;
}
*/

bool Max::setProjectedRansacLines(TBILine &_src_tsl_left, TBILine &_src_tsl_right, TBILine &_src_bwl_left, TBILine &_src_bwl_right,
                                  TBILine &_dst_tsl_left, TBILine &_dst_tsl_right, TBILine &_dst_bwl_left, TBILine &_dst_bwl_right)
{
    TBIPoint_Float _leftintersectionpnt;
    TBIPoint_Float _rightintersectionpnt;
    TBIPoint_Float _jointintersectionpnt;

    bool _int1 = _src_tsl_left.findPointofIntersection(_src_bwl_left, _leftintersectionpnt);
    bool _int2 = _src_tsl_right.findPointofIntersection(_src_bwl_right, _rightintersectionpnt);
    bool _int3 = _src_bwl_left.findPointofIntersection(_src_bwl_right, _jointintersectionpnt);
    if(_int1 && _int2 && _int3)
    {
        _dst_tsl_left.setPoint1(_src_tsl_left.getPoint1());
        _dst_tsl_left.setPoint2(_leftintersectionpnt);
        _dst_bwl_left.setPoint1(_leftintersectionpnt);
        _dst_bwl_left.setPoint2(_jointintersectionpnt);
        _dst_bwl_right.setPoint1(_jointintersectionpnt);
        _dst_bwl_right.setPoint2(_rightintersectionpnt);
        _dst_tsl_right.setPoint1(_rightintersectionpnt);
        _dst_tsl_right.setPoint2(_src_tsl_right.getPoint2());
    }
    else
    {
        return false;
    }

    return true;
}









//The Recieve New CV::Mat Method
//This Slot Handles A New Camera Frame
void Max::recieveNewCVMat(const Mat &_mat)
{


    if(m_in_proccesing_loop) return;
    m_in_proccesing_loop = true;

    //Start The Time in Loop Timer
    m_timer.start();

    //Make Sure Mat is Ok.
    if(_mat.channels() != 1)
    {
        qDebug()<<"Max::recieveNewCVMat() did not recieve a single Channel Mat";
        m_timeinloop = QString("Time in Loop: " + QString::number(m_timer.elapsed()) + " ms.-Camera FPS = " + QString::number(1000/m_timer.elapsed()));
        emit timeInLoopChanged(m_timeinloop);
        emit processingComplete(); //Must Be Last Signal Sent
        return;
    }
    if(!_mat.isContinuous())
    {
        qDebug()<<"Max::recieveNewCVMat() did not recieve a continuous Mat";
        m_timeinloop = QString("Time in Loop: " + QString::number(m_timer.elapsed()) + " ms.-Camera FPS = " + QString::number(1000/m_timer.elapsed()));
        emit timeInLoopChanged(m_timeinloop);
        emit processingComplete(); //Must Be Last Signal Sent
        return;
    }

    //Blank Internals
    blankProcessingArrays();

    //Declare Mats
    cv::Mat _raw_mat = _mat.clone();
    cv::Mat _blurr_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC1);
    cv::Mat _threshold_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC1);
    cv::Mat _pixelcolumn_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC1);
    cv::Mat _skel_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC1);
    cv::Mat _ransac_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC3); //Mats From Here Out are Color.
    cv::Mat _splitmerge_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC3);
    cv::Mat _operation_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC3);

    //Do OpenCV Proccesses
    cv::GaussianBlur(_raw_mat, _blurr_mat, Size(m_blur,m_blur), 0);
    cv::threshold(_blurr_mat, _threshold_mat, m_thresholdmin, m_thresholdmax, THRESH_TOZERO);


    cv::cvtColor(_raw_mat, _operation_mat, cv::COLOR_GRAY2BGR);
    blankProcessingArrays();

    if(clusterProcessThresholdMat(_threshold_mat, _pixelcolumn_mat, &m_total_image_intensity, m_max_image_intensity,
                                  m_min_image_intensity, m_min_cluster_size, m_max_cluster_size, m_max_clusterincol, *m_scan_ds))
    {
        m_scan_ds->drawToMat(_skel_mat);
        cv::cvtColor(_skel_mat, _ransac_mat, cv::COLOR_GRAY2BGR);
        //Build The Roughing TSL's
        m_scan_ds->buildDataSetForLeftTSL(*m_roughing_left_tsl_ds, 150);
        m_scan_ds->buildDataSetForRightTSL(*m_roughing_right_tsl_ds, 150);
        m_roughing_left_tsl_ds->drawToMat(_splitmerge_mat, CV_RGB(0,255,0));
        m_roughing_right_tsl_ds->drawToMat(_splitmerge_mat, CV_RGB(255, 0, 0));
        TBIRansac::doRansac(m_left_tsl, m_left_tsl_ransac, *m_roughing_left_tsl_ds);
        TBIRansac::doRansac(m_right_tsl, m_right_tsl_ransac, *m_roughing_right_tsl_ds);

        //Draw Them If They Are Valid
        if(m_left_tsl.isValid())
        {
            m_roughing_left_tsl_ds->buildDataSetForInliers(*m_left_tsl_inliers_ds, m_left_tsl, 1.0);
            m_left_tsl_inliers_ds->buildLeastSquareLine(m_left_tsl);
            if(m_left_tsl.isValid())
            {
                m_left_tsl.remakeLine(0, _raw_mat.cols);
                m_left_tsl.drawOnMat(_ransac_mat, CV_RGB(0,255,0));
            }
        }
        if(m_right_tsl.isValid())
        {
            m_roughing_right_tsl_ds->buildDataSetForInliers(*m_right_tsl_inliers_ds, m_right_tsl, 1.0);
            m_right_tsl_inliers_ds->buildLeastSquareLine(m_right_tsl);
            if(m_right_tsl.isValid())
            {

                m_right_tsl.remakeLine(0, _raw_mat.cols);
                m_right_tsl.drawOnMat(_ransac_mat, CV_RGB(255, 0, 0));
            }
        }

        //Start Masking The DataSets
        if(m_left_tsl.isValid() && m_right_tsl.isValid())
        {
            m_scan_ds->buildDataSetForJoint(*m_joint_ds, m_right_tsl, m_left_tsl, 5.0);
            m_joint_ds->drawToMat(_splitmerge_mat, CV_RGB(50,50,255));
        }

    }

    if(m_emitextramats)
    {
        emit newRawMatProcessed(_raw_mat);
        emit newBlurMatProcessed(_blurr_mat);
        emit newThresholdMatProcessed(_threshold_mat);
        emit newPixelColumnMatProcessed(_pixelcolumn_mat);
        emit newSkeletalMatProcessed(_skel_mat);
        emit newRansacMatProcessed(_ransac_mat);
        emit newSplitMergeMatProcessed(_splitmerge_mat);
    }

    emit newOperationMatProcessed(_operation_mat);

    m_timeinloop = m_timer.elapsed();
    qDebug() << "Time in Loop = " << m_timer.elapsed();
    emit timeInLoopChanged(m_timeinloop);
    emit totalImageIntensityChanged(m_total_image_intensity);
    m_in_proccesing_loop = false;

    emit processingComplete(); //Must Be Last Signal Sent

}





// Slot Handling Methods To Update Settings-------------------------------------------
void Max::onBlurChange(int _blur)
{
    m_blur = _blur;
}

void Max::onThresholdMinChange(int _min)
{
    m_thresholdmin = _min;
}

void Max::onThresholdMaxChange(int _max)
{
    m_thresholdmax = _max;
}

void Max::onMaxTIIChange(quint64 _tii)
{
    m_max_image_intensity = _tii;
}

void Max::onMinTIIChange(quint64 _tii)
{
    m_min_image_intensity = _tii;
}

void Max::onMaxClusterSizeChange(int _size)
{
    m_max_cluster_size = _size;
}

void Max::onMinClusterSizeChange(int _size)
{
    m_min_cluster_size = _size;
}

void Max::onMaxClustersInColChange(int _size)
{
    m_max_clusterincol = _size;
}

void Max::onMaxDiscontinuityChange(int _value)
{
    m_allowable_discontinuities = _value;
}









