#ifndef TBIPoint_Float_FLOAT_H
#define TBIPoint_Float_FLOAT_H

#include "opencv4/opencv2/core.hpp"

class TBIPoint_Float
{
public:
    TBIPoint_Float();
    TBIPoint_Float(float _x, float _y);
    TBIPoint_Float(const TBIPoint_Float&_pnt); //Copy Constructor
    void drawtoCVMat(cv::Mat &_dst, uint8_t _value);
    float distance(const TBIPoint_Float &_pnt) const;
    float getX() const {return m_x;}
    void setX(float _x){m_x = _x;}
    float getY() const {return m_y;}
    void setY(float _y){m_y=_y;}
    void zero();
    void inValidate(){m_valid = false;}
    void validate(){m_valid = true;}
    bool isValid(){return m_valid;}

    void operator=(const TBIPoint_Float&_pnt)
    {
        this->m_x = _pnt.m_x;
        this->m_y = _pnt.m_y;
    }

    bool operator==(const TBIPoint_Float&_pnt)
    {
        if((this->m_x==_pnt.m_x) && (this->m_y==_pnt.m_y)) return true;
        return false;
    }


private:
    float m_x;
    float m_y;
    bool m_valid;
};

#endif // TBIPoint_Float_FLOAT_H
