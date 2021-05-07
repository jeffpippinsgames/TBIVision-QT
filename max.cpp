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

bool Max::doTSLProcessing(Mat &_dst)
{
    m_left_tsl = TBILine();
    m_right_tsl = TBILine();

    if(_dst.channels() != 3)
    {
        qDebug() << "Max: fillTopSurfaceLines() requires a 3 channel Mat.";
        emit failedTSLCheck();
        return false;
    }
    if(m_flattened_iohrv  <= 1)
    {
        emit failedTSLCheck();
        return false;
    }
    std::vector<TBILineVotingStructure> _leftlinecandidates;
    std::vector<TBILineVotingStructure> _rightlinecandidates;
    int _iteration = 0;
    int _index1;
    int _index2;
    float _distance;
    int _highestvote_index = 0;
    int _highest_vote_count = 0;

    //Fill The Candidate List
    //1. Get Two Random Points
    //2. Calc The Line Values
    //3 Determine The Votes.

    //Get Left Side TSL----------------------------------------------------------------
    do
    {
        //Left Side
        _index1 =  QRandomGenerator::global()->bounded(m_flattened_iohrv);
        do
        {
            _index2 = QRandomGenerator::global()->bounded(m_flattened_iohrv);
        }while(_index2 == _index1);
        TBILineVotingStructure _leftlinecandidate;
        _leftlinecandidate.m_line = TBILine((float)_index1, m_skeletal_line_array[_index1], (float)_index2, m_skeletal_line_array[_index2]);
        if(_leftlinecandidate.m_line.isValid())
        {
            _leftlinecandidate.m_angletohorizontal = _leftlinecandidate.m_line.angleFromHorizontal();
            if(_leftlinecandidate.m_angletohorizontal >= m_min_tslleft_angle)
            {
                if(_leftlinecandidate.m_angletohorizontal <= m_max_tslleft_angle)
                {
                    _leftlinecandidates.push_back(_leftlinecandidate);
                }
            }
        }
        ++_iteration;
    }while(_iteration <= m_tsl_iterations);

    //Fail if There are No Candidates in TSL List
    if(_leftlinecandidates.size() == 0)
    {
        emit failedTSLCheck();
        return false;
    }

    //Count Left Side Votes
    _index1 = 0;
    _index2 = 0;
    do
    {
        TBIPoint _pnt((float)_index2, m_skeletal_line_array[_index2]);
        _distance = _leftlinecandidates[_index1].m_line.getOrthogonalDistance(_pnt);
        if(_distance <= m_tsl_distance_threshold)
        {
            ++_leftlinecandidates[_index1].m_numvotes;
            if(_leftlinecandidates[_index1].m_numvotes > _highest_vote_count)
            {
                _highest_vote_count = _leftlinecandidates[_index1].m_numvotes;
                _highestvote_index = _index1;
            }
        }
        ++_index2;
        if(_index2 == m_flattened_iohrv)
        {
            _index2 = 0;
            ++_index1;
        }
    }while(_index1 < (int)_leftlinecandidates.size());

    //Fail if Vote Count Fails
    if(_highest_vote_count < m_min_tsl_votes)
    {
        emit failedTSLCheck();
        return false;
    }

    //Set Left TSL
    m_left_tsl = _leftlinecandidates[_highestvote_index].m_line;
    m_left_tsl.remakeLine(0, m_flattened_iohrv);

    //Start Right TSL---------------------------------------------------------------------
    _index1 = 0;
    _index2 = 0;
    _iteration = 0;
    _highest_vote_count = 0;
    _highestvote_index = 0;
    _distance = 0.0;

    //Get Right Side TSL
    do
    {
        //Left Side
        _index1 =  QRandomGenerator::global()->bounded(m_flattened_iohrv, _dst.cols-1);
        do
        {
            _index2 = QRandomGenerator::global()->bounded(m_flattened_iohrv, _dst.cols-1);
        }while(_index2 == _index1);
        TBILineVotingStructure _rightlinecandidate;
        _rightlinecandidate.m_line = TBILine((float)_index1, m_skeletal_line_array[_index1], (float)_index2, m_skeletal_line_array[_index2]);
        if(_rightlinecandidate.m_line.isValid())
        {
            _rightlinecandidate.m_angletohorizontal = _rightlinecandidate.m_line.angleFromHorizontal();
            if(_rightlinecandidate.m_angletohorizontal >= m_min_tslright_angle)
            {
                if(_rightlinecandidate.m_angletohorizontal <= m_max_tslright_angle)
                {
                    _rightlinecandidates.push_back(_rightlinecandidate);
                }
            }
        }
        ++_iteration;
    }while(_iteration <= m_tsl_iterations);

    //Fail if There are No Candidates in TSL List
    if(_rightlinecandidates.size() == 0)
    {
        emit failedTSLCheck();
        return false;
    }

    //Count Right Side Votes
    _index1 = 0;
    _index2 = m_flattened_iohrv;
    do
    {
        TBIPoint _pnt((float)_index2, m_skeletal_line_array[_index2]);
        _distance = _rightlinecandidates[_index1].m_line.getOrthogonalDistance(_pnt);
        if(_distance <= m_tsl_distance_threshold)
        {
            ++_rightlinecandidates[_index1].m_numvotes;
            if(_rightlinecandidates[_index1].m_numvotes > _highest_vote_count)
            {
                _highest_vote_count = _rightlinecandidates[_index1].m_numvotes;
                _highestvote_index = _index1;
            }
        }
        ++_index2;
        if(_index2 == _dst.cols)
        {
            _index2 = 0;
            ++_index1;
        }
    }while(_index1 < (int)_rightlinecandidates.size());

    //Fail if Vote Count Fails
    if(_highest_vote_count < m_min_tsl_votes)
    {
        emit failedTSLCheck();
        return false;
    }

    //Set Right TSL
    m_right_tsl = _rightlinecandidates[_highestvote_index].m_line;
    m_right_tsl.remakeLine(m_flattened_iohrv, _dst.cols-1);

    //Draw on Mat
    m_left_tsl.drawOnMat(_dst, cv::Scalar(0,200,0), 1);
    m_right_tsl.drawOnMat(_dst, cv::Scalar(200,0,0), 1);

    return true;
}

