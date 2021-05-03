#ifndef PIXELCLUSTERCLASS_H
#define PIXELCLUSTERCLASS_H

#include "pixelfundamental.h"
#include "iostream"
#include "vector"
#include "opencv4/opencv2/core.hpp"

class PixelClusterClass
{
public:
    PixelClusterClass();
    void pushPixelToBack(PixelFundamental_t _pixel);
    float getRowCentroid();
    bool isGausian();
    void clear();
    int size();
    void ClusterToMat(cv::Mat &_mat);


private:
     std::vector<PixelFundamental_t> m_pixels;
};

#endif // PIXELCLUSTERCLASS_H
