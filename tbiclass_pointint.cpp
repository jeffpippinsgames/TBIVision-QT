#include "tbiclass_pointint.h"
#include "math.h"

float TBIPoint_Int::distance(const TBIPoint_Int &_pnt)
{
    float _dx = (float)m_x - (float)_pnt.m_x;
    float _dy = (float)m_y - (float)_pnt.m_y;
    return sqrtf(_dx*_dx + _dy*_dy);
}
