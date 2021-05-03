#ifndef PIXELCOLUMNCLASS_H
#define PIXELCOLUMNCLASS_H

#include "pixelclusterclass.h"
#include "pixelfundamental.h"
#include <vector>

class PixelColumnClass
{
public:
    PixelColumnClass();
    int size();
    void clear();
    void pushClusterToBack(PixelClusterClass _cluster);

private:
    std::vector<PixelClusterClass> m_clusters;
};

#endif // PIXELCOLUMNCLASS_H
