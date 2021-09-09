#include "tbiparameterclass_ranscaparms.h"



TBIRansacParameter::TBIRansacParameter(QObject *parent)
{
    this->setDefautValues();
}


void TBIRansacParameter::setIdealAngle(float _idealangle)
{
    if((_idealangle >= -90.0) && (_idealangle <= 90.0))
    {
        m_ideal_angle = _idealangle;
        emit idealAngleChanged();
    }
}

void TBIRansacParameter::setAllowedAngleVariance(float _allowedanglevariance)
{
    if((_allowedanglevariance >= 0) && (_allowedanglevariance <=90))
    {
        m_allowed_angle_variance = _allowedanglevariance;
        emit allowedAngleVarianceChanged();
    }
}

void TBIRansacParameter::setMinVotes(int _minvotes)
{
    if(_minvotes >= 1)
    {
        m_min_votes = _minvotes;
        emit minVotesChanged();
    }
}

void TBIRansacParameter::setMaxVotes(int _maxvotes)
{
    if(_maxvotes > m_min_votes)
    {
        m_max_votes = _maxvotes;
        emit maxVotesChanged();
    }
}

void TBIRansacParameter::setIterations(int _iterations)
{
    if(_iterations >= 1)
    {
        m_iterations = _iterations;
        emit iterationsChanged();
    }
}

void TBIRansacParameter::setDistanceThreshold(float _distancethreshold)
{
    if(_distancethreshold > 0)
    {
        m_distance_threshold = _distancethreshold;
        emit distanceThresholdChanged();
    }
}

void TBIRansacParameter::makeEqual(TBIRansacParameter &_srcransacparam)
{
    this->setIdealAngle(_srcransacparam.getIdealAngle());
    this->setAllowedAngleVariance(_srcransacparam.getAllowedAngleVariance());
    this->setMinVotes(_srcransacparam.getMinVotes());
    this->setMaxVotes(_srcransacparam.getMaxVotes());
    this->setDistanceThreshold(_srcransacparam.getDistanceThreshold());
    this->setIterations(_srcransacparam.getIterations());
}

void TBIRansacParameter::setDefautValues()
{
    this->setIdealAngle(0.0);
    this->setAllowedAngleVariance(0.0);
    this->setMinVotes(10);
    this->setMaxVotes(9999999);
    this->setDistanceThreshold(3);
    this->setIterations(50);
}

void TBIRansacParameter::saveToFile(QDataStream &_filedatastream)
{
    _filedatastream << m_ideal_angle;
    _filedatastream << m_allowed_angle_variance;
    _filedatastream << m_min_votes;
    _filedatastream << m_max_votes;
    _filedatastream << m_iterations;
    _filedatastream << m_distance_threshold;
}

void TBIRansacParameter::loadFromFile(QDataStream &_filedatastream)
{
    _filedatastream >> m_ideal_angle;
    _filedatastream >> m_allowed_angle_variance;
    _filedatastream >> m_min_votes;
    _filedatastream >> m_max_votes;
    _filedatastream >> m_iterations;
    _filedatastream >> m_distance_threshold;
    emit idealAngleChanged();
    emit allowedAngleVarianceChanged();
    emit minVotesChanged();
    emit maxVotesChanged();
    emit iterationsChanged();
    emit distanceThresholdChanged();
}


