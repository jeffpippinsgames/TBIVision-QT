#ifndef TBIPOINT_INT_H
#define TBIPOINT_INT_H


#include "tbipoint_float.h"
#include "tbiline.h"


class TBIPoint_Int
{
    public:

    int m_x;
    int m_y;


    TBIPoint_Int(){m_x = 0, m_y = 0;}
    TBIPoint_Int(int _x, int _y){m_x=_x;m_y=_y;}
    TBIPoint_Int(TBIPoint_Float &_pnt){m_x = (int)_pnt.getX(); m_y = (int)_pnt.getY();}
    TBIPoint_Int(TBIPoint_Int &_pnt){m_x=_pnt.m_x; m_y = _pnt.m_y;}
    float distance(TBIPoint_Int &_pnt);
    float distance(TBIPoint_Float &_pnt);
    float distance(TBILine &_line);
    float distanceAbs(TBILine &_line);

    TBIPoint_Int operator + (TBIPoint_Int &_pnt)
    {
        TBIPoint_Int __pnt((m_x + _pnt.m_x), (m_y + _pnt.m_y));
        return __pnt;
    }

    TBIPoint_Int operator - (TBIPoint_Int &_pnt)
    {
        TBIPoint_Int __pnt((m_x - _pnt.m_x), (m_y - _pnt.m_y));
        return __pnt;
    }

    void operator = (TBIPoint_Int &_pnt)
    {
        m_x=_pnt.m_x;
        m_y=_pnt.m_y;
    }

    bool operator == (TBIPoint_Int &_pnt)
    {
        if((m_x==_pnt.m_x) && (m_y==_pnt.m_y)) return true;
        return false;
    }



};

#endif // TBIPOINT_INT_H
