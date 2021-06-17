#include "tbiransac.h"
#include "random"

TBIRansac::TBIRansac()
{

}

bool TBIRansac::doRansac(TBILine &_line, const TBILinearRansac &_ransac, const TBIDataSet &_dataset)
{
    _line.clear();
    const int _datasetsize = _dataset.size();

    if(_datasetsize < _ransac.getMinVotes()) return false;
    if(_datasetsize < 10) return false;

    const int _iterations = _ransac.getIteration();
    const float _distancethreshold = _ransac.getDistanceThreshold();
    const float _min_angle_from_horizon = _ransac.getIdealAngle() - _ransac.getAllowedAngleVariance();
    const float _max_angle_from_horizon = _ransac.getIdealAngle() + _ransac.getAllowedAngleVariance();
    const int _min_votes = _ransac.getMinVotes();

    int _iteration;
    int _pntindex;
    int _votetotal;
    int _indexp1;
    int _indexp2;
    float _distance;
    float _aoh;

    TBILinearRansacVotingStructure _ransacresult;

    _iteration = 0;
    do
    {
        //Form Line From 2 Random Points in Data Set
        _indexp1 = randomInt(0,_datasetsize-1);
        do{_indexp2 = randomInt(0, _datasetsize-1);}while(_indexp1 == _indexp2);
        TBILine _tstline(_dataset[_indexp1], _dataset[_indexp2]);
        //Make Sure Test Line is Valid
        if(_tstline.isValid())
        {
            //Only Do The Voting if the Angle From Horizontal is Correct
            _aoh = _tstline.angleFromHorizontal();
            if((_aoh >= _min_angle_from_horizon) && (_aoh <= _max_angle_from_horizon))
            {
                //Reset the Point Index and the Vote Total
                _pntindex = 0;
                _votetotal = 0;
                //Do The Voting
                do
                {
                    _distance = _tstline.distanceAbs(_dataset[_pntindex]);
                    if((_distance <= _distancethreshold))
                    {
                        ++_votetotal;
                    }
                    ++_pntindex;
                }while(_pntindex < _datasetsize);
                //Make Sure The Test Line Satisfies The Min Vote Requirement.
                if(_votetotal >= _min_votes)
                {
                    //Set This Result As The Best Candidate if it has more votes then the last good result.
                    if(_votetotal > _ransacresult.m_numvotes)
                    {
                        _ransacresult.m_numvotes = _votetotal;
                        _ransacresult.m_line = _tstline;
                        _ransacresult.m_angletohorizontal = _aoh;
                    }
                }
            }
        }
        //Incriment The Next Iteration
        ++_iteration;
    }while(_iteration > _iterations);

    //Determine if a Valid Result Was Found.
    if(_ransacresult.m_line.isValid())
    {
        _line = _ransacresult.m_line;
    }
    else
    {
        return false;
    }
    return true;
}

//The Dummy Sets Are Used to Avoid Having to Allocate The Memory.
bool TBIRansac::doRansacWithInliers(TBILine &_line, TBIDataSet &_inlierset, TBIDataSet &_dummyset1,
                                    TBIDataSet &_dummyset2, const TBILinearRansac &_ransac,
                                    const TBIDataSet &_dataset)
{
    _line.clear();
    _dummyset1.clear();
    _dummyset2.clear();
    const int _datasetsize = _dataset.size();

    if(_datasetsize < _ransac.getMinVotes()) return false;
    if(_datasetsize < 10) return false;

    const int _iterations = _ransac.getIteration();
    const float _distancethreshold = _ransac.getDistanceThreshold();
    const float _min_angle_from_horizon = _ransac.getIdealAngle() - _ransac.getAllowedAngleVariance();
    const float _max_angle_from_horizon = _ransac.getIdealAngle() + _ransac.getAllowedAngleVariance();
    const int _min_votes = _ransac.getMinVotes();

    int _iteration;
    int _pntindex;
    int _votetotal;
    int _indexp1;
    int _indexp2;
    float _distance;
    float _aoh;

    TBILinearRansacVotingStructure _ransacresult;


    _iteration = 0;
    do
    {
        //Form Line From 2 Random Points in Data Set
        _indexp1 = randomInt(0,_datasetsize-1);
        do{_indexp2 = randomInt(0, _datasetsize-1);}while(_indexp1 == _indexp2);
        TBILine _tstline(_dataset[_indexp1], _dataset[_indexp2]);
        //Make Sure Test Line is Valid
        if(_tstline.isValid())
        {
            //Only Do The Voting if the Angle From Horizontal is Correct
            _aoh = _tstline.angleFromHorizontal();
            if((_aoh >= _min_angle_from_horizon) && (_aoh <= _max_angle_from_horizon))
            {
                //Reset the Point Index and the Vote Total
                _pntindex = 0;
                _votetotal = 0;
                _dummyset1.clear();
                //Do The Voting
                do
                {
                    _distance = _tstline.distanceAbs(_dataset[_pntindex]);
                    if((_distance <= _distancethreshold))
                    {
                        ++_votetotal;
                        TBIPoint_Int __pnt(_dataset[_pntindex].m_x, _dataset[_pntindex].m_y);
                        _dummyset1.insert(__pnt);
                    }
                    ++_pntindex;
                }while(_pntindex < _datasetsize);
                //Make Sure The Test Line Satisfies The Min Vote Requirement.
                if(_votetotal >= _min_votes)
                {
                    //Set This Result As The Best Candidate if it has more votes then the last good result.
                    if(_votetotal > _ransacresult.m_numvotes)
                    {
                        _ransacresult.m_numvotes = _votetotal;
                        _ransacresult.m_line = _tstline;
                        _ransacresult.m_angletohorizontal = _aoh;
                        _dummyset2 = _dummyset1;
                    }
                }
            }
        }
        //Incriment The Next Iteration
        ++_iteration;
    }while(_iteration > _iterations);

    //Determine if a Valid Result Was Found.
    if(_ransacresult.m_line.isValid())
    {
        _line = _ransacresult.m_line;
        _inlierset = _dummyset2;
    }
    else
    {
        return false;
    }
    return true;
}

int TBIRansac::randomInt(int _min, int _max)
{
    //C++ 11 Random Number Engine. #include <random>
    std::random_device seeder; //the random device that will seed the generator
    std::mt19937 engine(seeder()); //then make a mersenne twister engine
    std::uniform_int_distribution<int> dist(_min, _max); //then the easy part... the distribution
    return dist(engine); //Return the Number From The Distrobution
}


