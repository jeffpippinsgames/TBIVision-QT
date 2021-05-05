#ifndef TBILINE_H
#define TBILINE_H

#include "tbipoint.h"

class TBILine
{
public:
    TBILine();
    TBILine(TBIPoint _p1, TBIPoint _p2);
    TBILine(float _x1, float _y1, float _x2, float _y2);
    void setPoint1(TBIPoint _pnt);
    void setPoint2(TBIPoint _pnt);
    bool canLineBeSplitForSplitMerge(TBIPoint &_pnt, float _distance);
    float getOrthogonalDistance(TBIPoint &_pnt);
    float interiorAngleToLine(TBILine &_line);
    float getXatY(float _y);
    float getYatX(float _x);
    float getSlope(){return m_slope;}
    float getIntercept(){return m_intercept;}
    float getAFH(){return m_anglefromhorizontal;}
    float getLength(){return m_length;}
    bool isValid(){return m_validline;}

private:
    TBIPoint m_point1;
    TBIPoint m_point2;
    float m_slope; //yintercept form
    float m_intercept;
    float m_length;
    float m_anglefromhorizontal;
    bool m_validline;
    float m_a; //General form of a Line
    float m_b;
    float m_c;

    void updateInternals();
    void updateLength();
    void updateSlopeIntercept();
    void updateValidLine();
    void updateAFH(); //AngleFromHorizontal



};

#endif // TBILINE_H
