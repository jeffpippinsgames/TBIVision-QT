#ifndef TBITOPSURFACELINEVOTINGSTRUCTURE_H
#define TBITOPSURFACELINEVOTINGSTRUCTURE_H

#include "tbiline.h"

struct TBILineVotingStructure
{
    int m_numvotes;
    float m_angletohorizontal;
    TBILine m_line;

    TBILineVotingStructure()
    {
        m_numvotes = 0;
        m_angletohorizontal = 99999.99999;
    }
    TBILineVotingStructure(const TBILineVotingStructure& _lvs)
    {
        m_numvotes = _lvs.m_numvotes;
        m_angletohorizontal = _lvs.m_angletohorizontal;
        m_line.setPoint1(_lvs.m_line.getPoint1());
        m_line.setPoint2(_lvs.m_line.getPoint2());
    }
};

#endif // TBITOPSURFACELINEVOTINGSTRUCTURE_H
