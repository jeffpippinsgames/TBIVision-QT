#include "max.h"
#include <QDebug>

bool Max::FlattenMat(cv::Mat &_src, cv::Mat &_dst)
{
    _dst = _src.clone();
    uint8_t *_pixels = (uint8_t*)_dst.data;
    int _rows = _dst.rows;
    int _cols = _dst.cols;
    int _total_bytes = _rows*_cols;
    int _crow = 0;
    int _ccol = 0;
    int _cbyte = 0;

    m_current_image_intensity = 0;
    while(_cbyte < _total_bytes)
    {
       m_current_image_intensity += (unsigned long)_pixels[_cbyte];
       if(m_current_image_intensity > m_max_image_intensity) return false;



       ++_cbyte;
    }





}

Max::Max(QObject *parent) : QObject(parent)
{
    m_timeinloop = "Error:";
    m_blur = 3;
    m_thresholdmin = 100;
    m_thresholdmax = 255;
    m_max_image_intensity = 720*540*255*.3;
    m_min_image_intensity = 720*255;
    m_current_image_intensity = 0;
    emit timeInLoopChanged(m_timeinloop);
    qDebug()<<"Max: Max Object Created.";
}

Max::~Max()
{
    emit this->aboutToDestroy();
    qDebug()<<"Max: Max Object Destroyed";
}

void Max::recieveNewCVMat(const Mat &_mat)
{
    //Start The Time in Loop Timer
    m_timer.start();

    cv::Mat _raw_mat = _mat.clone();
    cv::Mat _blurr_mat;
    cv::Mat _threshold_mat;
    cv::Mat _columnite_mat;
    cv::Mat _skel_mat;
    cv::Mat _operation_mat;

    emit newRawMatProcessed(_raw_mat);
    cv::GaussianBlur(_raw_mat, _blurr_mat, Size(m_blur,m_blur), 0);
    emit newBlurMatProcessed(_blurr_mat);
    cv::threshold(_blurr_mat, _threshold_mat, m_thresholdmin, m_thresholdmax, THRESH_TOZERO);
    emit newThresholdMatProcessed(_threshold_mat);



    //Time The Loop.
    m_timeinloop = QString("Time in Loop: " + QString::number(m_timer.elapsed()) + " ms.-Camera FPS = " + QString::number(1000/m_timer.elapsed()));
    emit timeInLoopChanged(m_timeinloop);

    emit processingComplete(); //Must Be Last Signal Sent
}

void Max::onBlurChange(int _blur)
{
    m_blur = _blur;
}

void Max::onThresholdMinChange(int _min)
{
    m_thresholdmin = _min;
}

void Max::onThresholdMaxChange(int _max)
{
    m_thresholdmax = _max;
}



