#ifndef TBILINEARRANSACVOTINGSTRUCTURE_H
#define TBILINEARRANSACVOTINGSTRUCTURE_H

#include "tbiline.h"

struct TBILinearRansacVotingStructure
{
    int m_numvotes;
    float m_angletohorizontal;
    TBILine m_line;

    TBILinearRansacVotingStructure()
    {
        m_numvotes = 0;
        m_angletohorizontal = 99999.99999;
    }
    TBILinearRansacVotingStructure(const TBILinearRansacVotingStructure& _lvs)
    {
        m_numvotes = _lvs.m_numvotes;
        m_angletohorizontal = _lvs.m_angletohorizontal;
        m_line.setPoint1(_lvs.m_line.getPoint1());
        m_line.setPoint2(_lvs.m_line.getPoint2());
    }
};

#endif // TBILINEARRANSACVOTINGSTRUCTURE_H
