#ifndef TBILINEARRANSACVOTINGSTRUCTURE_H
#define TBILINEARRANSACVOTINGSTRUCTURE_H

#include "tbiline.h"
#include "tbidataset.h"

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

};

#endif // TBILINEARRANSACVOTINGSTRUCTURE_H
