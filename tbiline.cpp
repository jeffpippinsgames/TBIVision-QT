#include "tbiline.h"
#include "math.h"

TBILine::TBILine()
{
    m_point1.setX(0);
    m_point1.setY(0);
    m_point2.setX(0);
    m_point2.setY(0);
    m_validline = false;
    updateValidLine();

}

void TBILine::setPoint1(TBIPoint _pnt)
{
    m_point1.setX(_pnt.getX());
    m_point1.setY(_pnt.getY());
    updateValidLine();
}

void TBILine::setPoint2(TBIPoint _pnt)
{
    m_point2.setX(_pnt.getX());
    m_point2.setY(_pnt.getY());
    updateValidLine();
}

void TBILine::updateLength()
{
    m_length = m_point1.distance(m_point2);
}

void TBILine::updateSlopeIntercept()
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

    if(abs((m_point1.getY() - m_point2.getY())) < .00001)
    {
        m_validline = false;
    }
    else
    {
        m_validline = true;
    }
}

TBILine::TBILine(TBIPoint _p1, TBIPoint _p2)
{
    m_point1.setX(_p1.getX());
    m_point1.setY(_p1.getY());
    m_point2.setX(_p1.getX());
    m_point2.setY(_p1.getY());
    updateValidLine();
}
