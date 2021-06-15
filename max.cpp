#include "max.h"
#include <QDebug>
#include "pixelcolumnclass.h"
#include "pixelclusterclass.h"
#include "pixelfundamental.h"
#include "TBILinearRansacVotingStructure.h"
#include <QRandomGenerator>
#include <QThread>
#include <vector>
#include <random>

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



    m_left_tsl_ransac.setIdealAngle(0);
    m_left_tsl_ransac.setAllowedAngleVariance(10);
    m_left_tsl_ransac.setDistanceThreshold(2.0);
    m_left_tsl_ransac.setIterations(50);
    m_left_tsl_ransac.setMinVotes(20);

    m_right_tsl_ransac.setIdealAngle(0);
    m_right_tsl_ransac.setAllowedAngleVariance(10);
    m_right_tsl_ransac.setDistanceThreshold(2.0);
    m_right_tsl_ransac.setIterations(50);
    m_right_tsl_ransac.setMinVotes(20);

    m_left_bwl_ransac.setIdealAngle(-30);
    m_left_bwl_ransac.setAllowedAngleVariance(10);
    m_left_bwl_ransac.setDistanceThreshold(2.0);
    m_left_bwl_ransac.setIterations(50);
    m_left_bwl_ransac.setMinVotes(20);

    m_right_bwl_ransac.setIdealAngle(30);
    m_right_bwl_ransac.setAllowedAngleVariance(10);
    m_right_bwl_ransac.setDistanceThreshold(2.0);
    m_right_bwl_ransac.setIterations(50);
    m_right_bwl_ransac.setMinVotes(20);



    m_right_ransac_vertex.inValidate();
    m_left_ransac_vertex.inValidate();

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
        m_skeletal_line_array[_col] = -1;
        ++_col;
    }while(_col < Mat_Max_Width);

    m_left_tsl.clear();
    m_right_tsl.clear();
    m_right_bwl.clear();
    m_left_bwl.clear();
    m_right_ransac_vertex.inValidate();
    m_left_ransac_vertex.inValidate();
    m_topography_lines.clear();
    m_flattened_iohrv = 0;
    m_total_image_intensity = 0;
}

bool Max::doImageFlattening(Mat &_src, std::vector<TBIPoint> &_data, quint64 *_tii, const quint64 _max_tii, const quint64 _min_tii)
{
    *_tii = 0;
    _data.clear();

    if(_src.type() != CV_8UC1)
    {
        emit failedFlattenImageData();
        return false;
    }

    int _row = 0;
    int _col = 0;
    int _dataindex = 0;
    uint8_t * _srcdata = _src.data;

    do
    {
        _dataindex = (_row * _src.cols) + _col;
        if(_srcdata[_dataindex] > 0)
        {

            _data.push_back(TBIPoint((float)_col, (float)_row));
            *_tii += _srcdata[_dataindex];
        }

        if(*_tii > _max_tii)
        {
            emit failedTIICheck();
            return false;
        }

        ++_row;
        if(_row >= _src.rows)
        {
            ++_col;
            _row = 0;
        }
    }while(_col < _src.cols);

    if(*_tii < _min_tii)
    {
        emit failedTIICheck();
        return false;
    }

    if(_data.size() < 300)
    {
        emit failedFlattenImageData();
        return false;
    }

    return true;
}

void Max::drawFlattenedImageDataToMat(Mat &_dst, const std::vector<TBIPoint> &_data)
{
    if(_dst.type() != CV_8UC1)
    {
        return;
    }

    if(_data.size() == 0)
    {
        return;
    }

    int _row = 0;
    int _col = 0;
    const int _vectorsize = _data.size();
    int _vectorindex = 0;
    uint8_t * _matdata = _dst.data;
    int _matindex = 0;

    do
    {

        _col = (int)_data[_vectorindex].getX();
        _row = (int)_data[_vectorindex].getY();
        _matindex = (_row * _dst.cols) + _col;
        _matdata[_matindex] = 255;
        ++_vectorindex;

    }while(_vectorindex < _vectorsize);

}

