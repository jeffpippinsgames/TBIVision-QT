#include "pixelcolumnclass.h"
#include <QDebug>

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
    PixelClusterClass _cluster;

    //Start Building The Clusters
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

    if((int)m_clusters.size() > _max_clusters_in_column)
    {
        m_clusters.clear();
    }

    return PCP_OK;
}
