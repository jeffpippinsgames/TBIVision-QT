#ifndef PIXELCOLUMNCLASS_H
#define PIXELCOLUMNCLASS_H

#include "pixelclusterclass.h"
#include "pixelfundamental.h"
#include "opencv4/opencv2/core.hpp"
#include <vector>

class PixelColumnClass
{
public:
    PixelColumnClass();
    int size();
    void clear();
    void pushClusterToBack(PixelClusterClass _cluster);
    float getCentroidofCluster(int _clusterindex);
    void drawToMat(cv::Mat &_dst);


private:
    std::vector<PixelClusterClass> m_clusters;
};

#endif // PIXELCOLUMNCLASS_H