void Max::drawFlattenedImageDataToMat(Mat &_dst, const std::vector<TBIPoint> &_data, const Scalar _color)
{
    if(_dst.type() != CV_8UC3)
    {
        return;
    }

    if(_data.size() == 0)
    {
        return;
    }



    uint8_t * _matdata = _dst.data;
    const int _matchannels = _dst.channels();

    int _row = 0;
    int _col = 0;
    const int _vectorsize = _data.size();
    int _vectorindex = 0;


    int _matindexc1 = 0;
    int _matindexc2 = 0;
    int _matindexc3 = 0;

    //float value = pImgData[img.channels() * (img.cols * row + column) + channel];
    do
    {
        _col = (int)_data[_vectorindex].getX();
        _row = (int)_data[_vectorindex].getY();
        _matindexc1 = _matchannels * (_dst.cols * _row + _col) + 0;
        _matindexc2 = _matchannels * (_dst.cols * _row + _col) + 1;
        _matindexc3 = _matchannels * (_dst.cols * _row + _col) + 2;
        _matdata[_matindexc1] = _color[2];
        _matdata[_matindexc2] = _color[1];
        _matdata[_matindexc3] = _color[0];
        ++_vectorindex;
    }while(_vectorindex < _vectorsize);

}

void Max::copyFlattenedData(const std::vector<TBIPoint> &_src, std::vector<TBIPoint> &_dst)
{

    _dst.clear();
    if(_src.size() == 0) return;

    int _index = 0;
    const int _srcsize = _src.size();
    do
    {
        TBIPoint _pnt = _src[(_srcsize-1) - _index];
        _dst.push_back(_pnt);
        ++_index;
    }while(_index < _srcsize);
}

void Max::removeInliersFromFlattenedData(std::vector<TBIPoint> &_dstdata, const TBILine &_line, const float _distance_threshold_top, const float _distance_threshold_bottom)
{
    if(_dstdata.size() == 0)
    {
        return;
    }

    if(!_line.isValid())
    {
        return;
    }

    int _index = _dstdata.size()-1;
    float _distance;


    do
    {
        _distance = _line.getOrthogonalDistance(_dstdata[_index]);
        if(_line.isPointOnLine(_dstdata[_index]))
        {
            _dstdata.erase(_dstdata.begin() + _index);
        }
        else if(_line.isPointAboveLine(_dstdata[_index]))
        {
            if(_distance <= _distance_threshold_top)
            {
                _dstdata.erase(_dstdata.begin() + _index);
            }
        }
        else if(_line.isPointBelowLine(_dstdata[_index]))
        {
            if(_distance <= _distance_threshold_bottom)
            {
                _dstdata.erase(_dstdata.begin() + _index);
            }
        }
        --_index;
    }while((_index >= 0));
}

void Max::buildFlattenDataFromInliers(const std::vector<TBIPoint> &_srcdata, std::vector<TBIPoint> &_dstdata, const TBILine &_line, const float _distance_threshold_top, const float _distance_threshold_bottom)
{
    _dstdata.clear();
    if(_srcdata.size() == 0) return;
    if(!_line.isValid()) return;

    int _index = 0;
    int _srcvector_size = _srcdata.size();
    float _distance;

    do
    {
        _distance = _line.getOrthogonalDistance(_srcdata[_index]);
        if(_line.isPointOnLine(_srcdata[_index]))
        {
                  _dstdata.push_back(_srcdata[_index]);
        }
        else if(_line.isPointAboveLine(_srcdata[_index]))
        {
            if(_distance <= _distance_threshold_top)
            {
                 _dstdata.push_back(_srcdata[_index]);
            }
        }
        else if(_line.isPointBelowLine(_srcdata[_index]))
        {
            if(_distance <= _distance_threshold_bottom)
            {
                 _dstdata.push_back(_srcdata[_index]);
            }
        }

        ++_index;
    }while(_index < _srcvector_size);
}


bool Max::buildFlattenDataFromBegining(const std::vector<TBIPoint> &_srcdata, std::vector<TBIPoint> &_dstdata, const int _consequetiverows)
{
    _dstdata.clear();

    if(_srcdata[_srcdata.size() - 1].getX() < _consequetiverows)
    {
        return false;
    }

    int _newrowsencoutered = 0;
    int _currentrow = -1;
    int _index = 0;
    int _srcsize = _srcdata.size();

    do
    {
        if(_currentrow != _srcdata[_index].getX())
        {
            _currentrow = _srcdata[_index].getX();
            ++_newrowsencoutered;
        }
        _dstdata.push_back(_srcdata[_index]);
        ++_index;
    }while((_newrowsencoutered < _consequetiverows) && (_index < _srcsize ));

    if(_newrowsencoutered < _consequetiverows)
    {
        return false;
    }

    return true;
}

