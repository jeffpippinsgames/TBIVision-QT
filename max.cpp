#include "max.h"
#include <QDebug>
#include "pixelcolumnclass.h"
#include "pixelclusterclass.h"
#include "pixelfundamental.h"
#include "TBITopSurfaceLineVotingStructure.h"
#include <QRandomGenerator>

//Constructors and Destructor--------------------------------------------------------
Max::Max(QObject *parent) : QObject(parent)
{
    m_in_proccesing_loop = false;
    m_timeinloop = "Error:";
    m_blur = 3;
    m_thresholdmin = 100;
    m_thresholdmax = 255;
    m_max_image_intensity = 720*540*255*.3;
    m_min_image_intensity = 720*255;
    m_min_cluster_size = 6;
    m_max_cluster_size = 75;
    m_allowable_discontinuities = 20;
    m_max_tslleft_angle = 10;
    m_min_tslleft_angle = -10;
    m_max_tslright_angle = 10;
    m_min_tslright_angle = -10;
    m_min_tsl_votes = 20;
    m_tsl_iterations = 75;
    m_tsl_distance_threshold = 1;
    m_max_bwlleft_angle = -20;
    m_min_bwlleft_angle = -60;
    m_max_bwlright_angle = 60;
    m_min_bwlright_angle = 20;
    m_min_bwl_votes = 20;
    m_bwl_iterations = 75;
    m_bwl_distance_threshold = 1;

    emit timeInLoopChanged(m_timeinloop);
    qDebug()<<"Max: Max Object Created.";
}

Max::~Max()
{
    emit this->aboutToDestroy();
    qDebug()<<"Max: Max Object Destroyed";
}

//Processing Methods------------------------------------------------------------------
void Max::blankProcessingArrays()
{

    int _col=0;
    do
    {
        m_cluster_columns[_col].clear();
        m_skeletal_line_array[_col] = 0;
        ++_col;
    }while(_col < Mat_Max_Width);

    m_left_tsl.clear();
    m_right_tsl.clear();
    m_right_bwl.clear();
    m_left_bwl.clear();
    m_topography_lines.clear();
    m_flattened_iohrv = 0;
}

bool Max::doPixelColumnProcessing(Mat &_src, Mat &_dst)
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
        _srcindex = (_row * _src.cols)+_col;
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
        if(_row == _src.rows)
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
    }while(_col < _src.cols);

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
    }while(_col < _src.cols);

    //Draw The Clusters in a New Mat
    _col = 0;
    do
    {
        m_cluster_columns[_col].drawToMat(_dst);
        ++_col;
    }while(_col < _src.cols);

    emit totalImageIntensityChanged(m_total_image_intensity);
    return true;
}

bool Max::doSkeletonProcessing(Mat &_dst)
{
    uint8_t* _data = (uint8_t*)_dst.data;
    int _dataindex;
    int _index = 0;
    float _rowcentroid;
    int _empty_cols = 0;
    float _highestrowvalue = 0.0;

    do
    {
        if(m_cluster_columns[_index].size() == 1) //One Cluster Pop it
        {
            _rowcentroid = m_cluster_columns[_index].getCentroidofCluster(0);
            m_skeletal_line_array[_index] = _rowcentroid;
            if(_rowcentroid > _highestrowvalue)
            {
                _highestrowvalue = _rowcentroid;
                m_flattened_iohrv = _index;
            }
            emit skeletalArrayChanged(_rowcentroid, _index);
            _dataindex = ((int)_rowcentroid * _dst.cols) + _index;
            _data[_dataindex] = 255;
        }
        else
        {
            emit skeletalArrayChanged(0.0, _index);
            ++_empty_cols;
        }
        ++_index;
    }while(_index < _dst.cols);

    if(_empty_cols > m_allowable_discontinuities)
    {
        emit failedDiscontinuityCheck();
        return false;
    }


    return true;
}

bool Max::doVotingLineProcessing(Mat &_dst, TBILine &_line, int _total_iterations, int _vote_threshold,
                                 float _distance_threshold, float _min_angle_to_horizon, float _max_angle_to_horizon,
                                 float *_skeletalarray, int _start_index, int _end_index, Scalar _line_color)
{
    _line.clear();

    //std::vector<TBILineVotingStructure> _linecandidates;
    QList<TBILineVotingStructure> _linecandidates;
    int _index1;
    int _index2;
    float _distance;
    int _highestvote_index = 0;
    int _highest_vote_count = 0;
    int _iteration = 0;

    if(_dst.channels() != 3)
    {
        qDebug() << "Max: doVotingLineProcessing() requires a 3 channel Mat.";
        emit failedVLCheck();
        return false;
    }

    //Find Line Candidates
    do
    {
        _index1 =  QRandomGenerator::system()->bounded(_start_index, _end_index);
        do
        {
            _index2 = QRandomGenerator::system()->bounded(_start_index, _end_index);
        }while(_index2 == _index1);
        TBILineVotingStructure _linecandidate;
        _linecandidate.m_line = TBILine((float)_index1, _skeletalarray[_index1], (float)_index2, _skeletalarray[_index2]);
        if(_linecandidate.m_line.isValid())
        {
            _linecandidate.m_angletohorizontal = _linecandidate.m_line.angleFromHorizontal();
            if(_linecandidate.m_angletohorizontal >= _min_angle_to_horizon)
            {
                if(_linecandidate.m_angletohorizontal <= _max_angle_to_horizon)
                {
                    _linecandidates.push_back(_linecandidate);

                }
            }
        }
        ++_iteration;
    }while(_iteration < _total_iterations);

    //Fail if There are No Candidates in VL List
    if(_linecandidates.size() == 0)
    {
        emit failedVLCheck();
        return false;
    }


    //Count Votes
    _index1 = _start_index;
    _iteration = 0;
    do
    {
        TBIPoint _pnt((float)_index1, m_skeletal_line_array[_index1]);
        _distance = _linecandidates[_iteration].m_line.getOrthogonalDistance(_pnt);
        if(_distance <= _distance_threshold)
        {
            ++_linecandidates[_iteration].m_numvotes;
            if(_linecandidates[_iteration].m_numvotes > _highest_vote_count)
            {
                _highest_vote_count = _linecandidates[_iteration].m_numvotes;
                _highestvote_index = _iteration;
            }
        }
        ++_index1;
        if(_index1 > _end_index)
        {
            _index1 = 0;
            ++_iteration;
        }
    }while(_iteration < (int)_linecandidates.size());

    //Fail if Vote Count Fails
    if(_highest_vote_count < _vote_threshold)
    {
        emit failedVLCheck();
        return false;
    }

    //Set The Line
    _line = _linecandidates[_highestvote_index].m_line;
    _line.remakeLine(_start_index, _end_index);

    //Draw The Line
     _line.drawOnMat(_dst, _line_color, 1);

     return true;
}

