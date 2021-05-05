#ifndef TBIPOINT_H
#define TBIPOINT_H

#include "opencv4/opencv2/core.hpp"

class TBIPoint
{
public:
    TBIPoint();
    TBIPoint(float _x, float _y);
    void drawtoCVMat(cv::Mat &_dst, uint8_t _value);
    float distance(TBIPoint &_pnt);
    bool isEqual(TBIPoint &_pnt);
    float getX(){return m_x;}
    void setX(float _x){m_x = _x;}
    float getY(){return m_y;}
    void setY(float _y){m_y=_y;}

private:
    float m_x;
    float m_y;
};

#endif // TBIPOINT_H
