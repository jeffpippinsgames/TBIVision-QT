#include "tbipoint_int.h"

float TBIPoint_Int::distance(TBIPoint_Int &_pnt)
{
    float _dx = (float)m_x - (float)_pnt.m_x;
    float _dy = (float)m_y - (float)_pnt.m_y;
    return sqrtf(_dx*_dx + _dy*_dy);
}

float TBIPoint_Int::distance(TBIPoint_Float &_pnt)
{
    float _dx = (float)m_x - _pnt.getX();
    float _dy = (float)m_y - _pnt.getY();
    return sqrtf(_dx*_dx + _dy*_dy);
}

float TBIPoint_Int::distance(TBILine &_line)
{
    if(!_line.isValid()) return 0.0;
    float _ls = _line.getSlope();
    float _li = _line.getIntercept();
    return (_ls * m_x - m_y + _li)/(sqrt(_ls * _ls + 1));
}

float TBIPoint_Int::distanceAbs(TBILine &_line)
{
    if(!_line.isValid()) return 0.0;
    float _ls = _line.getSlope();
    float _li = _line.getIntercept();
    return abs((_ls * m_x - m_y + _li)/(sqrt(_ls * _ls + 1)));
}
