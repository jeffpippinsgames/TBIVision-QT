#ifndef TBICLASS_OPENCVMATCONTAINER_H
#define TBICLASS_OPENCVMATCONTAINER_H

#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/imgproc/imgproc.hpp"

class TBIClass_OpenCVMatContainer
{
public:
    TBIClass_OpenCVMatContainer();
    void initMats(const cv::Mat &_srcmat);

    cv::Mat m_raw;
    cv::Mat m_blurr;
    cv::Mat m_threshold;
    cv::Mat m_gausiandecluster;
    cv::Mat m_inliers;
    cv::Mat m_ransac;
    cv::Mat m_geometricconstruction;
    cv::Mat m_operation;
};

#endif // TBICLASS_OPENCVMATCONTAINER_H
