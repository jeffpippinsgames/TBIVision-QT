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
    float getRowCentroid(){return m_rowcentroid;}
    void setRowCentroid();
    bool isGausian();
    void clear();
    int size();
    void DrawToMat(cv::Mat &_mat);
    void EraseFromMat(cv::Mat &_mat);
    int getColumn();
    bool addPixelForGausianCluster(PixelFundamental_t _pixel);
    int getHighestInstensityPixelValue();
    int getHighestIntensityPixelIndex();


private:
     std::vector<PixelFundamental_t> m_pixels;
     float m_rowcentroid;
     int m_highestintensityvalue;
     int m_highestintensityindex;
};

#endif // PIXELCLUSTERCLASS_H
