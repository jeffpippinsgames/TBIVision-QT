#include "tbiparameterclass_gausiandecluster.h"



TBIGausianDeclusteringParameters::TBIGausianDeclusteringParameters()
{
    m_max_clusters_in_col = 0;
    m_min_cluster_size = 0;
    m_max_clusters_in_col = 0;
    m_blur_value = 1;
    m_threshold_min_value = 5;
    m_threshold_max_value = 15;
    m_min_image_intensity = 1;
    m_max_image_intensity = 2000000;
    m_total_image_intensity = 0;
    m_max_decluster_distro_deviation = 15.0;
}

bool TBIGausianDeclusteringParameters::totalImageIntensityToHigh()
{
    if(m_total_image_intensity > m_max_image_intensity) return true;
    return false;
}

bool TBIGausianDeclusteringParameters::totalImageInstenisyToLow()
{
    if(m_total_image_intensity < m_min_image_intensity) return true;
    return false;
}

void TBIGausianDeclusteringParameters::clearTotalImageIntensity()
{
    m_total_image_intensity = 0;
}

void TBIGausianDeclusteringParameters::addToTotalImageIntensity(int _value)
{
    m_total_image_intensity += _value;
}


