#include "tbilinearransac.h"



TBILinearRansac::TBILinearRansac(QObject *parent)
{
    m_ideal_angle = 0;
    m_allowed_angle_variance = 0;
    m_min_votes = 20;
    m_distance_threshold = 20;
    m_iterations = 50;
}

bool TBILinearRansac::doRansacProcessingLinearLines(TBILine &_dstline, float *_skel_data, int _startindex, int _endindex)
{

    return true;
}

void TBILinearRansac::setIdealAngle(float _idealangle)
{
    if((_idealangle >= -90.0) && (_idealangle <= 90.0))
    {
        m_ideal_angle = _idealangle;
    }
}

void TBILinearRansac::setAllowedAngleVariance(float _allowedanglevariance)
{
    if((_allowedanglevariance >= 0) && (_allowedanglevariance <=90))
    {
        m_allowed_angle_variance = _allowedanglevariance;
    }
}

void TBILinearRansac::setMinVotes(int _minvotes)
{
    if(_minvotes >= 1)
    {
        m_min_votes = _minvotes;
    }
}

void TBILinearRansac::setIterations(int _iterations)
{
    if(_iterations >= 1)
    {
        m_iterations = _iterations;
    }
}

void TBILinearRansac::setDistanceThreshold(float _distancethreshold)
{
    if(_distancethreshold > 0)
    {
        m_distance_threshold = _distancethreshold;
    }
}

void TBILinearRansac::makeEqual(TBILinearRansac &_ransac)
{
    m_ideal_angle = _ransac.getIdealAngle();
    m_allowed_angle_variance = _ransac.getAllowedAngleVariance();
    m_iterations = _ransac.getIteration();
    m_distance_threshold = _ransac.getDistanceThreshold();
    m_min_votes = _ransac.getMinVotes();
}


