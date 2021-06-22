#include "tbiclass_datadistributionset.h"
#include "math.h"

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

float TBIDataDistributionSet::standardDeviation()
{
    if(m_size == 0) return 0.0;
    float _mean=0;
    int _index=0;
    int _total_elements=0;
    float _standard_dev=0;
    float _sum=0;

    do
    {
        if(m_data[_index] > 0)
        {
            _mean += (float)m_data[_index];
            ++_total_elements;
        }
        ++_index;
    }while(_index < m_size);
    _mean = _mean/_total_elements;
    _index = 0;

    do
    {
        if(m_data[_index] > 0)
        {
            _sum += ((float)m_data[_index] - _mean) * ((float)m_data[_index] - _mean);
        }
        ++_index;
    }while(_index < m_size);


    if(_total_elements != 0)
    {
        _standard_dev = sqrtf(((float)1/(float)_total_elements)*_sum);
    }
    else
    {
        _standard_dev = 0;
    }

    return _standard_dev;
}

