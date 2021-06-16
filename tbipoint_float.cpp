#include "tbipoint_float.h"
#include "math.h"

TBIPoint_Float::TBIPoint_Float()
{
    m_x = 0;
    m_y = 0;
    m_valid = true;
}

TBIPoint_Float::TBIPoint_Float(float _x, float _y)
{
    m_x = _x;
    m_y = _y;
}

TBIPoint_Float::TBIPoint_Float(const TBIPoint_Float &_pnt)
{
    this->m_x=_pnt.m_x;
    this->m_y=_pnt.m_y;
}

void TBIPoint_Float::drawtoCVMat(cv::Mat &_dst, uint8_t _value)
{
    if(_dst.channels() != 1) return;
    if(_dst.rows == -1) return;
    if(_dst.cols == -1) return;
    if(m_x < 0) return;
    if(m_y < 0) return;
    if(m_x >= _dst.rows) return;
    if(m_y >= _dst.cols) return;

    int _dataindex = (m_y * _dst.cols) + m_x;
    _dst.data[_dataindex] = _value;
}

float TBIPoint_Float::distance(const TBIPoint_Float &_pnt) const
{
    return sqrt(((m_x - _pnt.m_x)*(m_x - _pnt.m_x)) + ((m_y - _pnt.m_y)*(m_y - _pnt.m_y)));
}

void TBIPoint_Float::zero()
{
    m_x = 0;
    m_y = 0;
}





