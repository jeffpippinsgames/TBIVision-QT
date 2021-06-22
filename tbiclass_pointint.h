#ifndef TBICLASS_POINTINT_H
#define TBICLASS_POINTINT_H


#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/imgproc/imgproc.hpp"

class TBIPoint_Int
{
    public:

    int m_x;
    int m_y;


    TBIPoint_Int(){m_x = 0, m_y = 0;}
    TBIPoint_Int(int _x, int _y){m_x=_x;m_y=_y;}
    TBIPoint_Int(const TBIPoint_Int &_pnt){m_x=_pnt.m_x; m_y = _pnt.m_y;}
    float distance(const TBIPoint_Int &_pnt);
    cv::Point toCVPoint(){return cv::Point(m_x, m_y);}


    TBIPoint_Int operator + (const TBIPoint_Int &_pnt)
    {
        TBIPoint_Int __pnt((m_x + _pnt.m_x), (m_y + _pnt.m_y));
        return __pnt;
    }


    TBIPoint_Int operator - (const TBIPoint_Int &_pnt)
    {
        TBIPoint_Int __pnt((m_x - _pnt.m_x), (m_y - _pnt.m_y));
        return __pnt;
    }

    void operator = (const TBIPoint_Int &_pnt)
    {
        m_x=_pnt.m_x;
        m_y=_pnt.m_y;
    }

    bool operator == (const TBIPoint_Int &_pnt)
    {
        if((m_x==_pnt.m_x) && (m_y==_pnt.m_y)) return true;
        return false;
    }



};

#endif // TBICLASS_POINTINT_H
