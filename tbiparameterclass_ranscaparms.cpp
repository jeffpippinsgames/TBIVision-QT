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
        emit this->idealAngleChanged();
    }
}

void TBIRansacParameter::setAllowedAngleVariance(float _allowedanglevariance)
{
    if((_allowedanglevariance >= 0) && (_allowedanglevariance <=90))
    {
        m_allowed_angle_variance = _allowedanglevariance;
        emit this->allowedAngleVarianceChanged();
    }
}

void TBIRansacParameter::setMinVotes(int _minvotes)
{
    if(_minvotes >= 1)
    {
        m_min_votes = _minvotes;
        emit this->minVotesChanged();
    }
}

void TBIRansacParameter::setMaxVotes(int _maxvotes)
{
    if(_maxvotes > m_min_votes)
    {
        m_max_votes = _maxvotes;
        emit this->maxVotesChanged();
    }
}

void TBIRansacParameter::setIterations(int _iterations)
{
    if(_iterations >= 1)
    {
        m_iterations = _iterations;
        emit this->iterationsChanged();
    }
}

void TBIRansacParameter::setDistanceThreshold(float _distancethreshold)
{
    if(_distancethreshold > 0)
    {
        m_distance_threshold = _distancethreshold;
        emit this->distanceThresholdChanged();
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
    this->setMaxVotes(100);
    this->setDistanceThreshold(3);
    this->setIterations(50);
}

void TBIRansacParameter::saveToFile(QDataStream &_filedatastream)
{
    _filedatastream << this->m_ideal_angle;
    _filedatastream << this->m_allowed_angle_variance;
    _filedatastream << this->m_min_votes;
    _filedatastream << this->m_max_votes;
    _filedatastream << this->m_iterations;
    _filedatastream << this->m_distance_threshold;
}

void TBIRansacParameter::loadFromFile(QDataStream &_filedatastream)
{
    _filedatastream >> this->m_ideal_angle;
    _filedatastream >> this->m_allowed_angle_variance;
    _filedatastream >> this->m_min_votes;
    _filedatastream >> this->m_max_votes;
    _filedatastream >> this->m_iterations;
    _filedatastream >> this->m_distance_threshold;
    emit this->idealAngleChanged();
    emit this->allowedAngleVarianceChanged();
    emit this->minVotesChanged();
    emit this->maxVotesChanged();
    emit this->iterationsChanged();
    emit this->distanceThresholdChanged();
}


