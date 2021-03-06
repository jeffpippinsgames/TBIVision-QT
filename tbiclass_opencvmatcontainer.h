#ifndef TBICLASS_OPENCVMATCONTAINER_H
#define TBICLASS_OPENCVMATCONTAINER_H

#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/imgproc/imgproc.hpp"
#include "tbiparameterclass_gausiandecluster.h"

class TBIClass_OpenCVMatContainer
{
public:
    TBIClass_OpenCVMatContainer();
    void initMats(const cv::Mat &_srcmat);

    cv::Mat m_raw;
    cv::Mat m_pre_blurr;
    cv::Mat m_erode;
    cv::Mat m_edge;
    cv::Mat m_post_blurr;
    cv::Mat m_threshold;
    cv::Mat m_gausiandecluster;
    cv::Mat m_inliers;
    cv::Mat m_ransac;
    cv::Mat m_geometricconstruction;
    cv::Mat m_operation;
    cv::Mat m_testmatgrey;
    cv::Mat m_testmatcolor;
};

#endif // TBICLASS_OPENCVMATCONTAINER_H
