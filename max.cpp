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
    m_emitextramats = false;

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

    m_max_tslleft_angle = 5;
    m_min_tslleft_angle = -5;
    m_min_tslleft_votes = 20;
    m_tslleft_iterations = 50;
    m_tslleft_distance_threshold = 1;

    m_max_tslright_angle = 5;
    m_min_tslright_angle = -5;
    m_min_tslright_votes = 20;
    m_tslright_iterations = 50;
    m_tslright_distance_threshold = 1;

    m_max_bwlleft_angle = -30;
    m_min_bwlleft_angle = -60;
    m_min_bwlleft_votes = 20;
    m_bwlleft_iterations = 50;
    m_bwlleft_distance_threshold = 1;

    m_max_bwlright_angle = 60;
    m_min_bwlright_angle = 30;
    m_min_bwlright_votes = 20;
    m_bwlright_iterations = 50;
    m_bwlright_distance_threshold = 1;

    m_sm_distance_threshold = 5;
    m_sm_length_threshold = 20;

    emit timeInLoopChanged(m_timeinloop);
    qDebug()<<"Max::Max() Max Object Created.";
}

Max::~Max()
{
    emit this->aboutToDestroy();
    qDebug()<<"Max::~Max() Max Object Destroyed";
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

bool Max::doPixelColumnProcessing(Mat &_src, Mat &_dst, PixelColumnClass *_pixel_column_array, quint64 *_tii, quint64 _max_tii,
                                  quint64 _min_tii, int _min_cluster_size, int _max_cluster_size, int _max_clusters_in_column)
{
    *_tii = 0;
    int _column_index = 0;
    PixelColumnProcessingReturn_t _val;
    int _cont_index_start = -1;

    //Create The Column List.
    do
    {
        _val = _pixel_column_array[_column_index].pixelProccessColumn(_src, _column_index, _tii, _max_tii,
                                                                      _min_cluster_size, _max_cluster_size, _max_clusters_in_column);
        if(_val == PCP_FAILED_MAX_TII)
        {
            emit totalImageIntensityChanged(m_total_image_intensity);
            emit failedTIICheck();
            return false;
        }
        else if(_val == PCP_OK)
        {
            _pixel_column_array[_column_index].drawToMat(_dst);
        }
        ++_column_index;

    }while(_column_index < _src.cols);

    //Check For Min Total Image Intensity
    if(*_tii < _min_tii)
    {
        emit totalImageIntensityChanged(m_total_image_intensity);
        emit failedTIICheck();
        return false;
    }





    //No Starting Continuity Location.
    //Continuity faled. Return false;
    if(_cont_index_start == -1)
    {
        return false;
    }

    //start the right side continuity check.

    emit totalImageIntensityChanged(m_total_image_intensity);
    return true;

}
bool Max::doSkeletonProcessing(Mat &_dst, PixelColumnClass *_pixel_column_array, float *_skel_array, float _continuity_threshold)
{
    uint8_t* _data = (uint8_t*)_dst.data;
    int _closest_index_left = -1;
    int _closest_index_right = -1;
    int _total_discontinuities = 0;
    //int _empty_cols = 0;
    int _index = 0;
     bool _added_point = false;
    int _dataindex;
    //float _rowcentroid;
    //float _highestrowvalue = 0.0;
    //const float _max_continuity = 2;





    //Build initial skeleton with a single cluster---------------------------------------
    do
    {
        if(_pixel_column_array[_index].size() == 1)
        {
            _skel_array[_index] = _pixel_column_array[_index].getCentroidofCluster(0);
        }
        else
        {
            _skel_array[_index] = -1.0;
        }

    }while(_index < Mat_Max_Width);
    //------------------------------------------------------------------------------------

    //Continue to Iterate and pull out continuous centroids of clusters until no more can be rebuilt.
    do
    {
        //Reset the control internals
        _added_point = false;
        _index = 0;
        // Go Thru The Skeletal Array and look for compatible clusters
        do
        {
            _closest_index_left = -1;
            _closest_index_right = -1;
            //Mulitple Clusters. See if one of them meets the continuity requirement.
            if(_pixel_column_array[_index].size() > 1)
            {
                //Look left side if not index 0
                if(_index > 0)
                {
                   _closest_index_left = _pixel_column_array[_index].getIndexOfClosestCentroid(_skel_array[_index-1], _continuity_threshold);
                }
                //Look Right Side if not index Mat_Max_width - 1
                if(_index < Mat_Max_Width - 1)
                {
                    _closest_index_right = _pixel_column_array[_index].getIndexOfClosestCentroid(_skel_array[_index + 1], _continuity_threshold);
                }
                //There is a cluster that needs to be kept
                //One Side or the other has to be chosen to place the value
                if(_closest_index_left != -1)
                {
                    //This data point is part of the left side
                    _skel_array[_index] = _pixel_column_array[_index].getCentroidofCluster(_closest_index_left);
                    _added_point = true;
                }
                else if(_closest_index_right != -1)
                {
                    //This data point is part of the right side
                    _skel_array[_index] = _pixel_column_array[_index].getCentroidofCluster(_closest_index_right);
                    _added_point = true;
                }
            }
            ++_index;
        }while(_index < Mat_Max_Width);
    }while(_added_point);


    //Count the Discontinuities in the Skeletal Array and Draw To Mat
     _index = 0;
     do
     {
         if(_skel_array[_index] == -1.0) //Non Valid Centroid
         {
             ++_total_discontinuities;
         }
         else
         {
             _dataindex = ((int)_skel_array[_index] * _dst.cols) + _index;
             _data[_dataindex] = 255;
         }
           ++_index;
     }while(_index < Mat_Max_Width);
     if(_total_discontinuities > m_allowable_discontinuities)
     {
         emit failedDiscontinuityCheck();
         return false;
     }

     //Old Skeletal Algorythm
     /*

     do
     {
         if(m_cluster_columns[_index].size() == 1) //One Cluster Pop it
         {
             _rowcentroid = m_cluster_columns[_index].getCentroidofCluster(0);
             m_skeletal_line_array[_index] = _rowcentroid;
             if(_rowcentroid > _highestrowvalue)
             {
                 _highestrowvalue = _rowcentroid;
                 m_flattened_hrv = _rowcentroid;
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
 */

    return true;
}

bool Max::doRansacLineProcessing(Mat &_dst, TBILine &_line, int _total_iterations, int _vote_threshold,
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
    int _attempts=0;


    if(_dst.channels() != 3)
    {
        qDebug() << "Max::doRansacLineProcessing() requires a 3 channel Mat.";
        emit failedRansacCheck();
        return false;
    }

    //Find Line Candidates
    do
    {

            _attempts = 0;
            do
            {
                _index1 =  QRandomGenerator::system()->bounded(_start_index, _end_index);
                ++_attempts;
                if(_attempts == 1000)
                {
                    emit failedRansacCheck();
                    return false;
                }
            }while(_skeletalarray[_index1] == 0);
            _attempts = 0;
            do
            {
                _index2 = QRandomGenerator::system()->bounded(_start_index, _end_index);
                ++_attempts;
                if(_attempts == 1000)
                {
                    emit failedRansacCheck();
                    return false;
                }
            }while((_index2 == _index1) || (_skeletalarray[_index2]==0));


        _attempts = 0;
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
        emit failedRansacCheck();
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
        emit failedRansacCheck();
        return false;
    }

    //Set The Line
    _line = _linecandidates[_highestvote_index].m_line;
    _line.remakeLine(_start_index, _end_index);

    //Draw The Line
    _line.drawOnMat(_dst, _line_color, 1);

    return true;
}

bool Max::doSplitMergeProcesssing(float *_data_array, int _max_index, std::vector<TBILine> &_line_vectors, float _min_distance_threshold)
{
    //Clear The Line Vector
    _line_vectors.clear();

    int _index_of_most_distant=0;
    float _most_distant = 0.0;
    float _distance = 0.0;
    int _start_index = -1;
    int _end_index = -1;
    int _current_index = _start_index;
    int _current_vector_index = 0;
    float _data_value;


    //Set the Start and Ending index
    _current_index = 0;
    do
    {
        if(_data_array[_current_index] != -1)
        {
            _start_index = _current_index;
            _current_index = _max_index;
        }
        ++_current_index;
    }while(_current_index < _max_index);
    //Set the Ending Index
    _current_index = _max_index;
    do
    {
        if(_data_array[_current_index] != -1)
        {
            _end_index= _current_index;
            _current_index = _start_index;
        }
        --_current_index;
    }while(_current_index > _start_index);

    //Make Sure The start and ending index is ok
    if((_start_index == -1) || (_end_index == -1))
    {
        return false;
    }

    //Add First Line.
    TBILine _line((float)_start_index, _data_array[_start_index], (float)_end_index, _data_array[_end_index]);
    _line_vectors.push_back(_line);

    //Get Distance From All Points start
    _current_index = _start_index;
    do
    {
        //Get Distance Of All Points From _start_index to _end_index
        _data_value = _data_array[_current_index];
        if(_data_value > 0.0)
        {
           _distance = _line_vectors[_current_vector_index].getOrthogonalDistance((float)_current_index, _data_value);
        }
        else
        {
            _distance = 0;
        }

            if(_distance >= _most_distant)
            {
                _most_distant = _distance;
                _index_of_most_distant = _current_index;
            }
            //Increment The Current Index
            ++_current_index;
            if(_current_index > _end_index) //All The Distant Points Are Checked. Do The Split Checks
            {
                if(_most_distant >= _min_distance_threshold) //The Line Can Be Split!
                {
                    TBILine _line1((float)_start_index, _data_array[_start_index], (float)_index_of_most_distant, _data_array[_index_of_most_distant]);
                    TBILine _line2((float)_index_of_most_distant, _data_array[_index_of_most_distant], (float)_end_index, _data_array[_end_index]);
                    auto _iterator = _line_vectors.begin();
                    _line_vectors[_current_vector_index] = _line1;
                    _line_vectors.insert(_iterator+_current_vector_index+1, _line2);
                    ++_current_vector_index;
                }
                else //The Line Cannot Be Split
                {
                    --_current_vector_index;
                }
                //Adjust The Indexes and reset the distance variables
                //For The Next Line Iteration
                if(_current_vector_index >= 0)
                {
                    _start_index = (int)_line_vectors[_current_vector_index].getPoint1X();
                    _end_index = (int)_line_vectors[_current_vector_index].getPoint2X();
                    _current_index = _start_index;
                    _most_distant = 0;
                    _index_of_most_distant = 0;
                }
            }

    }while(_current_vector_index >= 0);

    if(_line_vectors.size() < 4)
    {
        emit failedSplitMergeCheck();
        return false;
    }
    return true;
}

bool Max::setProjectedRansacLines(TBILine &_src_tsl_left, TBILine &_src_tsl_right, TBILine &_src_bwl_left, TBILine &_src_bwl_right,
                                  TBILine &_dst_tsl_left, TBILine &_dst_tsl_right, TBILine &_dst_bwl_left, TBILine &_dst_bwl_right)
{
    TBIPoint _leftintersectionpnt;
    TBIPoint _rightintersectionpnt;
    TBIPoint _jointintersectionpnt;

    bool _int1 = _src_tsl_left.findPointofIntersection(_src_bwl_left, _leftintersectionpnt);
    bool _int2 = _src_tsl_right.findPointofIntersection(_src_bwl_right, _rightintersectionpnt);
    bool _int3 = _src_bwl_left.findPointofIntersection(_src_bwl_right, _jointintersectionpnt);
    if(_int1 && _int2 && _int3)
    {
        _dst_tsl_left.setPoint1(_src_tsl_left.getPoint1());
        _dst_tsl_left.setPoint2(_leftintersectionpnt);
        _dst_bwl_left.setPoint1(_leftintersectionpnt);
        _dst_bwl_left.setPoint2(_jointintersectionpnt);
        _dst_bwl_right.setPoint1(_jointintersectionpnt);
        _dst_bwl_right.setPoint2(_rightintersectionpnt);
        _dst_tsl_right.setPoint1(_rightintersectionpnt);
        _dst_tsl_right.setPoint2(_src_tsl_right.getPoint2());
    }
    else
    {
        return false;
    }

    return true;
}

void Max::trimRansacTopSurfaceLinesForVGroove(TBILine &_src_tsl_left, TBILine &_src_tsl_right, float *_skel_array, float _cutoff_threshold)
{
    int _index = 0;
    float _distance;
    //Trim Left TSL
    do
    {
        if(_skel_array[_index] != -1)
        {
        TBIPoint _pnt((float)_index, _skel_array[_index]);
        _distance = _src_tsl_left.getOrthogonalDistance(_pnt);
        if(_distance <= _cutoff_threshold)
        {
            _src_tsl_left.setPoint2(_pnt);
            _index = Mat_Max_Width;
        }
        }
        ++_index;
    }while (_index < Mat_Max_Width);

    //Trim Right TSL
    _index = Mat_Max_Width - 1;
    do
    {
        if(_skel_array[_index] != -1)
        {
        TBIPoint _pnt((float)_index, _skel_array[_index]);
        _distance = _src_tsl_right.getOrthogonalDistance(_pnt);
        if(_distance <= _cutoff_threshold)
        {
            _src_tsl_right.setPoint1(_pnt);
            _index = 0;
        }
        }
        --_index;
    }while (_index > 0);

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
        qDebug()<<"Max::recieveNewCVMat() did not recieve a single Channel Mat";
        m_timeinloop = QString("Time in Loop: " + QString::number(m_timer.elapsed()) + " ms.-Camera FPS = " + QString::number(1000/m_timer.elapsed()));
        emit timeInLoopChanged(m_timeinloop);
        emit processingComplete(); //Must Be Last Signal Sent
        return;
    }
    if(!_mat.isContinuous())
    {
        qDebug()<<"Max::recieveNewCVMat() did not recieve a continuous Mat";
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
    cv::Mat _ransac_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC3); //Mats From Here Out are Color.
    cv::Mat _splitmerge_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC3);
    cv::Mat _operation_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC3);

    //Do OpenCV Proccesses
    cv::GaussianBlur(_raw_mat, _blurr_mat, Size(m_blur,m_blur), 0);
    cv::threshold(_blurr_mat, _threshold_mat, m_thresholdmin, m_thresholdmax, THRESH_TOZERO);

    cv::cvtColor(_raw_mat, _operation_mat, cv::COLOR_GRAY2BGR);
    blankProcessingArrays();
    if(doPixelColumnProcessing(_threshold_mat, _pixelcolumn_mat, m_cluster_columns, &m_total_image_intensity, m_max_image_intensity, m_min_image_intensity, m_min_cluster_size, m_max_cluster_size, m_max_clusterincol)) //The TII Check is Done Here
    {


        if(doSkeletonProcessing(_skel_mat, m_cluster_columns, m_skeletal_line_array, 2)) //Fill Skeleton and Discontinuity Trap
        {

            cv::cvtColor(_skel_mat, _ransac_mat, cv::COLOR_GRAY2BGR);
            bool _left_tsl_good = doRansacLineProcessing(_ransac_mat, m_left_tsl, m_tslleft_iterations, m_min_tslleft_votes, m_tslleft_distance_threshold,
                                                         m_min_tslleft_angle, m_max_tslleft_angle, m_skeletal_line_array, 0, m_flattened_iohrv,
                                                         CV_RGB(13,252,224));
            bool _right_tsl_good = doRansacLineProcessing(_ransac_mat, m_right_tsl, m_tslright_iterations, m_min_tslright_votes, m_tslright_distance_threshold,
                                                          m_min_tslright_angle, m_max_tslright_angle, m_skeletal_line_array, m_flattened_iohrv, _skel_mat.cols,
                                                          CV_RGB(13,252,224));


            if(_left_tsl_good && _right_tsl_good)
            {
                {

                    if(setProjectedRansacLines(m_left_tsl, m_right_tsl, m_left_bwl, m_right_bwl,
                                               m_ransac_projection_lefttsl, m_ransac_projection_righttsl,
                                               m_ransac_projection_leftbwl, m_ransac_projection_rightbwl))
                    {

                        m_ransac_projection_leftbwl.drawOnMat(_operation_mat, CV_RGB(0,0,200), 2);
                        m_ransac_projection_rightbwl.drawOnMat(_operation_mat, CV_RGB(0,0,200), 2);
                    }
                }

                trimRansacTopSurfaceLinesForVGroove(m_left_tsl, m_right_tsl, m_skeletal_line_array, 3);
                m_ransac_projection_lefttsl.drawOnMat(_operation_mat, CV_RGB(0,0,200), 2);
                m_ransac_projection_righttsl.drawOnMat(_operation_mat, CV_RGB(0,0,200), 2);

                if(doSplitMergeProcesssing(m_skeletal_line_array, (int)_skel_mat.cols-1, m_topography_lines, m_sm_distance_threshold))
                {
                    cv::cvtColor(_skel_mat, _splitmerge_mat, cv::COLOR_GRAY2BGR);
                    for(int i = 0; i < (int)m_topography_lines.size(); ++i)
                    {
                        if(i%2 == 0)
                        {
                           m_topography_lines[i].drawOnMat(_splitmerge_mat, CV_RGB(0,200,0), 2);
                        }
                        else
                        {
                            m_topography_lines[i].drawOnMat(_splitmerge_mat, CV_RGB(0,0,200), 2);
                        }
                        m_topography_lines[i].drawOnMat(_operation_mat, CV_RGB(200,0,0), 2);

                    }


                }
                if(m_ransac_projection_lefttsl.compareLines(m_topography_lines[0], 10) && m_ransac_projection_righttsl.compareLines(m_topography_lines[(int)m_topography_lines.size()-1], 10))
                {
                    TBIPoint _trackingpoint;
                    m_ransac_projection_lefttsl.findPointofIntersection(m_ransac_projection_leftbwl, _trackingpoint);
                    cv::drawMarker(_operation_mat, cv::Point((int)_trackingpoint.getX(), (int)_trackingpoint.getY()), CV_RGB(0,200,200), MARKER_DIAMOND, 20, 3);
                    m_ransac_projection_righttsl.findPointofIntersection(m_ransac_projection_rightbwl, _trackingpoint);
                    cv::drawMarker(_operation_mat, cv::Point((int)_trackingpoint.getX(), (int)_trackingpoint.getY()), CV_RGB(0,200,200), MARKER_DIAMOND, 20, 3);
                }

            }
            else
            {
                _ransac_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC3);
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

    if(m_emitextramats)
    {
        emit newRawMatProcessed(_raw_mat);
        emit newBlurMatProcessed(_blurr_mat);
        emit newThresholdMatProcessed(_threshold_mat);
        emit newPixelColumnMatProcessed(_pixelcolumn_mat);
        emit newSkeletalMatProcessed(_skel_mat);
        emit newRansacMatProcessed(_ransac_mat);
        emit newSplitMergeMatProcessed(_splitmerge_mat);
    }
    emit newOperationMatProcessed(_operation_mat);

    m_timeinloop = QString("Time in Loop: " + QString::number(m_timer.elapsed()) + " ms.");
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









