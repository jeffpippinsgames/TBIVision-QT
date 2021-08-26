#ifndef TBICLASS_LINE_H
#define TBICLASS_LINE_H

#include "tbiclass_pointfloat.h"
#include "tbiclass_pointint.h"
#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/imgproc.hpp"


class TBILine
{
public:

    TBILine();
    TBILine(const TBIPoint_Float &_p1, const TBIPoint_Float &_p2){m_point1 = _p1; m_point2 = _p2; updateInternals();}
    TBILine(const TBIPoint_Int &_p1, const TBIPoint_Int &_p2){m_point1.m_x=(float)_p1.m_x; m_point1.m_y=(float)_p1.m_y;m_point2.m_x=(float)_p2.m_x,m_point2.m_y=(float)_p2.m_y;updateInternals();}
    TBILine(const float _x1, const float _y1, const float _x2, const float _y2){m_point1.m_x=_x1;m_point1.m_y=_y1;m_point2.m_x=_x2;m_point2.m_y=_y2;updateInternals();}
    TBILine(const TBILine &_line){*this = _line;}


    //Get Functions------------------------------------------------------------
    float getXatY(float _y) const;
    float getYatX(float _x) const;
    float getSlope() const {return m_slope;}
    float getIntercept() const {return m_intercept;}
    float getLength() const {return m_length;}
    float getPoint1X() const{return m_point1.m_x;}
    float getPoint1Y() const{return m_point1.m_y;}
    float getPoint2X() const{return m_point2.m_x;}
    float getPoint2Y() const{return m_point2.m_y;}
    TBIPoint_Float getPoint1() const;
    TBIPoint_Float getPoint2() const;
    TBIPoint_Float getVector() const;
    int getThickness(){return m_thickness;}
    TBIPoint_Int getMidPointofLine();

    //Set Functions------------------------------------------------------------
    void setPoint1(TBIPoint_Float _pnt);
    void setPoint2(TBIPoint_Float _pnt);
    void setColor(cv::Scalar _color) {m_color = _color;}
    void setColor(uint8_t _r, uint8_t _g, uint8_t _b) { m_color = CV_RGB(_r,_g,_b);}
    void setThickness(int _thickness){if(_thickness > 0)m_thickness=_thickness;}

    //Distance Functions------------------------------------------------------------
    float distanceAbs(const TBIPoint_Float &_pnt) const;
    float distanceAbs(const TBIPoint_Int &_pnt) const;
    float distanceAbs(float _x, float _y) const;
    float distance(const TBIPoint_Float &_pnt) const;
    float distance(const TBIPoint_Int &_pnt) const;
    float distance(float _x, float _y) const;

    //OpenCV Functions---------------------------------------------------------------
    void drawOnMat(cv::Mat &_dst, cv::Scalar _color, int _thickness = 1);
    void drawOnMat(cv::Mat &_dst);

    //Utility Functions--------------------------------------------------------------
    bool canLineBeSplitForSplitMerge(float *_data, int _start_index, int _end_index, int *_split_index, float _distance_threshold) const;
    float angleFromHorizontal() const;
    bool isValid() const {return m_validline;}
    bool findPointofIntersection(const TBILine &_line, TBIPoint_Float &_intersectionpnt);
    bool findPointofIntersection(const TBILine &_line, TBIPoint_Int &_intersectionpnt);
    void clear();
    void remakeLine(int _startcol, int _endcol);
    bool compareLines(TBILine &_line, float _distance_threshold);


    bool isPointAboveLine(const TBIPoint_Float &_pnt) const;
    bool isPointBelowLine(const TBIPoint_Float &_pnt) const;
    bool isPointOnLine (const TBIPoint_Float &_pnt) const;


    //Overloaded Operators-----------------------------------------------
    void operator = (const TBILine &_line)
    {
        this->m_point1 = _line.m_point1;
        this->m_point2 = _line.m_point2;
        this->m_color = _line.m_color;
        this->m_intercept = _line.m_intercept;
        this->m_length = _line.m_length;
        this->m_slope = _line.m_slope;
        this->m_thickness = _line.m_thickness;
        this->m_validline = _line.m_validline;
    }

    bool operator == (const TBILine &_line)
    {
        if((this->m_point1 ==_line.m_point1) && (this->m_point2 ==_line.m_point2)) return true;
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

    //Private Functions-----------------------------------------
    void updateInternals();
    void updateLength();
    void updateSlopeandIntercept();
    void updateValidLine();
};

#endif // TBICLASS_LINE_H

