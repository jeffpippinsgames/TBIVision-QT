#include "tbiclass_line.h"
#include "math.h"


TBILine::TBILine()
{

    m_validline = false;
    updateInternals();
    m_thickness = 1;
    m_color = CV_RGB(255,255,255);

}

//Get Functions-----------------------------------------------------------------
float TBILine::getXatY(float _y) const
{
    if(!m_validline) return 99999.99999;
    if(m_slope < m_vertical_asymptote_criteria) return _y;
    return (_y-m_intercept)/m_slope;
}

float TBILine::getYatX(float _x) const
{
    if(!m_validline) return 99999.99999;
    return m_slope*_x+m_intercept;
}

TBIPoint_Float TBILine::getPoint1() const
{

    return m_point1;
}

TBIPoint_Float TBILine::getPoint2() const
{
    return m_point2;
}

TBIPoint_Float TBILine::getVector() const
{
    return(TBIPoint_Float((m_point2.m_x - m_point1.m_x),(m_point2.m_y - m_point2.m_y)));
}

TBIPoint_Int TBILine::getIntMidPointofLine()
{
    TBIPoint_Int _pnt;
    if(!this->isValid())
    {
        _pnt.valid = false;
        return _pnt;
    }
    _pnt.m_x = (m_point1.m_x + m_point2.m_x)/2;
    _pnt.m_y = (m_point1.m_y + m_point2.m_y) /2;
    _pnt.valid = true;
    return _pnt;
}

TBIPoint_Float TBILine::getFloatMidPointofLine()
{
    TBIPoint_Float _pnt;

    _pnt.m_x = (m_point1.m_x + m_point2.m_x)/2;
    _pnt.m_y = (m_point1.m_y + m_point2.m_y) /2;

    return _pnt;
}


//Set Functions------------------------------------------------------------------
void TBILine::setPoint1(TBIPoint_Float _pnt)
{
    m_point1 = _pnt;
    updateInternals();
}

void TBILine::setPoint2(TBIPoint_Float _pnt)
{
    m_point2 = _pnt;
    updateInternals();
}


//Distance Functions------------------------------------------------------------
float TBILine::distance(const TBIPoint_Float &_pnt) const
{
    if(!m_validline) return 0.0;
    return (m_slope*_pnt.m_x - _pnt.m_y + m_intercept)/(sqrt(m_slope*m_slope + 1));
}

float TBILine::distance(const TBIPoint_Int &_pnt) const
{
    if(!m_validline) return 0.0;
    return (m_slope*(float)_pnt.m_x - (float)_pnt.m_y + m_intercept)/(sqrt(m_slope*m_slope + 1));
}

float TBILine::distance(float _x, float _y) const
{
    if(!m_validline) return 0.0;
    return (m_slope*_x - _y + m_intercept)/(sqrt(m_slope*m_slope + 1));
}

float TBILine::distanceAbs(const TBIPoint_Float &_pnt) const
{
    if(!m_validline) return 0.0;
    return abs((m_slope*_pnt.m_x - _pnt.m_y + m_intercept)/(sqrt(m_slope*m_slope + 1)));
}

float TBILine::distanceAbs(const TBIPoint_Int &_pnt) const
{
    if(!m_validline) return 0.0;
    return abs((m_slope*(float)_pnt.m_x - (float)_pnt.m_y + m_intercept)/(sqrt(m_slope*m_slope + 1)));
}

float TBILine::distanceAbs(float _x, float _y) const
{
    if(!m_validline) return 0.0;
    return abs((m_slope*_x - _y + m_intercept)/(sqrt(m_slope*m_slope + 1)));
}

//OpenCV Functions---------------------------------------------------------------

void TBILine::drawOnMat(cv::Mat &_dst, cv::Scalar _color, int _thickness)
{
    if(!m_validline) return;
    cv::Point _p1((int)this->m_point1.m_x, (int)this->m_point1.m_y);
    cv::Point _p2((int)this->m_point2.m_x, (int)this->m_point2.m_y);
    cv::line(_dst, _p1, _p2, _color, _thickness, cv::LINE_AA);

}

void TBILine::drawOnMat(cv::Mat &_dst)
{
    if(!m_validline) return;
    cv::Point _p1((int)this->m_point1.m_x, (int)this->m_point1.m_y);
    cv::Point _p2((int)this->m_point2.m_x, (int)this->m_point2.m_y);
    cv::line(_dst, _p1, _p2, m_color, m_thickness, cv::LINE_AA);
}


//Utility Functions--------------------------------------------------------------
bool TBILine::canLineBeSplitForSplitMerge(float *_data, int _start_index, int _end_index, int *_split_index, float _distance_threshold) const
{
    return true;
}

float TBILine::angleFromHorizontal() const
{
    if(!m_validline) return 9999999.9999999;

    return -atan(m_slope) * 57.2958;
}

