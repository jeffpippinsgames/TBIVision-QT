#include "pcolumn.h"

PColumn::PColumn(QObject *parent) : QObject(parent)
{

}

void PColumn::addCluster(PCluster &_cluster)
{


}

void PColumn::removeNonGausianClusters()
{
    //Start at the last cluster and work to 0;
    if(m_pcolumn.count() == 0) return;

    for(int i = m_pcolumn.count()-1; i >= 0; --i)
    {
        m_pcolumn.removeAt(i);
    }
}
