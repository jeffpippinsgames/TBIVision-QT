#include "max.h"
#include <QDebug>

void Max::blankProcessingArrays()
{
    int _row=0;
    int _col=0;
    do
    {
        m_flattened_pixel_array[_col][_row] = 0;
        ++_row;
        if(_row == Mat_Max_Height)
        {
            m_skeletal_line_array[_col] = 0;
            _row = 0;
            ++_col;
        }
    }while(_col < Mat_Max_Width);
}

bool Max::fillFlattenedArray(Mat &_src)
{
    if(_src.channels() != 1)
    {
        qDebug()<<"Max: Error: fillFlattenedArray() Requires a Single Channel cv::Mat.";
        return false;
    }
    if(!_src.isContinuous())
    {
        qDebug()<<"Max: Error: fillFlattenedArray() Requires a Continuous cv::Mat.";
        return false;
    }

    m_flattened_rows = _src.rows;
    m_flattened_cols = _src.cols;
    m_total_image_intensity = 0;

    int _row=0;
    int _col=0;
    uint8_t* _data = (uint8_t*) _src.data;
    int _dataindex = 0;

    do
    {
        m_flattened_pixel_array[_col][_row] = _data[_dataindex];
        m_total_image_intensity = m_total_image_intensity + _data[_dataindex];
        ++_row;
        if(_row == m_flattened_rows)
        {
            _row = 0;
            ++_col;
        }
        _dataindex = (_row * m_flattened_cols) + _col;
    }while(_col < m_flattened_cols);
    return true;
}

Max::Max(QObject *parent) : QObject(parent)
{
    m_timeinloop = "Error:";
    m_blur = 3;
    m_thresholdmin = 100;
    m_thresholdmax = 255;
    m_max_image_intensity = 720*540*255*.3;
    m_min_image_intensity = 720*255;
    m_min_cluster_size = 3;
    m_max_cluster_size = 20;
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

    //Make Sure Mat is Ok.
    if(_mat.channels() != 1)
    {
        qDebug()<<"Max: recieveNewCvMat did not recieve a single Channel Mat";
        m_timeinloop = QString("Time in Loop: " + QString::number(m_timer.elapsed()) + " ms.-Camera FPS = " + QString::number(1000/m_timer.elapsed()));
        emit timeInLoopChanged(m_timeinloop);
        emit processingComplete(); //Must Be Last Signal Sent
        return;
    }
    if(!_mat.isContinuous())
    {
        qDebug()<<"Max: recieveNewCvMat did not recieve a continuous Mat";
        m_timeinloop = QString("Time in Loop: " + QString::number(m_timer.elapsed()) + " ms.-Camera FPS = " + QString::number(1000/m_timer.elapsed()));
        emit timeInLoopChanged(m_timeinloop);
        emit processingComplete(); //Must Be Last Signal Sent
        return;
    }

    //Blank Internals
    blankProcessingArrays();

    //Declare Mats
    cv::Mat _raw_mat = _mat.clone();
    cv::Mat _blurr_mat;
    cv::Mat _threshold_mat;
    cv::Mat _columite_mat;
    cv::Mat _skel_mat;
    cv::Mat _operation_mat;


    //Do OpenCV Proccesses
    emit newRawMatProcessed(_raw_mat);
    cv::GaussianBlur(_raw_mat, _blurr_mat, Size(m_blur,m_blur), 0);
    emit newBlurMatProcessed(_blurr_mat);
    cv::threshold(_blurr_mat, _threshold_mat, m_thresholdmin, m_thresholdmax, THRESH_TOZERO);
    emit newThresholdMatProcessed(_threshold_mat);

    //Start Building Internals and Doing Checks
    fillFlattenedArray(_threshold_mat);
    if(m_total_image_intensity > m_max_image_intensity)
    {
        emit totalImageIntensityExceeded();
        m_timeinloop = QString("Time in Loop: " + QString::number(m_timer.elapsed()) + " ms.-Camera FPS = " + QString::number(1000/m_timer.elapsed()));
        emit timeInLoopChanged(m_timeinloop);
        emit processingComplete(); //Must Be Last Signal Sent
        return;
    }




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



