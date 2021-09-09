#include "tbiclass_opencvmatcontainer.h"


TBIClass_OpenCVMatContainer::TBIClass_OpenCVMatContainer()
{

}

void TBIClass_OpenCVMatContainer::initMats(const cv::Mat &_srcmat)
{
    m_raw = _srcmat.clone();
    m_blurr = cv::Mat::zeros(m_raw.rows, m_raw.cols, CV_8UC1);
    m_threshold = cv::Mat::zeros(m_raw.rows, m_raw.cols, CV_8UC1);
    m_gausiandecluster = cv::Mat::zeros(m_raw.rows, m_raw.cols, CV_8UC1);
    m_inliers = cv::Mat::zeros(m_raw.rows, m_raw.cols, CV_8UC3); //Mats From Here Out are Color.
    m_ransac = cv::Mat::zeros(m_raw.rows, m_raw.cols, CV_8UC3);
    m_geometricconstruction = cv::Mat::zeros(m_raw.rows, m_raw.cols, CV_8UC3);
    m_operation = cv::Mat::zeros(m_raw.rows, m_raw.cols, CV_8UC3);
}
