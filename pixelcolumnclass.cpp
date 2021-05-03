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
