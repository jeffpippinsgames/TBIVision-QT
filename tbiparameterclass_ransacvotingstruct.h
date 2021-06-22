#ifndef TBIPARAMETERCLASS_RANSACVOTINGSTRUCT_H
#define TBIPARAMETERCLASS_RANSACVOTINGSTRUCT_H

#include "tbiclass_line.h"
#include "tbiclass_dataset.h"

struct TBIRansacVotingStructure
{
    int m_numvotes;
    float m_angletohorizontal;
    TBILine m_line;


    TBIRansacVotingStructure()
    {
        m_numvotes = 0;
        m_angletohorizontal = 99999.99999;
    }

};

#endif // TBIPARAMETERCLASS_RANSACVOTINGSTRUCT_H
