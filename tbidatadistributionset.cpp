#include "tbidatadistributionset.h"

TBIDataDistributionSet::TBIDataDistributionSet()
{
    m_size = 0;
    int _index=0;
    do
    {
        m_data[_index] = 0;
        ++_index;
    }while(_index < TBIConstants::Max_Camera_Width);
}

void TBIDataDistributionSet::clear()
{
    int _index = 0;
    do
    {
        m_data[_index] = 0;
        ++_index;
    }while(_index < m_size);
    m_size = 0;
}