//The Recieve New CV::Mat Method
void Max::recieveNewCVMat(const Mat &_mat)
{

    if(m_in_proccesing_loop) return;
    m_in_proccesing_loop = true;

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
    cv::Mat _tsl_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC3); //Mats From Here Out are Color.
    cv::Mat _topography_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC3);
    cv::Mat _operation_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC3);

    //Do OpenCV Proccesses
    cv::GaussianBlur(_raw_mat, _blurr_mat, Size(m_blur,m_blur), 0);
    cv::threshold(_blurr_mat, _threshold_mat, m_thresholdmin, m_thresholdmax, THRESH_TOZERO);

    cv::cvtColor(_raw_mat, _operation_mat, cv::COLOR_GRAY2BGR);
    blankProcessingArrays();
    if(doPixelColumnProcessing(_threshold_mat, _pixelcolumn_mat)) //The TII Check is Done Here
    {


        if(doSkeletonProcessing(_skel_mat)) //Fill Skeleton and Discontinuity Trap
        {

            cv::cvtColor(_skel_mat, _tsl_mat, cv::COLOR_GRAY2BGR);
            bool _left_tsl_good = doVotingLineProcessing(_tsl_mat, m_left_tsl, m_tsl_iterations, m_min_tsl_votes, m_tsl_distance_threshold,
                                                         m_min_tslleft_angle, m_max_tslleft_angle, m_skeletal_line_array, 0, m_flattened_iohrv,
                                                         CV_RGB(0,200,0));
            bool _right_tsl_good = doVotingLineProcessing(_tsl_mat, m_right_tsl, m_tsl_iterations, m_min_tsl_votes, m_tsl_distance_threshold,
                                                          m_min_tslright_angle, m_max_tslright_angle, m_skeletal_line_array, m_flattened_iohrv, _skel_mat.cols,
                                                          CV_RGB(200,0,0));
            bool _left_bwl_good = doVotingLineProcessing(_tsl_mat, m_left_bwl, m_bwl_iterations, m_min_bwl_votes, m_bwl_distance_threshold,
                                                         m_min_bwlleft_angle, m_max_bwlleft_angle, m_skeletal_line_array, 0, m_flattened_iohrv,
                                                         CV_RGB(0,200,0));
            bool _right_bwl_good = doVotingLineProcessing(_tsl_mat, m_right_bwl, m_bwl_iterations, m_min_bwl_votes, m_bwl_distance_threshold,
                                                          m_min_bwlright_angle, m_max_bwlright_angle, m_skeletal_line_array, m_flattened_iohrv, _skel_mat.cols,
                                                          CV_RGB(200,0,0));

            if(_left_tsl_good && _right_tsl_good && _left_bwl_good && _right_bwl_good)
            {


                TBIPoint _intersectionpnt;
                if(m_left_tsl.findPointofIntersection(m_left_bwl, _intersectionpnt))
                {
                    cv::drawMarker(_operation_mat, cv::Point((int)_intersectionpnt.getX(), (int)_intersectionpnt.getY()),
                                  CV_RGB(0,200,200), cv::MARKER_CROSS, 20, 2);
                }
                if(m_right_tsl.findPointofIntersection(m_right_bwl, _intersectionpnt))
                {
                    cv::drawMarker(_operation_mat, cv::Point((int)_intersectionpnt.getX(), (int)_intersectionpnt.getY()),
                                  CV_RGB(0,200,200), cv::MARKER_CROSS, 20, 2);
                }


            }
            else
            {
                _tsl_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC3);           
            }

        }
        else
        {
            _skel_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8SC1);
        }

    }
    else
    {
      _pixelcolumn_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8SC1);
    }
    //Time The Loop.


    emit newRawMatProcessed(_raw_mat);
    emit newBlurMatProcessed(_blurr_mat);
    emit newThresholdMatProcessed(_threshold_mat);
    emit newPixelColumnMatProcessed(_pixelcolumn_mat);
    emit newSkeletalMatProcessed(_skel_mat);
    emit newTSLMatProcessed(_tsl_mat);
    emit newOperationMatProcessed(_operation_mat);

    m_timeinloop = QString("Time in Loop: " + QString::number(m_timer.elapsed()));
    emit timeInLoopChanged(m_timeinloop);
    m_in_proccesing_loop = false;
    emit processingComplete(); //Must Be Last Signal Sent

}

// Slot Handling Methods To Update Settings-------------------------------------------
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

void Max::onMaxDiscontinuityChange(int _value)
{
    m_allowable_discontinuities = _value;
}









