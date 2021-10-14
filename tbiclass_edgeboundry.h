#ifndef TBIEDGEBOUNDRY_H
#define TBIEDGEBOUNDRY_H

#include "tbiclass_pointint.h"
#include "tbiclass_pointfloat.h"

class TBIEdgeBoundry
{
public:
    TBIEdgeBoundry();

private:

    TBIPoint_Int m_upper_boundry;
    TBIPoint_Int m_lower_boundry;
    TBIPoint_Float m_mid_point;
    float m_length;
};

#endif // TBIEDGEBOUNDRY_H
