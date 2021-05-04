#include "max.h"
#include <QDebug>
#include "pixelcolumnclass.h"
#include "pixelclusterclass.h"
#include "pixelfundamental.h"

void Max::blankProcessingArrays()
{

    int _col=0;
    do
    {
        m_cluster_columns[_col].clear();
        m_skeletal_line_array[_col] = 0;
        ++_col;
    }while(_col < Mat_Max_Width);
}

bool Max::fillColumnClusterArray(Mat &_src, Mat &_dst)
{
    m_total_image_intensity = 0;

    uint8_t * _srcdata = (uint8_t*)_src.data;
    int _srcindex = 0;
    int _row = 0;
    int _col = 0;
    PixelFundamental_t _pixel;
    PixelClusterClass _cluster;

    //Fill the PixelColumn Array
    do
    {
        if(m_total_image_intensity > m_max_image_intensity)
        {
            emit totalImageIntensityChanged(m_total_image_intensity);
            emit failedTIICheck();
            return false;
        }
        _srcindex = (_row * m_flattened_cols)+_col;
        if(_srcdata[_srcindex] > 0) //Start The Cluster Count.
        {
            _pixel.col = _col;
            _pixel.row = _row;
            _pixel.intensity = _srcdata[_srcindex];
            _cluster.pushPixelToBack(_pixel);
            m_total_image_intensity += _srcdata[_srcindex];
        }
        if(_srcdata[_srcindex] == 0) //Check to see if We need to Build a Cluster List
        {
            if((_cluster.size() >= m_min_cluster_size) && (_cluster.size() <= m_max_cluster_size))
            {
                if(_cluster.isGausian()) m_cluster_columns[_col].pushClusterToBack(_cluster);
            }
            _cluster.clear();
        }

        ++_row;
        if(_row == m_flattened_rows)
        {
            //Check to see if this leftover cluster needs to be added.
            if((_cluster.size() >= m_min_cluster_size) && (_cluster.size() <= m_max_cluster_size))
            {
                if(_cluster.isGausian()) m_cluster_columns[_col].pushClusterToBack(_cluster);
            }
            _cluster.clear();
            _row = 0;
            ++_col;
        }
    }while(_col < m_flattened_cols);

    //Check TII For Min Requirement
    if(m_total_image_intensity < m_min_image_intensity)
    {
        emit totalImageIntensityChanged(m_total_image_intensity);
        emit failedTIICheck();
        return false;
    }

    //Remove Any Clusters That Do Not Meet The Max Cluster in Col Check
    _col = 0;
    do
    {
        if(m_cluster_columns[_col].size() > m_max_clusterincol)
        {
            m_cluster_columns[_col].clear();
        }
        ++_col;
    }while(_col < m_flattened_cols);

    //Draw The Clusters in a New Mat
    _col = 0;
    do
    {
        m_cluster_columns[_col].drawToMat(_dst);
        ++_col;
    }while(_col < m_flattened_cols);

    emit totalImageIntensityChanged(m_total_image_intensity);
    return true;
}

bool Max::fillSkeleton(Mat _dst)
{
    uint8_t* _data = (uint8_t*)_dst.data;
    int _dataindex;
    int _index = 0;
    float _rowcentroid;
    int _empty_cols = 0;

    do
    {
        if(m_cluster_columns[_index].size() == 1) //One Cluster Pop it
        {
            _rowcentroid = m_cluster_columns[_index].getCentroidofCluster(0);
            m_skeletal_line_array[_index] = _rowcentroid;
            _dataindex = ((int)_rowcentroid * m_flattened_cols) + _index;
            _data[_dataindex] = 255;
        }
        else
        {
            ++_empty_cols;
        }
        ++_index;
    }while(_index < m_flattened_cols);

    if(_empty_cols > m_allowable_discontinuities)
    {
        emit failedDiscontinuityCheck();
        return false;
    }

    return true;
}

bool Max::updateFlattenedMembers(Mat _src)
{
    if(_src.channels() != 1)
    {
        qDebug() << "Max: updateFlattenedMembers requires a Single Channel Mat.";
        return false;
    }
    m_flattened_cols = _src.cols;
    m_flattened_rows = _src.rows;
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
    m_min_cluster_size = 6;
    m_max_cluster_size = 75;
    m_allowable_discontinuities = 20;
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
    cv::Mat _blurr_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC1);
    cv::Mat _threshold_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC1);
    cv::Mat _pixelcolumn_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC1);
    cv::Mat _skel_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC1);
    cv::Mat _operation_mat;

    //Do OpenCV Proccesses
    emit newRawMatProcessed(_raw_mat);
    cv::GaussianBlur(_raw_mat, _blurr_mat, Size(m_blur,m_blur), 0);
    emit newBlurMatProcessed(_blurr_mat);
    cv::threshold(_blurr_mat, _threshold_mat, m_thresholdmin, m_thresholdmax, THRESH_TOZERO);
    emit newThresholdMatProcessed(_threshold_mat);

    //Pixel Column Processing and TII Trap.
    blankProcessingArrays();
    if(updateFlattenedMembers(_threshold_mat))
    {
        if(fillColumnClusterArray(_threshold_mat, _pixelcolumn_mat)) //The TII Check is Done Here
        {
            emit newPixelColumnMatProcessed(_pixelcolumn_mat);
            if(fillSkeleton(_skel_mat)) //Fill Skeleton and Discontinuity Trap
            {
                emit newSkeletalMatProcessed(_skel_mat);
            }
            else
            {
                _skel_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8SC1);
                emit newSkeletalMatProcessed(_skel_mat);
            }
        }
        else
        {
            _pixelcolumn_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8SC1);
            emit newPixelColumnMatProcessed(_pixelcolumn_mat);
            emit newSkeletalMatProcessed(_skel_mat);
        }
    }
    else
    {
        emit newPixelColumnMatProcessed(_pixelcolumn_mat);
        emit newSkeletalMatProcessed(_skel_mat);
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

void Max::onMaxTIIChange(quint64 _tii)
{
    m_max_image_intensity = _tii;
}

void Max::onMinTIIChange(quint64 _tii)
{
    m_min_image_intensity = _tii;
}

void Max::onMaxClusterSizeChange(int _size)
{
    m_max_cluster_size = _size;
}

void Max::onMinClusterSizeChange(int _size)
{
    m_min_cluster_size = _size;
}

void Max::onMaxClustersInColChange(int _size)
{
    m_max_clusterincol = _size;
}









