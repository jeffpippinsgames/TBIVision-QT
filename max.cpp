#include "max.h"
#include <QDebug>

Max::Max(QObject *parent) : QObject(parent)
{
    m_timeinloop = "Error:";
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
    cv::GaussianBlur(_raw_mat, _blurr_mat, Size(25,25), 0);
    emit newBlurMatProcessed(_blurr_mat);
    cv::threshold(_blurr_mat, _threshold_mat, 100, 255, THRESH_TOZERO);
    emit newThresholdMatProcessed(_threshold_mat);



    //Time The Loop.
    m_timeinloop = QString("Time in Loop: " + QString::number(m_timer.elapsed()) + " ms.-Camera FPS = " + QString::number(1000/m_timer.elapsed()));
    emit timeInLoopChanged(m_timeinloop);

    emit processingComplete(); //Must Be Last Signal Sent
}

