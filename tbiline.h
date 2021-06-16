#ifndef TBILINE_H
#define TBILINE_H

#include "tbipoint_float.h"
#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/imgproc.hpp"

class TBILine
{
public:

    TBILine();
    TBILine(const TBIPoint_Float &_p1, const TBIPoint_Float &_p2);
    TBILine(const float _x1, const float _y1, const float _x2, const float _y2);
    TBILine(const TBILine &_line); //Copy Constructor
    void setPoint1(TBIPoint_Float _pnt);
    void setPoint2(TBIPoint_Float _pnt);
    bool canLineBeSplitForSplitMerge(float *_data, int _start_index, int _end_index, int *_split_index, float _distance_threshold) const;
    float getOrthogonalDistance(const TBIPoint_Float &_pnt) const;
    float getOrthogonalDistance(float _x, float _y) const;
    float interiorAngleToLine(const TBILine &_line) const;
    float angleFromHorizontal() const;
    float getXatY(float _y) const;
    float getYatX(float _x) const;
    float getSlope() const {return m_slope;}
    float getIntercept() const {return m_intercept;}
    float getLength() const {return m_length;}
    bool isValid() const {return m_validline;}
    TBIPoint_Float getPoint1() const;
    TBIPoint_Float getPoint2() const;
    TBIPoint_Float getVector() const;
    bool findPointofIntersection(const TBILine &_line, TBIPoint_Float &_intersectionpnt);
    void clear();
    void remakeLine(int _startcol, int _endcol);
    void drawOnMat(cv::Mat &_dst, cv::Scalar _color, int _thickness = 1);
    void drawOnMat(cv::Mat &_dst);
    float getPoint1X() const{return m_point1.getX();}
    float getPoint1Y() const{return m_point1.getY();}
    float getPoint2X() const{return m_point2.getX();}
    float getPoint2Y() const{return m_point2.getY();}
    bool compareLines(TBILine &_line, float _distance_threshold);
    int getThickness(){return m_thickness;}
    void setThickness(int _thickness){if(_thickness > 0)m_thickness=_thickness;}
    void setColor(cv::Scalar _color) {m_color = _color;}
    void setColor(uint8_t _r, uint8_t _g, uint8_t _b) { m_color = CV_RGB(_r,_g,_b);}
    bool isPointAboveLine(const TBIPoint_Float &_pnt) const;
    bool isPointBelowLine(const TBIPoint_Float &_pnt) const;
    bool isPointOnLine (const TBIPoint_Float &_pnt) const;



    void operator = (const TBILine &_line)
    {
        this->m_point1 = _line.m_point1;
        this->m_point2 = _line.m_point2;
        this->updateInternals();
    }

    bool operator == (const TBILine &_line)
    {
        if((this->m_point1==_line.m_point1) && (this->m_point2==_line.m_point2)) return true;
        return false;
    }

private:
    const float m_vertical_asymptote_criteria = .0001;
    TBIPoint_Float m_point1;
    TBIPoint_Float m_point2;
    float m_slope; //yintercept form
    float m_intercept;
    float m_length;
    bool m_validline;
    int m_thickness;
    cv::Scalar m_color;


    void updateInternals();
    void updateLength();
    void updateSlopeandIntercept();
    void updateValidLine();



};

#endif // TBILINE_H

