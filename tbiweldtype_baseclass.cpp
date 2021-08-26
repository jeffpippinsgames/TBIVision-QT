#include "tbiweldtype_baseclass.h"

tbiweldtype_baseclass::tbiweldtype_baseclass(QObject *parent) : QObject(parent)
{

}

void tbiweldtype_baseclass::doDeGausianClustering(const cv::Mat _incommingmat)
{

    if(_incommingmat.channels() != 1)
    {
        qDebug()<<"Max::processVGrooveTracking did not recieve a single Channel Mat";
        m_timeinloop = QString("Time in Loop: " + QString::number(m_timer.elapsed()) + " ms.-Camera FPS = " + QString::number(1000/m_timer.elapsed()));
        emit timeInLoopChanged(m_timeinloop);
        emit processingComplete(); //Must Be Last Signal Sent
        return;
    }
    if(!_mat.isContinuous())
    {
        qDebug()<<"Max::processVGrooveTracking() did not recieve a continuous Mat";
        m_timeinloop = QString("Time in Loop: " + QString::number(m_timer.elapsed()) + " ms.-Camera FPS = " + QString::number(1000/m_timer.elapsed()));
        emit timeInLoopChanged(m_timeinloop);
        emit processingComplete(); //Must Be Last Signal Sent
        return;
    }

   //Release Mats
    m_raw_mat.release();
    m_blurr_mat.release();
    m_threshold_mat.release();
    m_gausiandecluster_mat.release();

    //Reestablish Mats
    m_raw_mat = _incommingmat.clone();
    m_blurr_mat = cv::Mat::zeros(m_raw_mat.rows, m_raw_mat.cols, CV_8UC1);
    m_threshold_mat = cv::Mat::zeros(m_raw_mat.rows, m_raw_mat.cols, CV_8UC1);
    m_gausiandecluster_mat = cv::Mat::zeros(m_raw_mat.rows, m_raw_mat.cols, CV_8UC1);

    //

}
