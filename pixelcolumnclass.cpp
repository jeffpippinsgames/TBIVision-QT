#include "pixelcolumnclass.h"

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
    if(_clusterindex >= m_clusters.size()) return -1;
    return m_clusters[_clusterindex].getRowCentroid();
}

void PixelColumnClass::drawToMat(cv::Mat &_dst)
{
    if(m_clusters.size() == 0) return;
    int _index = 0;
    do
    {
        m_clusters[_index].DrawToMat(_dst);
        ++_index;
    }while(_index < m_clusters.size());
}