bool Max::buildFlattenDataFromEnd(const std::vector<TBIPoint> &_srcdata, std::vector<TBIPoint> &_dstdata, const int _consequetiverows)
{
    _dstdata.clear();

    if(_srcdata[_srcdata.size() - 1].getX() < _consequetiverows)
    {
        return false;
    }

    int _newrowsencoutered = 0;
    int _currentrow = -1;
    int _index = _srcdata.size()-1;
    int _srcsize = _srcdata.size();

    do
    {
        if(_currentrow != _srcdata[_index].getX())
        {
            _currentrow = _srcdata[_index].getX();
            ++_newrowsencoutered;
        }
        _dstdata.push_back(_srcdata[_index]);
        --_index;
    }while((_newrowsencoutered < _consequetiverows) && (_index > 0));

    if(_newrowsencoutered < _consequetiverows)
    {
        return false;
    }

    return true;
}

bool Max::doRansacLineProcessing(TBILine &_line, const TBILinearRansac &_ransac, const std::vector<TBIPoint> &_vector)
{
    _line.clear();

    if(_vector.size() < 50)
    {
        emit failedRansacCheck();
        return false;
    }

    std::vector<TBILinearRansacVotingStructure> _linecandidates;

    int _iteration = 0;
    int _pnt1index;
    int _pnt2index;
    int _votingindex = 0;
    float _distance = 0;

    const int _vectorsize = _vector.size();
    const int _requiredvotes = _ransac.getMinVotes();
    const int _totaliterations = _ransac.getIteration();
    const float _distancethreshold = _ransac.getDistanceThreshold();
    float _minanglefromhorizon = _ransac.getIdealAngle() - _ransac.getAllowedAngleVariance();
    float _maxanglefromhorizon = _ransac.getIdealAngle() + _ransac.getAllowedAngleVariance();
    float _idealanglefromhorizon = _ransac.getIdealAngle();

    //Do the Ransac
    do
    {
        //Instantiate the Voting Container
        TBILinearRansacVotingStructure _linecandidate;
        //Start Getting Two Random Points
        _pnt1index = randomInt(0, _vectorsize-1);
        do
        {
            _pnt2index = randomInt(0, _vectorsize-1);
        }while(_pnt1index == _pnt2index);
        //Build The Line From The Two Data Points
        _linecandidate.m_line = TBILine(_vector[_pnt1index], _vector[_pnt2index]);
        //If Horizonal Angle Check is Good Do The Voting.
        _linecandidate.m_angletohorizontal = _linecandidate.m_line.angleFromHorizontal();
        if((_linecandidate.m_angletohorizontal >= _minanglefromhorizon) && (_linecandidate.m_angletohorizontal <= _maxanglefromhorizon))
        {
            //Horizon Angle Check is Good Do The Voting
            _votingindex = 0;
            do
            {
                _distance = _linecandidate.m_line.getOrthogonalDistance(_vector[_votingindex]);
                if(_distance <= _distancethreshold)
                {
                    ++_linecandidate.m_numvotes;
                }
                ++_votingindex;
            }while(_votingindex < _vectorsize);
            //Process The Voting And Add the Candidate If Ok
            if(_linecandidate.m_numvotes >= _requiredvotes)
            {
                _linecandidates.push_back(_linecandidate);
            }
        }
        ++_iteration;
    }while(_iteration < _totaliterations);

    //Evaluate The Candidate List and set the Line if Ok
    if(_linecandidates.size() == 0)
    {
        emit failedRansacCheck();
        return false;
    }

    //Choose the Line Candidate That has the Most Votes
    const int _candidatesize = (int)_linecandidates.size();
    int _candidateindex = 0;
    int _highestvotecount = -1;
    int _highestcandidateindex = -1;
    do
    {

        if(_highestvotecount < _linecandidates[_candidateindex].m_numvotes)
        {
            _highestvotecount = _linecandidates[_candidateindex].m_numvotes;
            _highestcandidateindex = _candidateindex;
        }
        ++_candidateindex;
    }while(_candidateindex < _candidatesize);
    if(_highestcandidateindex != -1)
    {
        _line = _linecandidates[_highestcandidateindex].m_line;
    }
    else
    {
        emit failedRansacCheck();
        return false;
    }


    return true;
}

