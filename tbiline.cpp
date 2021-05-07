#include "tbiline.h"
#include "math.h"

TBILine::TBILine()
{
    m_point1.setX(0);
    m_point1.setY(0);
    m_point2.setX(0);
    m_point2.setY(0);
    m_validline = false;
    updateInternals();

}

TBILine::TBILine(const TBILine &_line)
{
    this->m_point1 = _line.m_point1;
    this->m_point2 = _line.m_point2;
    updateInternals();
}

void TBILine::setPoint1(TBIPoint _pnt)
{
    m_point1 = _pnt;
    updateInternals();
}

void TBILine::setPoint2(TBIPoint _pnt)
{
    m_point2 = _pnt;
    updateInternals();
}

bool TBILine::canLineBeSplitForSplitMerge(const TBIPoint &_pnt, float _distance) const
{
    return false;
}

float TBILine::getOrthogonalDistance(const TBIPoint &_pnt) const
{
    if(!m_validline) return 0.0;
    return abs((m_slope*_pnt.getX() - _pnt.getY() + m_intercept)/(sqrt(m_slope*m_slope + 1)));
}

float TBILine::interiorAngleToLine(const TBILine &_line) const
{
    if(!m_validline) return 0.0;
    if(!_line.isValid()) return 0.0;
    TBIPoint _v1(_line.getVector());
    TBIPoint _v2(this->getVector());
    float _xa = _v1.getX();
    float _ya = _v1.getY();
    float _xb = _v2.getX();
    float _yb = _v2.getY();

    float _ans = - acos(((_xa*_xb) + (_ya*_yb))/(sqrt((_xa*_xa)+(_ya*_ya))*sqrt((_xb*_xb)+(_yb*_yb))));
    return _ans * 57.2958;
}

float TBILine::angleFromHorizontal() const
{
    if(!m_validline) return 9999999.9999999;

    return -atan(m_slope) * 57.2958;
}

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

TBIPoint TBILine::getPoint1() const
{
    return m_point1;
}

TBIPoint TBILine::getPoint2() const
{
    return m_point2;
}

TBIPoint TBILine::getVector() const
{
    return(TBIPoint((m_point2.getX() - m_point1.getX()),(m_point2.getY() - m_point2.getY())));
}

bool TBILine::findPointofIntersection(const TBILine &_line, TBIPoint &_intersectionpnt)
{
    if(!m_validline) return false;
    if(!_line.isValid()) return false;
    if(m_slope == _line.getSlope()) return false;

    float _x = (m_intercept-_line.getIntercept())/(_line.getSlope() - m_slope);
    float _y = m_slope*_x+m_intercept;
    _intersectionpnt.setX(_x);
    _intersectionpnt.setY(_y);
    return true;
}

void TBILine::clear()
{
    m_point1.zero();
    m_point2.zero();
    updateInternals();
}

void TBILine::remakeLine(int _startcol, int _endcol)
{
    if(m_validline)
    {
        *this = TBILine((float)_startcol, this->getYatX((float)_startcol), (float)_endcol, this->getYatX((float)_endcol));
    }
}

void TBILine::drawOnMat(cv::Mat &_dst, cv::Scalar _color, int _thickness)
{
    if(!m_validline) return;
    cv::Point _p1((int)this->m_point1.getX(), (int)this->m_point1.getY());
    cv::Point _p2((int)this->m_point2.getX(), (int)this->m_point2.getY());
    cv::line(_dst, _p1, _p2, _color, _thickness);

}

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
        m_slope = (m_point1.getY() - m_point2.getY())/(m_point1.getX()-m_point2.getX());
        m_intercept = m_point1.getY() - m_slope*m_point1.getX();
    }
}

void TBILine::updateValidLine()
{

    if(abs((m_point1.getX() - m_point2.getX())) < m_vertical_asymptote_criteria)
    {
        m_validline = false;
    }
    else
    {
        m_validline = true;
    }
}

TBILine::TBILine(const TBIPoint &_p1, const TBIPoint &_p2)
{
    m_point1.setX(_p1.getX());
    m_point1.setY(_p1.getY());
    m_point2.setX(_p2.getX());
    m_point2.setY(_p2.getY());
    updateInternals();
}

TBILine::TBILine(const float _x1, const float _y1, const float _x2, const float _y2)
{
    m_point1 = TBIPoint(_x1, _y1);
    m_point2 = TBIPoint(_x2, _y2);
    updateInternals();
}