bool Max::doBWLProcessing(Mat &_dst)
{
    m_left_bwl = TBILine();
    m_right_bwl = TBILine();

    if(_dst.channels() != 3)
    {
        qDebug() << "Max: fillTopSurfaceLines() requires a 3 channel Mat.";
        emit failedTSLCheck();
        return false;
    }
    if(m_flattened_iohrv  <= 1)
    {
        emit failedTSLCheck();
        return false;
    }
    std::vector<TBILineVotingStructure> _leftlinecandidates;
    std::vector<TBILineVotingStructure> _rightlinecandidates;
    int _iteration = 0;
    int _index1;
    int _index2;
    float _distance;
    int _highestvote_index = 0;
    int _highest_vote_count = 0;

    //Fill The Candidate List
    //1. Get Two Random Points
    //2. Calc The Line Values
    //3 Determine The Votes.

    //Get Left Side BWL----------------------------------------------------------------
    do
    {
        //Left Side
        _index1 =  QRandomGenerator::global()->bounded(m_flattened_iohrv);
        do
        {
            _index2 = QRandomGenerator::global()->bounded(m_flattened_iohrv);
        }while(_index2 == _index1);
        TBILineVotingStructure _leftlinecandidate;
        _leftlinecandidate.m_line = TBILine((float)_index1, m_skeletal_line_array[_index1], (float)_index2, m_skeletal_line_array[_index2]);
        if(_leftlinecandidate.m_line.isValid())
        {
            _leftlinecandidate.m_angletohorizontal = _leftlinecandidate.m_line.angleFromHorizontal();
            if(_leftlinecandidate.m_angletohorizontal >= m_min_bwlleft_angle)
            {
                if(_leftlinecandidate.m_angletohorizontal <= m_max_bwlleft_angle)
                {
                    _leftlinecandidates.push_back(_leftlinecandidate);
                }
            }
        }
        ++_iteration;
    }while(_iteration <= m_bwl_iterations);

    //Fail if There are No Candidates in TSL List
    if(_leftlinecandidates.size() == 0)
    {
        emit failedTSLCheck();
        return false;
    }

    //Count Left Side Votes
    _index1 = 0;
    _index2 = 0;
    do
    {
        TBIPoint _pnt((float)_index2, m_skeletal_line_array[_index2]);
        _distance = _leftlinecandidates[_index1].m_line.getOrthogonalDistance(_pnt);
        if(_distance <= m_bwl_distance_threshold)
        {
            ++_leftlinecandidates[_index1].m_numvotes;
            if(_leftlinecandidates[_index1].m_numvotes > _highest_vote_count)
            {
                _highest_vote_count = _leftlinecandidates[_index1].m_numvotes;
                _highestvote_index = _index1;
            }
        }
        ++_index2;
        if(_index2 == m_flattened_iohrv)
        {
            _index2 = 0;
            ++_index1;
        }
    }while(_index1 < (int)_leftlinecandidates.size());

    //Fail if Vote Count Fails
    if(_highest_vote_count < m_min_bwl_votes)
    {
        emit failedTSLCheck();
        return false;
    }

    //Set Left BWL
    m_left_bwl = _leftlinecandidates[_highestvote_index].m_line;
    m_left_bwl.remakeLine(0, _dst.cols);

    //Start Right BWL---------------------------------------------------------------------
    _index1 = 0;
    _index2 = 0;
    _iteration = 0;
    _highest_vote_count = 0;
    _highestvote_index = 0;
    _distance = 0.0;

    //Get Right Side BWL
    do
    {
        //Left Side
        _index1 =  QRandomGenerator::global()->bounded(m_flattened_iohrv, _dst.cols-1);
        do
        {
            _index2 = QRandomGenerator::global()->bounded(m_flattened_iohrv, _dst.cols-1);
        }while(_index2 == _index1);
        TBILineVotingStructure _rightlinecandidate;
        _rightlinecandidate.m_line = TBILine((float)_index1, m_skeletal_line_array[_index1], (float)_index2, m_skeletal_line_array[_index2]);
        if(_rightlinecandidate.m_line.isValid())
        {
            _rightlinecandidate.m_angletohorizontal = _rightlinecandidate.m_line.angleFromHorizontal();
            if(_rightlinecandidate.m_angletohorizontal >= m_min_bwlright_angle)
            {
                if(_rightlinecandidate.m_angletohorizontal <= m_max_bwlright_angle)
                {
                    _rightlinecandidates.push_back(_rightlinecandidate);
                }
            }
        }
        ++_iteration;
    }while(_iteration <= m_bwl_iterations);

    //Fail if There are No Candidates in TSL List
    if(_rightlinecandidates.size() == 0)
    {
        emit failedTSLCheck();
        return false;
    }

    //Count Right Side Votes
    _index1 = 0;
    _index2 = m_flattened_iohrv;
    do
    {
        TBIPoint _pnt((float)_index2, m_skeletal_line_array[_index2]);
        _distance = _rightlinecandidates[_index1].m_line.getOrthogonalDistance(_pnt);
        if(_distance <= m_bwl_distance_threshold)
        {
            ++_rightlinecandidates[_index1].m_numvotes;
            if(_rightlinecandidates[_index1].m_numvotes > _highest_vote_count)
            {
                _highest_vote_count = _rightlinecandidates[_index1].m_numvotes;
                _highestvote_index = _index1;
            }
        }
        ++_index2;
        if(_index2 == _dst.cols)
        {
            _index2 = 0;
            ++_index1;
        }
    }while(_index1 < (int)_rightlinecandidates.size());

    //Fail if Vote Count Fails
    if(_highest_vote_count < m_min_bwl_votes)
    {
        emit failedTSLCheck();
        return false;
    }

    //Set Right BWL
    m_right_bwl = _rightlinecandidates[_highestvote_index].m_line;
    m_right_bwl.remakeLine(0, _dst.cols);

    //Draw on Mat
    m_left_bwl.drawOnMat(_dst, cv::Scalar(0,200,0), 1);
    m_right_bwl.drawOnMat(_dst, cv::Scalar(200,0,0), 1);

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


    blankProcessingArrays();
    if(doPixelColumnProcessing(_threshold_mat, _pixelcolumn_mat)) //The TII Check is Done Here
    {


        if(doSkeletonProcessing(_skel_mat)) //Fill Skeleton and Discontinuity Trap
        {

            cv::cvtColor(_skel_mat, _tsl_mat, cv::COLOR_GRAY2BGR);
            if(doTSLProcessing(_tsl_mat) && doBWLProcessing(_tsl_mat))
            {

                _operation_mat = _tsl_mat.clone();
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

    m_timeinloop = QString("Time in Loop: " + QString::number(m_timer.elapsed()) + " ms.-Camera FPS = " + QString::number(1000/m_timer.elapsed()));
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









