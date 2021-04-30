#include "pcluster.h"

PCluster::PCluster(QObject *parent) : QObject(parent)
{
    m_cluster.empty();
    m_totalclusterintensity = 0;
}

void PCluster::empty()
{
    m_cluster.empty();
    m_totalclusterintensity = 0;
}

void PCluster::addFundamental(PFundamental &_fundamental)
{
    PFundamental _fund;
    _fund.m_intensity = _fundamental.m_intensity;
    m_totalclusterintensity += _fund.m_intensity;
    _fund.m_row = _fundamental.m_row;
    _fund.m_col = _fundamental.m_col;
    m_cluster.append(_fund);
}

float PCluster::getIntensityCentroid()
{

    if(m_cluster.isEmpty()) return 0.0;
    float _weightsum = 0.0;
    float _sumrowweightproduct = 0.0;
    for(int i = 0; i < m_cluster.count(); ++i)
    {
        _weightsum += m_cluster[i].m_intensity;
        _sumrowweightproduct += m_cluster[i].m_intensity * m_cluster[i].m_row;
    }
    if(_weightsum==0.0) return 0.0;
    return _sumrowweightproduct/_weightsum;

}

bool PCluster::IsClusterGausian()
{
        if (m_cluster.count() == 0) return false;
        if (m_cluster.count() == 1) return true;
        int _lastintensity = m_cluster[0].m_intensity;
        int _nextintensity;
        bool _peaked=false;

        for(int i = 1; i < m_cluster.count(); ++i)
        {
            _nextintensity = m_cluster[i].m_intensity;
           if(!_peaked && (_nextintensity < _lastintensity)) _peaked = true;
           else if(_peaked && (_nextintensity > _lastintensity)) return false;
           _lastintensity = _nextintensity;
        }
        return true;
}

