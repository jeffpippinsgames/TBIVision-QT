#include "tbiclass_ransac.h"
#include "random"

TBIRansac::TBIRansac()
{

}

bool TBIRansac::doRansac(TBILine &_line, const TBIRansacParameter &_ransac, const TBIDataSet &_dataset)
{
    _line.clear();
    const int _datasetsize = _dataset.size();

    if(_datasetsize < _ransac.getMinVotes()) return false;
    if(_datasetsize < 10) return false;

    const int _iterations = _ransac.getIterations();
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

    int _actual_validlinestested=0;
    int _highestvotecount = 0;
    TBILine _resultingline(0,0,0,0);

    //TBILinearRansacVotingStructure _ransacresult;

    _iteration = 0;
    do
    {
        //Form Line From 2 Random Points in Data Set
        _indexp1 = randomInt(0,_datasetsize-1);
        do
        {
            _indexp2 = randomInt(0, _datasetsize-1);
        }while(_indexp1 == _indexp2);
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
                ++_actual_validlinestested;
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
                    if(_votetotal > _highestvotecount)
                    {
                        _highestvotecount = _votetotal;
                        _resultingline = _tstline;
                    }
                }
            }
            else
            {
                //qDebug() << "_aoh = " << _aoh << ":   Required aoh  " << _min_angle_from_horizon << "-" << _max_angle_from_horizon;
            }

        }
        //Incriment The Next Iteration

        ++_iteration;
    }while(_iteration < _iterations);

    //Determine if a Valid Result Was Found.
    if(_resultingline.isValid())
    {
        _line = _resultingline;
    }
    else
    {
        return false;
    }
    return true;
}

bool TBIRansac::doRansac(TBILine &_line, const TBIRansacParameter &_ransac, const TBIDataSet &_dataset, const int _startdsindex, const int _enddsindex)
{
    _line.clear();
    const int _datasetsize = _dataset.size();
    if(_startdsindex >= _enddsindex) return false;
    if(_enddsindex >= _dataset.size()) return false;

    if(_datasetsize < _ransac.getMinVotes()) return false;
    if(_datasetsize < 10) return false;

    const int _iterations = _ransac.getIterations();
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

    int _actual_validlinestested=0;
    int _highestvotecount = 0;
    TBILine _resultingline(0,0,0,0);

    //TBILinearRansacVotingStructure _ransacresult;

    _iteration = 0;
    do
    {
        //Form Line From 2 Random Points in Data Set
        _indexp1 = randomInt(_startdsindex,_enddsindex);
        do
        {
            _indexp2 = randomInt(_startdsindex, _enddsindex);
        }while(_indexp1 == _indexp2);
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
                ++_actual_validlinestested;
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
                    if(_votetotal > _highestvotecount)
                    {
                        _highestvotecount = _votetotal;
                        _resultingline = _tstline;
                    }
                }
            }
            else
            {
                //qDebug() << "_aoh = " << _aoh << ":   Required aoh  " << _min_angle_from_horizon << "-" << _max_angle_from_horizon;
            }

        }
        //Incriment The Next Iteration

        ++_iteration;
    }while(_iteration < _iterations);

    //Determine if a Valid Result Was Found.
    if(_resultingline.isValid())
    {
        _line = _resultingline;
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


