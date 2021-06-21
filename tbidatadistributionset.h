#ifndef TBIDATADISTRIBUTIONSET_H
#define TBIDATADISTRIBUTIONSET_H

#include "tbiconstants.h"


class TBIDataDistributionSet
{
public:
    TBIDataDistributionSet();

    int size(){return m_size;}
    void clear();
    void incrementIndex(int _index){++m_data[_index]; if(m_size < (_index+1)) m_size = _index + 1;}

    int operator []  (int _index) const
    {
        return m_data[_index];
    }

private:
    int m_data[TBIConstants::Max_Camera_Width];
    int m_size;
};

#endif // TBIDATADISTRIBUTIONSET_H