bool Max::doRansacLineProcessing(TBILine &_line, const TBILinearRansac &_ransac, const std::vector<TBIPoint> &_vector, const int _startvectorindex, const int _endvectorindex)
{
    _line.clear();

    if(_vector.size() < 50)
    {
        emit failedRansacCheck();
        return false;
    }


    std::vector<TBILinearRansacVotingStructure> _linecandidates;

    int _iteration = 0;
    int _pnt1index;
    int _pnt2index;
    int _votingindex = 0;
    float _distance = 0;

    const int _vectorsize = _vector.size();
    const int _requiredvotes = _ransac.getMinVotes();
    const int _totaliterations = _ransac.getIteration();
    const float _distancethreshold = _ransac.getDistanceThreshold();
    float _minanglefromhorizon = _ransac.getIdealAngle() - _ransac.getAllowedAngleVariance();
    float _maxanglefromhorizon = _ransac.getIdealAngle() + _ransac.getAllowedAngleVariance();
    float _idealanglefromhorizon = _ransac.getIdealAngle();

    if((_startvectorindex >= _vectorsize) || (_endvectorindex >= _vectorsize) || (_startvectorindex < 0) || (_endvectorindex <= _startvectorindex))
    {
        emit failedRansacCheck();
        return false;

    }

    //Do the Ransac
    do
    {
        //Instantiate the Voting Container
        TBILinearRansacVotingStructure _linecandidate;
        //Start Getting Two Random Points
        _pnt1index = randomInt(_startvectorindex, _endvectorindex);
        do
        {
            _pnt2index = randomInt(_startvectorindex, _endvectorindex);
        }while(_pnt1index == _pnt2index);
        //Build The Line From The Two Data Points
        _linecandidate.m_line = TBILine(_vector[_pnt1index], _vector[_pnt2index]);
        //If Horizonal Angle Check is Good Do The Voting.
        _linecandidate.m_angletohorizontal = _linecandidate.m_line.angleFromHorizontal();
        if((_linecandidate.m_angletohorizontal >= _minanglefromhorizon) && (_linecandidate.m_angletohorizontal <= _maxanglefromhorizon))
        {
            //Horizon Angle Check is Good Do The Voting
            _votingindex = 0;
            do
            {
                _distance = _linecandidate.m_line.getOrthogonalDistance(_vector[_votingindex]);
                if(_distance <= _distancethreshold)
                {
                    ++_linecandidate.m_numvotes;
                }
                ++_votingindex;
            }while(_votingindex < _vectorsize);
            //Process The Voting And Add the Candidate If Ok
            if(_linecandidate.m_numvotes >= _requiredvotes)
            {
                _linecandidates.push_back(_linecandidate);
            }
        }
        ++_iteration;
    }while(_iteration < _totaliterations);

    //Evaluate The Candidate List and set the Line if Ok
    if(_linecandidates.size() == 0)
    {
        emit failedRansacCheck();
        return false;
    }

    //Choose the Line Candidate That has the Most Votes
    const int _candidatesize = (int)_linecandidates.size();
    int _candidateindex = 0;
    int _highestvotecount = -1;
    int _highestcandidateindex = -1;
    do
    {

        if(_highestvotecount < _linecandidates[_candidateindex].m_numvotes)
        {
            _highestvotecount = _linecandidates[_candidateindex].m_numvotes;
            _highestcandidateindex = _candidateindex;
        }
        ++_candidateindex;
    }while(_candidateindex < _candidatesize);
    if(_highestcandidateindex != -1)
    {
        _line = _linecandidates[_highestcandidateindex].m_line;
    }
    else
    {
        emit failedRansacCheck();
        return false;
    }

}




