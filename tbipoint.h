#ifndef TBIPOINT_H
#define TBIPOINT_H

#include "opencv4/opencv2/core.hpp"

class TBIPoint
{
public:
    TBIPoint();
    TBIPoint(float _x, float _y);
    TBIPoint(const TBIPoint&_pnt); //Copy Constructor
    void drawtoCVMat(cv::Mat &_dst, uint8_t _value);
    float distance(const TBIPoint &_pnt) const;
    float getX() const {return m_x;}
    void setX(float _x){m_x = _x;}
    float getY() const {return m_y;}
    void setY(float _y){m_y=_y;}
    void zero();
    void inValidate(){m_valid = false;}
    void validate(){m_valid = true;}
    bool isValid(){return m_valid;}

    void operator=(const TBIPoint&_pnt)
    {
        this->m_x = _pnt.m_x;
        this->m_y = _pnt.m_y;
    }

    bool operator==(const TBIPoint&_pnt)
    {
        if((this->m_x==_pnt.m_x) && (this->m_y==_pnt.m_y)) return true;
        return false;
    }


private:
    float m_x;
    float m_y;
    bool m_valid;
};

#endif // TBIPOINT_H
