#ifndef TBIPOINT_INT_H
#define TBIPOINT_INT_H




class TBIPoint_Int
{
    public:

    int m_x;
    int m_y;


    TBIPoint_Int(){m_x = 0, m_y = 0;}
    TBIPoint_Int(int _x, int _y){m_x=_x;m_y=_y;}
    TBIPoint_Int(const TBIPoint_Int &_pnt){m_x=_pnt.m_x; m_y = _pnt.m_y;}
    float distance(const TBIPoint_Int &_pnt);



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

#endif // TBIPOINT_INT_H
