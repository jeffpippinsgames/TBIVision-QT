#include "tbiclass_pointfloat.h"
#include "math.h"


float TBIPoint_Float::distance(const TBIPoint_Float &_pnt)
{
    float _dx = m_x - _pnt.m_x;
    float _dy = m_y - _pnt.m_y;
    return sqrtf(_dx*_dx + _dy*_dy);
}

float TBIPoint_Float::vectorMagnitude()
{
    return sqrtf(this->m_x*this->m_x + this->m_y*this->m_y);
}

TBIPoint_Float TBIPoint_Float::getUnitVecor()
{
    TBIPoint_Float _vect;
    float _magnitude = sqrtf(this->m_x*this->m_x + this->m_y*this->m_y);
    _vect.m_x = this->m_x/_magnitude;
    _vect.m_y = this->m_y/_magnitude;
    return _vect;
}

TBIPoint_Float TBIPoint_Float::getVectorBetweenPoint(const TBIPoint_Float &_pnt)
{
    TBIPoint_Float _vect;
    _vect.m_x = this->m_x - _pnt.m_x;
    _vect.m_y = this->m_y - _pnt.m_y;
}



cv::Point TBIPoint_Float::toCVPoint()
{
    cv::Point _cvpnt((int)round(m_x), (int)round(m_y));
    return _cvpnt;
}









