#include "max.h"
#include <QDebug>

Max::Max(QObject *parent) : QObject(parent)
{
    qDebug()<<"Max: Max Object Created.";
}

Max::~Max()
{
    emit this->aboutToDestroy();
    qDebug()<<"Max: Max Object Destroyed";
}

void Max::recieveNewCVMat(const Mat &_mat)
{
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

    emit processingComplete(); //Must Be Last Signal Sent

}

