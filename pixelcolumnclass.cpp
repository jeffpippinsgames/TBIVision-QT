#include "pixelcolumnclass.h"
#include <QDebug>
#include <math.h>

PixelColumnClass::PixelColumnClass()
{

}

int PixelColumnClass::size()
{
    return m_clusters.size();
}

void PixelColumnClass::clear()
{
    m_clusters.clear();
}

void PixelColumnClass::pushClusterToBack(PixelClusterClass _cluster)
{
    m_clusters.push_back(_cluster);
}

float PixelColumnClass::getCentroidofCluster(int _clusterindex)
{
    if(_clusterindex >= (int)m_clusters.size()) return -1;
    return m_clusters.at(_clusterindex).getRowCentroid();
}

void PixelColumnClass::drawToMat(cv::Mat &_dst)
{
    if(m_clusters.size() == 0) return;
    int _index = 0;
    do
    {
        m_clusters[_index].DrawToMat(_dst);
        ++_index;
    }while(_index < (int)m_clusters.size());
}

PixelColumnProcessingReturn_t  PixelColumnClass::pixelProccessColumn(cv::Mat &_src, uint _column_in_mat_to_process, quint64 *_tii,
                                                                     quint64 _max_tii, int _min_cluster_size, int _max_cluster_size, int _max_clusters_in_column)
{
    if(_src.channels() != 1)
    {
        qDebug() << "PixelColumnClass::pixelProccessColumn() Requires a Single Channel cv::Mat.";
        return PCP_FAILED_MAT_TYPE;
    }
    if(_column_in_mat_to_process > (uint)_src.cols)
    {
        qDebug() << "PixelColumnClass::pixelProccessColumn() _column_in_mat_to_process is out of range for cv::Mat.";
        return PCP_FAILED_COLUMN_OUT_OF_RANGE;
    }

    uint8_t *_data = _src.data;
    int _rowindex = 0;
    int _dataindex;
    int _value;
    PixelClusterClass _cluster;

    //Cluster Organization By Gausian Distribution
    do
    {
        _dataindex = (_rowindex * _src.cols) + _column_in_mat_to_process;
        _value = _data[_dataindex];
        if(_value > 0) //Process the pixel
        {
            //Add the pixel to the cluster if it it fits into the gausian model.
            if(_cluster.addPixelForGausianCluster(PixelFundamental_t(_rowindex, _column_in_mat_to_process, _data[_dataindex])))
            {
                //Pixel Was Added To Cluster
            }
            else //The Pixel did not fit into the gausian model
            {
                //The Cluster has elements but this pixel is not part of a gausian distribution
                //This marks the begining of a new cluster add it to the list, This Pixel marks the end of the cluster
                //It is ignored. The smallest possible gausian distribution is 3 pixels wide. The cluster size must be at least that big
                //if it is add it to the list
                if((_cluster.size() >= _min_cluster_size) && (_cluster.size() <=  _max_cluster_size))
                {
                    _cluster.setRowCentroid();
                    m_clusters.push_back(_cluster);
                }
                _cluster.clear();
            }
            *_tii += _value; //Incriment the Total Image Intensity
        }
        else //There is no pixel intensity at this pixel location
        {
            //Check to see if there is a constructed cluster
            //if there is add it the the list
            if((_cluster.size() >= _min_cluster_size) && (_cluster.size() <=  _max_cluster_size))
            {
                _cluster.setRowCentroid();
                m_clusters.push_back(_cluster);
            }
            _cluster.clear();
        }
        ++_rowindex;
        if(*_tii > _max_tii) return PCP_FAILED_MAX_TII;
    }while(_rowindex < _src.cols);

    /* Cluster Organization By 0 Intensity Values.
    //Build Clusters from 0 intensity values.
    do
    {

        _dataindex = (_rowindex * _src.cols) + _column_in_mat_to_process;
        if(_data[_dataindex] > 0)
        {
            *_tii += _data[_dataindex];
            _cluster.pushPixelToBack(PixelFundamental_t(_rowindex, _column_in_mat_to_process, _data[_dataindex]));
        }
        else
        {
            if((_cluster.size() >= _min_cluster_size) && (_cluster.size() <=  _max_cluster_size) && (_cluster.isGausian()))
            {
                _cluster.setRowCentroid();
                m_clusters.push_back(_cluster);

            }
            _cluster.clear();
        }
        ++_rowindex;
        if(*_tii > _max_tii) return PCP_FAILED_MAX_TII;
    }while(_rowindex < _src.rows);
*/

    //If there are too many clusters in the list reject the list.
    if((int)m_clusters.size() > _max_clusters_in_column)
    {
        m_clusters.clear();
    }

    return PCP_OK;
}

void PixelColumnClass::processForContinuity(float _targetrowcentroid, float _distancethreshold)
{
    if(m_clusters.size() == 0) return;

    int _index = 0;
    float _distance;
    float _mindistance = 9999999;
    int _minindex = -1;
    do
    {

        _distance = std::abs(m_clusters[_index].getRowCentroid() - _targetrowcentroid);
        if((_distance <= _distancethreshold) && (_distance < _mindistance))
        {
            _mindistance = _distance;
            _minindex = _index;
        }
        ++_index;
    }while(_index < (int)m_clusters.size());

    if(_minindex == -1)
    {
        m_clusters.clear();
        return;
    }

    PixelClusterClass _cluster(m_clusters[_minindex]);
    m_clusters.clear();
    m_clusters.push_back(_cluster);
}

int PixelColumnClass::getIndexOfClosestCentroid(float _targetcentroid, float _distancethreshold)
{
    if(m_clusters.size() == 0) return -1;
    if(_targetcentroid == -1) return -1;
    if(_distancethreshold < 0) return -1;

    int _index = 0;
    float _distance;
    float _mindistance = 9999999;
    int _minindex = -1;
    do
    {

        _distance = std::abs(m_clusters[_index].getRowCentroid() - _targetcentroid);
        if((_distance <= _distancethreshold) && (_distance < _mindistance))
        {
            _mindistance = _distance;
            _minindex = _index;
        }
        ++_index;
    }while(_index < (int)m_clusters.size());

   return _minindex;
}




































