#ifndef TBIPARAMETERCLASS_IMAGEINTENSITY_H
#define TBIPARAMETERCLASS_IMAGEINTENSITY_H

#include <stdint.h>

class TBIImageIntensityParameters
{
public:
    TBIImageIntensityParameters();
    uint64_t m_tii;
    uint64_t m_max_tii;
    uint64_t m_min_tii;
};

#endif // TBIPARAMETERCLASS_IMAGEINTENSITY_H
