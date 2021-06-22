#include "tbiparameterclass_ranscaparms.h"



TBIRansacParameter::TBIRansacParameter(QObject *parent)
{
    m_ideal_angle = 0;
    m_allowed_angle_variance = 0;
    m_min_votes = 20;
    m_distance_threshold = 20;
    m_iterations = 50;
}


void TBIRansacParameter::setIdealAngle(float _idealangle)
{
    if((_idealangle >= -90.0) && (_idealangle <= 90.0))
    {
        m_ideal_angle = _idealangle;
    }
}

void TBIRansacParameter::setAllowedAngleVariance(float _allowedanglevariance)
{
    if((_allowedanglevariance >= 0) && (_allowedanglevariance <=90))
    {
        m_allowed_angle_variance = _allowedanglevariance;
    }
}

void TBIRansacParameter::setMinVotes(int _minvotes)
{
    if(_minvotes >= 1)
    {
        m_min_votes = _minvotes;
    }
}

void TBIRansacParameter::setIterations(int _iterations)
{
    if(_iterations >= 1)
    {
        m_iterations = _iterations;
    }
}

void TBIRansacParameter::setDistanceThreshold(float _distancethreshold)
{
    if(_distancethreshold > 0)
    {
        m_distance_threshold = _distancethreshold;
    }
}

void TBIRansacParameter::makeEqual(TBIRansacParameter &_ransac)
{
    m_ideal_angle = _ransac.getIdealAngle();
    m_allowed_angle_variance = _ransac.getAllowedAngleVariance();
    m_iterations = _ransac.getIteration();
    m_distance_threshold = _ransac.getDistanceThreshold();
    m_min_votes = _ransac.getMinVotes();
}