bool TBILine::findPointofIntersection(const TBILine &_line, TBIPoint_Float &_intersectionpnt)
{
    if(!m_validline) return false;
    if(!_line.isValid()) return false;
    if(m_slope == _line.getSlope()) return false;

    float _x = (m_intercept-_line.getIntercept())/(_line.getSlope() - m_slope);
    float _y = m_slope*_x+m_intercept;
    _intersectionpnt.m_x = _x;
    _intersectionpnt.m_y = _y;
    return true;
}

bool TBILine::findPointofIntersection(const TBILine &_line, TBIPoint_Int &_intersectionpnt)
{
    if(!m_validline) return false;
    if(!_line.isValid()) return false;
    if(m_slope == _line.getSlope()) return false;

    float _x = (m_intercept-_line.getIntercept())/(_line.getSlope() - m_slope);
    float _y = m_slope*_x+m_intercept;
    _intersectionpnt.m_x = (int)_x;
    _intersectionpnt.m_y = (int)_y;
    return true;
}

void TBILine::clear()
{
    m_point1.m_x=0;
    m_point1.m_y=0;
    m_point2.m_x=0;
    m_point2.m_y=0;
    updateInternals();
}

void TBILine::remakeLine(int _startcol, int _endcol)
{
    if(m_validline)
    {
        *this = TBILine((float)_startcol, this->getYatX((float)_startcol), (float)_endcol, this->getYatX((float)_endcol));
    }
}

bool TBILine::compareLines(TBILine &_line, float _distance_threshold)
{
    if(!m_validline) return false;
    if(!_line.isValid()) return false;

    float _distance1 = _line.getPoint1().distance(m_point1);
    float _distance2 = _line.getPoint2().distance(m_point2);
    if((_distance1 + _distance2) <= _distance_threshold ) return true;
    _distance1 = _line.getPoint1().distance(m_point2);
    _distance2 = _line.getPoint2().distance(m_point1);
    if((_distance1 + _distance2) <= _distance_threshold ) return true;
    return false;
}

bool TBILine::isPointAboveLine(const TBIPoint_Float &_pnt) const
{

    //Note: Because The System Stays In Basic Screen Coordinates Where Y Down is Positive
    //The Slopes are actually inverse to the standard model.

    if(!m_validline) return false;
    float _x1 = _pnt.m_x;
    float _y1 = _pnt.m_y;
    float _x2 = (_x1 + m_slope*(_y1 - m_intercept))/(1+(m_slope*m_slope));
    float _y2 = ((m_slope*_x1) + (m_slope*m_slope*_y1) + m_intercept) / (1+(m_slope*m_slope));

    float _i = _x1 - _x2;
    float _j = _y1 - _y2;


    if(m_slope == 0)
    {
        if(_j < 0) return true;

    }
    if(m_slope > 0)
    {
        if((_j < 0) && (_i > 0)) return true;
    }
    if(m_slope < 0)
    {
        if((_j < 0) && (_i < 0)) return true;
    }
    return false;
}

bool TBILine::isPointBelowLine(const TBIPoint_Float &_pnt) const
{
    //Note: Because The System Stays In Basic Screen Coordinates Where Y Down is Positive
    //The Slopes are actually inverse to the standard model.

    if(!m_validline) return false;
    float _x1 = _pnt.m_x;
    float _y1 = _pnt.m_y;
    float _x2 = (_x1 + m_slope*(_y1 - m_intercept))/(1+(m_slope*m_slope));
    float _y2 = ((m_slope*_x1) + (m_slope*m_slope*_y1) + m_intercept) / (1+(m_slope*m_slope));

    float _i = _x1 - _x2;
    float _j = _y1 - _y2;

    if(m_slope == 0)
    {
        if(_j > 0 ) return true;
    }
    if(m_slope > 0)
    {
        if((_j > 0) && (_i < 0)) return true;
    }
    if(m_slope < 0)
    {
        if((_j > 0) && (_i > 0)) return true;
    }
    return false;
}

bool TBILine::isPointOnLine(const TBIPoint_Float &_pnt) const
{
    if(!m_validline) return false;

    float _dist = this->distanceAbs(_pnt);
    if(_dist < .00010) return true;
    return false;
}


//Internal Update Functions.
void TBILine::updateInternals()
{
    updateValidLine();
    if(m_validline)
    {
        updateLength();
        updateSlopeandIntercept();
    }
    else
    {
        m_length = 0.0;
        m_slope = 0.0;
        m_intercept = 0.0;
    }
}

void TBILine::updateLength()
{
    m_length = m_point1.distance(m_point2);
}

void TBILine::updateSlopeandIntercept()
{
    if(!m_validline)
    {
        m_slope = 0;
        m_intercept = 0;
    }
    else
    {
        m_slope = (m_point1.m_y - m_point2.m_y)/(m_point1.m_x-m_point2.m_x);
        m_intercept = m_point1.m_y - m_slope*m_point1.m_x;
    }
}

void TBILine::updateValidLine()
{

    if(abs((m_point1.m_x - m_point2.m_x)) < m_vertical_asymptote_criteria)
    {
        m_validline = false;
    }
    else
    {
        m_validline = true;
    }
}

