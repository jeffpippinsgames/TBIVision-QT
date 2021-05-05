#include "tbipoint.h"
#include "math.h"

TBIPoint::TBIPoint()
{
    m_x = 0;
    m_y = 0;
}

TBIPoint::TBIPoint(float _x, float _y)
{
    m_x = _x;
    m_y = _y;
}

void TBIPoint::drawtoCVMat(cv::Mat &_dst, uint8_t _value)
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

float TBIPoint::distance(TBIPoint &_pnt)
{
    float _x2 = _pnt.getX();
    float _y2 = _pnt.getY();
    return sqrt(((m_x - _x2)*(m_x - _x2)) + ((m_y - _y2)*(m_y - _y2)));
}

bool TBIPoint::isEqual(TBIPoint &_pnt)
{
    if((_pnt.getX() == m_x) && (_pnt.getY() == m_y)) return true;
    return false;
}



