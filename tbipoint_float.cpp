#include "tbipoint_float.h"
#include "math.h"


float TBIPoint_Float::distance(const TBIPoint_Float &_pnt)
{
    float _dx = m_x - _pnt.m_x;
    float _dy = m_y - _pnt.m_y;
    return sqrtf(_dx*_dx + _dy*_dy);
}

cv::Point TBIPoint_Float::toCVPoint()
{
    cv::Point _cvpnt((int)m_x, (int)m_y);
    return _cvpnt;
}







