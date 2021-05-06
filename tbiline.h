#ifndef TBILINE_H
#define TBILINE_H

#include "tbipoint.h"
#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/imgproc.hpp"

class TBILine
{
public:

    TBILine();
    TBILine(const TBIPoint &_p1, const TBIPoint &_p2);
    TBILine(const float _x1, const float _y1, const float _x2, const float _y2);
    TBILine(const TBILine &_line);
    void setPoint1(TBIPoint _pnt);
    void setPoint2(TBIPoint _pnt);
    void setLine(float _x1, float _y1, float _x2, float _y2);
    void copyLine(const TBILine &_line);
    bool canLineBeSplitForSplitMerge(const TBIPoint &_pnt, float _distance) const;
    float getOrthogonalDistance(const TBIPoint &_pnt) const;
    float interiorAngleToLine(const TBILine &_line) const;
    float angleFromHorizontal() const;
    float getXatY(float _y) const;
    float getYatX(float _x) const;
    float getSlope() const {return m_slope;}
    float getIntercept() const {return m_intercept;}
    float getLength() const {return m_length;}
    bool isValid() const {return m_validline;}
    TBIPoint getPoint1() const;
    TBIPoint getPoint2() const;
    TBIPoint getVector() const;
    void clear();
    void makeLeftTSLLine(int _flattened_iohr);
    void makeRightTSLLine(int _flattened_iohr, int _maxcols);
    void drawOnMat(cv::Mat &_dst, cv::Scalar _color, int _thickness = 1);

    TBILine operator = (const TBILine &_line)
    {
        TBILine __line(_line);
        return __line;
    }

    bool operator == (const TBILine &_line)
    {
        if((this->m_point1==_line.m_point1) && (this->m_point2==_line.m_point2)) return true;
        return false;
    }

private:
    const float m_vertical_asymptote_criteria = .0001;
    TBIPoint m_point1;
    TBIPoint m_point2;
    float m_slope; //yintercept form
    float m_intercept;
    float m_length;
    bool m_validline;


    void updateInternals();
    void updateLength();
    void updateSlopeandIntercept();
    void updateValidLine();



};

#endif // TBILINE_H
