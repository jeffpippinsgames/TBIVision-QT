#ifndef TBIPoint_Float_FLOAT_H
#define TBIPoint_Float_FLOAT_H

#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/imgproc/imgproc.hpp"

class TBIPoint_Float
{
public:

    float m_x;
    float m_y;

    TBIPoint_Float(){m_x = 0, m_y = 0;}
    TBIPoint_Float(float _x, float _y){m_x=_x;m_y=_y;}
    TBIPoint_Float(int _x, int _y){m_x=(float)_x;m_y=(float)_y;}
    TBIPoint_Float(const TBIPoint_Float &_pnt){m_x = _pnt.m_x; m_y = _pnt.m_y;}

    float distance(const TBIPoint_Float &_pnt);
    float vectorMagnitude();
    TBIPoint_Float getUnitVecor();
    TBIPoint_Float getVectorBetweenPoint(const TBIPoint_Float &_pnt);
    cv::Point toCVPoint();





    TBIPoint_Float operator /(const float _scalar)
    {
        TBIPoint_Float _pnt(this->m_x/_scalar, this->m_y/_scalar);
        return _pnt;
    }

    TBIPoint_Float operator *(const int _scalar)
    {
        TBIPoint_Float _pnt((m_x*(float)_scalar), (m_y*(float)_scalar));
        return _pnt;
    }

    TBIPoint_Float operator *(const float _scalar)
    {
        TBIPoint_Float _pnt((m_x*_scalar), (m_y*_scalar));
        return _pnt;
    }

    TBIPoint_Float operator + (const TBIPoint_Float &_pnt)
    {
        TBIPoint_Float __pnt((m_x + _pnt.m_x), (m_y + _pnt.m_y));
        return __pnt;
    }


    TBIPoint_Float operator - (const TBIPoint_Float &_pnt)
    {
        TBIPoint_Float __pnt((m_x - _pnt.m_x), (m_y - _pnt.m_y));
        return __pnt;
    }

    void operator = (const TBIPoint_Float &_pnt)
    {
        m_x=_pnt.m_x;
        m_y=_pnt.m_y;
    }

    bool operator == (const TBIPoint_Float &_pnt)
    {
        if((m_x==_pnt.m_x) && (m_y==_pnt.m_y)) return true;
        return false;
    }


};

#endif // TBIPoint_Float_FLOAT_H