bool Max::doPixelColumnProcessing(Mat &_src, Mat &_dst, PixelColumnClass *_pixel_column_array, quint64 *_tii, quint64 _max_tii,
                                  quint64 _min_tii, int _min_cluster_size, int _max_cluster_size, int _max_clusters_in_column)
{
    *_tii = 0;
    int _column_index = 0;
    PixelColumnProcessingReturn_t _val;



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



    //start the right side continuity check.

    emit totalImageIntensityChanged(m_total_image_intensity);
    return true;

}
bool Max::doSkeletonProcessing(Mat &_dst, PixelColumnClass *_pixel_column_array, float *_skel_array, float _continuity_threshold)
{
    uint8_t* _data = (uint8_t*)_dst.data;
    int _empty_cols = 0;
    int _index = 0;
    int _dataindex;
    float _rowcentroid;
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
                m_flattened_hrv = _rowcentroid;
                m_flattened_iohrv = _index;
            }
            _dataindex = ((int)_rowcentroid * _dst.cols) + _index;
            _data[_dataindex] = 255;
        }
        else
        {
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

void Max::deleteSkelPointsAbovePZL(TBILine &_lpzl, TBILine &_rpzl, float *_skel_array)
{

    if(!_lpzl.isValid()) return;
    if(!_rpzl.isValid()) return;

    //GO Thru Every Skel data element and remove anything that is about the pzl left or right
    int _seperatedindex = (int)_lpzl.getPoint2X();

    int _currentindex = 0;
    float _linevalue;


    do
    {
        if(_skel_array[_currentindex] != -1)
        {
            //Get the Correct Line Value. Either Left or Right.
            if(_currentindex < _seperatedindex) //On the Left PZL
            {
                _linevalue = _lpzl.getYatX((float)_currentindex);
            }
            else if(_currentindex > _seperatedindex) //on the Right PZL
            {
                _linevalue = _rpzl.getYatX((float)_currentindex);
            }
            else//Must Determine which line to use.
            {
                if(_lpzl.getYatX((float)_currentindex) < _rpzl.getYatX((float)_currentindex)) //left is higher. Use left
                {
                    _linevalue = _lpzl.getYatX((float)_currentindex);
                }
                else
                {
                    _linevalue = _rpzl.getYatX((float)_currentindex);
                }
            }
            if(_linevalue > _skel_array[_currentindex]) //The Point is above the PZL
            {
                _skel_array[_currentindex] = -1;
            }
        }
        ++_currentindex;
    }while(_currentindex < Mat_Max_Width);

}



bool Max::doRansacLineProcessing(Mat &_dst, TBILine &_line, TBILinearRansac &_ransac,
                                 float *_skeletalarray, int _start_index, int _end_index, Scalar _line_color)
{
    _line.clear();

    //std::vector<TBILineVotingStructure> _linecandidates;
    QList<TBILinearRansacVotingStructure> _linecandidates;
    int _index1;
    int _index2;
    float _distance;
    int _highestvote_index = 0;
    int _highest_vote_count = 0;
    int _iteration = 0;
    int _attempts=0;


    const int _total_iterations = _ransac.getIteration();
    const int _vote_threshold = _ransac.getMinVotes();
    const float _distance_threshold = _ransac.getDistanceThreshold();
    const float _min_angle_to_horizon = _ransac.getIdealAngle() - _ransac.getAllowedAngleVariance();
    const float _max_angle_to_horizon = _ransac.getIdealAngle() + _ransac.getAllowedAngleVariance();




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
        TBILinearRansacVotingStructure _linecandidate;
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

bool Max::doRansacVertexProcessing(TBILine &_ltsl, TBILine &_rtsl, float *_skel_array, TBIPoint &_lv, TBIPoint &_rv, const float _left_vertex_dist_threshold, const float _right_vertex_dist_threshold)
{
    /*
     Finding the Left and Right Side Vertexes is critical.
     Each vertex uses a left side right side comparison.
     */
    _lv.inValidate();
    _rv.inValidate();

    if(_left_vertex_dist_threshold <= 0)
    {
        emit failedRansacVertexProcesssing();
        return false;
    }
    if(_right_vertex_dist_threshold <= 0)
    {
        emit failedRansacVertexProcesssing();
        return false;
    }
    if(!_ltsl.isValid() || !_rtsl.isValid())
    {
        emit failedRansacVertexProcesssing();
        return false;
    }


    const int _seperationleftintdex = (int)_ltsl.getPoint2X();
    const int _seperationrightindex = (int)_rtsl.getPoint1X();
    int _lastvalidindex = -1;
    int _currentindex;
    float _distance;


    //Find The Last Voting Ransac Member of Left TSL
    _currentindex = (int)_ltsl.getPoint1X();
    do
    {
        if(_skel_array[_currentindex] != -1)
        {
            _distance = _ltsl.getOrthogonalDistance(TBIPoint((float)_currentindex, _skel_array[_currentindex]));
            if(_distance <= _left_vertex_dist_threshold + 2)
            {
                _lastvalidindex = _currentindex;
            }
        }
        ++_currentindex;
    }while(_currentindex < _seperationleftintdex);
    if(_lastvalidindex == -1)
    {
        emit failedRansacVertexProcesssing();
        return false;
    }
    _lv.setX((float)_lastvalidindex);
    _lv.setY(_skel_array[_lastvalidindex]);
    _lv.validate();


    //Find The Last Voting Ransac Member of Right TSL
    _currentindex = (int)_rtsl.getPoint2X();
    _lastvalidindex = -1;
    do
    {
        if(_skel_array[_currentindex] != -1)
        {
            _distance = _rtsl.getOrthogonalDistance(TBIPoint((float)_currentindex, _skel_array[_currentindex]));
            if(_distance <= _right_vertex_dist_threshold + 2)
            {
                _lastvalidindex = _currentindex;
            }
        }

        --_currentindex;
    }while(_currentindex > _seperationrightindex);
    if(_lastvalidindex == -1)
    {
        _lv.inValidate();
        emit failedRansacVertexProcesssing();
        return false;
    }
    _rv.setX((float)_lastvalidindex);
    _rv.setY(_skel_array[_lastvalidindex]);
    _rv.validate();
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
    int _current_index;
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

int Max::randomInt(int _min, int _max)
{
    //C++ 11 Random Number Engine. #include <random>
    std::random_device seeder; //the random device that will seed the generator
    std::mt19937 engine(seeder()); //then make a mersenne twister engine
    std::uniform_int_distribution<int> dist(_min, _max); //then the easy part... the distribution
    return dist(engine); //Return the Number From The Distrobution
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

    //Image Flatten The Scan Data Vector
    if(doImageFlattening(_threshold_mat, m_flattened_scan_data, &m_total_image_intensity, m_max_image_intensity, m_min_image_intensity))
    {

        cv::cvtColor(_raw_mat, _ransac_mat, cv::COLOR_GRAY2BGR);
        //Build The Left TSL Flattened Vector From The Scan Vector
        if(buildFlattenDataFromBegining(m_flattened_scan_data, m_flattened_left_tsl_data, 50))
        {
            //Draw The Flattened Left TSL Data To Ransac
            drawFlattenedImageDataToMat(_ransac_mat, m_flattened_left_tsl_data,  CV_RGB(250,10,10));
            //The TSL Flattend Vector Built Good. Do The Ransac and Draw it
            if(doRansacLineProcessing(m_left_tsl, m_left_tsl_ransac,m_flattened_left_tsl_data))
            {
                //The Left TSL Ransac Was Found Draw It
                m_left_tsl.remakeLine(0, _raw_mat.cols);
                m_left_tsl.drawOnMat(_ransac_mat, CV_RGB(125,125,250), 1);

                //Start Working on the Right TSL
                if(buildFlattenDataFromEnd(m_flattened_scan_data, m_flattened_right_tsl_data, 50))
                {
                    //The Right Side TSL was Found Draw it
                    drawFlattenedImageDataToMat(_ransac_mat, m_flattened_right_tsl_data, CV_RGB(10,250,10));
                    //Now Do the Right TSL Ransac
                    if(doRansacLineProcessing(m_right_tsl, m_right_tsl_ransac, m_flattened_right_tsl_data))
                    {
                        //The Right TSL Was Found. Draw it
                        m_right_tsl.remakeLine(0, _raw_mat.cols);
                        m_right_tsl.drawOnMat(_ransac_mat, CV_RGB(125, 250, 150), 1);

                        //Start Working on Bevel Walls
                        copyFlattenedData(m_flattened_scan_data, m_flattened_bevel_wall_data);
                        removeInliersFromFlattenedData(m_flattened_bevel_wall_data, m_left_tsl, _raw_mat.rows, 5);
                        removeInliersFromFlattenedData(m_flattened_bevel_wall_data, m_right_tsl, _raw_mat.rows, 5);
                        drawFlattenedImageDataToMat(_ransac_mat, m_flattened_bevel_wall_data, CV_RGB(10,10,250));
                        //Do The Left BWL
                        //Adjust Left BWL Ransac For Left TSL and Right BWL For Right TSL
                        TBILinearRansac _adjusted_left_bwl_ransac;
                        _adjusted_left_bwl_ransac.makeEqual(m_left_bwl_ransac);
                        _adjusted_left_bwl_ransac.setIdealAngle(m_left_tsl.angleFromHorizontal() - m_left_bwl_ransac.getIdealAngle());
                        TBILinearRansac _adjusted_right_bwl_ransac;
                        _adjusted_right_bwl_ransac.makeEqual(m_right_bwl_ransac);
                        _adjusted_right_bwl_ransac.setIdealAngle(m_right_tsl.angleFromHorizontal() - m_right_bwl_ransac.getIdealAngle());

                        //Process the Left BWL Thru Flattened Data Refining
                        if(doRansacLineProcessing(m_left_bwl, _adjusted_left_bwl_ransac, m_flattened_bevel_wall_data))
                        {
                            //Refine The Ransac For The Bevel Wall.
                            buildFlattenDataFromInliers(m_flattened_bevel_wall_data, m_flattened_left_bwl_data, m_left_bwl, 10, 10);
                            drawFlattenedImageDataToMat(_ransac_mat, m_flattened_left_bwl_data,  CV_RGB(250,10,10));
                            //Do Ransac Again On Refined Flattened Data For Left BWL
                            if(doRansacLineProcessing(m_left_bwl, _adjusted_left_bwl_ransac, m_flattened_left_bwl_data))
                            {
                                m_left_bwl.remakeLine(0, _raw_mat.cols);
                                m_left_bwl.drawOnMat(_ransac_mat, CV_RGB(125,125,125), 1);

                                //Process the Right BWL Thru Flattened Data Refining
                                if(doRansacLineProcessing(m_right_bwl, _adjusted_right_bwl_ransac, m_flattened_bevel_wall_data))
                                {
                                    //Refine The Ransac For The Bevel Wall.
                                    buildFlattenDataFromInliers(m_flattened_bevel_wall_data, m_flattened_right_bwl_data, m_right_bwl, 10, 10);
                                    drawFlattenedImageDataToMat(_ransac_mat, m_flattened_right_bwl_data,  CV_RGB(10,250,10));
                                    //Do Ransac Again On Refined Flattened Data For Left BWL
                                    if(doRansacLineProcessing(m_right_bwl, _adjusted_right_bwl_ransac, m_flattened_right_bwl_data))
                                    {
                                        m_right_bwl.remakeLine(0, _raw_mat.cols);
                                        m_right_bwl.drawOnMat(_ransac_mat, CV_RGB(125,125,125), 1);

                                        //Trim Ransac Lines and OverView Tracking Points
                                        TBIPoint _intersectionpnt1;
                                        TBIPoint _intersectionpnt2;
                                        TBIPoint _intersectionpnt3;

                                        if(m_left_tsl.findPointofIntersection(m_left_bwl, _intersectionpnt1))
                                        {
                                            if(m_left_bwl.findPointofIntersection(m_right_bwl, _intersectionpnt2))
                                            {
                                                if(m_right_bwl.findPointofIntersection(m_right_tsl, _intersectionpnt3))
                                                {
                                                    //Trim Lines
                                                    m_left_tsl.remakeLine(0 , (int)_intersectionpnt1.getX());
                                                    m_left_bwl.remakeLine(_intersectionpnt1.getX(), (int)_intersectionpnt2.getX());
                                                    m_right_bwl.remakeLine((int)_intersectionpnt2.getX(), (int)_intersectionpnt3.getX());
                                                    m_right_tsl.remakeLine((int)_intersectionpnt3.getX(), _raw_mat.cols);
                                                    m_left_tsl.drawOnMat(_operation_mat, CV_RGB(100,255,100), 1);
                                                    m_left_bwl.drawOnMat(_operation_mat, CV_RGB(100,255,100), 1);
                                                    m_right_bwl.drawOnMat(_operation_mat, CV_RGB(100,255,100), 1);
                                                    m_right_tsl.drawOnMat(_operation_mat, CV_RGB(100,255,100), 1);
                                                    cv::drawMarker(_operation_mat, cv::Point((int)_intersectionpnt1.getX(), (int)_intersectionpnt1.getY()), CV_RGB(0,200,200), MARKER_DIAMOND, 20, 3);
                                                    cv::drawMarker(_operation_mat, cv::Point((int)_intersectionpnt3.getX(), (int)_intersectionpnt3.getY()), CV_RGB(0,200,200), MARKER_DIAMOND, 20, 3);

                                                }
                                            }
                                        }

                                    }

                                }
                            }

                        }


                    }
                }

            }
        }

    }





    /*
    if(doPixelColumnProcessing(_threshold_mat, _pixelcolumn_mat, m_cluster_columns, &m_total_image_intensity, m_max_image_intensity, m_min_image_intensity, m_min_cluster_size, m_max_cluster_size, m_max_clusterincol)) //The TII Check is Done Here
    {


        if(doSkeletonProcessing(_skel_mat, m_cluster_columns, m_skeletal_line_array, 2)) //Fill Skeleton and Discontinuity Trap
        {

            cv::cvtColor(_skel_mat, _ransac_mat, cv::COLOR_GRAY2BGR);
            //bool _left_tsl_good = doRansacLineProcessing(_ransac_mat, m_left_tsl, m_left_tsl_ransac, m_skeletal_line_array, 0, m_flattened_iohrv, CV_RGB(13,252,224));
            //bool _right_tsl_good = doRansacLineProcessing(_ransac_mat, m_right_tsl, m_right_tsl_ransac, m_skeletal_line_array, m_flattened_iohrv, _skel_mat.cols, CV_RGB(13,252,224));
            //bool _left_bwl_good = doRansacLineProcessing(_ransac_mat, m_left_bwl, m_left_bwl_ransac, m_skeletal_line_array, 0, m_flattened_iohrv, CV_RGB(13,252,224));
            //bool _right_bwl_good = doRansacLineProcessing(_ransac_mat, m_right_bwl, m_right_bwl_ransac, m_skeletal_line_array, m_flattened_iohrv, _skel_mat.cols, CV_RGB(13,252,224));

            bool _left_tsl_good = doRansacLineProcessing(_ransac_mat, m_left_tsl, m_left_tsl_ransac, m_skeletal_line_array, 0, m_flattened_iohrv, CV_RGB(13,252,224));
            bool _right_tsl_good = doRansacLineProcessing(_ransac_mat, m_right_tsl, m_right_tsl_ransac, m_skeletal_line_array, m_flattened_iohrv, _skel_mat.cols, CV_RGB(13,252,224));
            bool _left_bwl_good = doRansacLineProcessing(_ransac_mat, m_left_bwl, m_left_bwl_ransac, m_skeletal_line_array, 0, _skel_mat.cols-1, CV_RGB(13,252,224));
            bool _right_bwl_good = doRansacLineProcessing(_ransac_mat, m_right_bwl, m_right_bwl_ransac, m_skeletal_line_array, 0, _skel_mat.cols-1, CV_RGB(13,252,224));

            if(_left_tsl_good && _right_tsl_good && _left_bwl_good && _right_bwl_good)
            {



                deleteSkelPointsAbovePZL(m_left_tsl, m_right_tsl, m_skeletal_line_array);



                   // cv::drawMarker(_ransac_mat, cv::Point((int)m_left_ransac_vertex.getX(), (int)m_left_ransac_vertex.getY()), CV_RGB(0,200,200), MARKER_DIAMOND, 20, 3);
                    //cv::drawMarker(_ransac_mat, cv::Point((int)m_right_ransac_vertex.getX(), (int)m_right_ransac_vertex.getY()), CV_RGB(0,200,200), MARKER_DIAMOND, 20, 3);
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



            }
            else
            {
                //_ransac_mat = cv::Mat::zeros(_raw_mat.rows, _raw_mat.cols, CV_8UC3);
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
*/
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

    emit timeInLoopChanged(m_timeinloop);
    m_in_proccesing_loop = false;

    qDebug() << "Time in loop " << m_timer.elapsed();
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









