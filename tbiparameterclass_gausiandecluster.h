#ifndef TBIPARAMETERCLASS_GAUSIANDECLUSTER_H
#define TBIPARAMETERCLASS_GAUSIANDECLUSTER_H

#include "tbicore_constants.h"
#include <cstdint>
#include <QObject>


class TBIGausianDeclusteringParameters
{
public:
    TBIGausianDeclusteringParameters();
    int m_max_cluster_size;
    int m_min_cluster_size;
    int m_max_clusters_in_col;
    int m_blur_value;
    int m_threshold_min_value;
    int m_threshold_max_value;
    float m_max_decluster_distro_deviation;

    quint64 m_max_image_intensity;
    quint64 m_min_image_intensity;
    quint64 m_total_image_intensity;

    bool totalImageIntensityToHigh();
    bool totalImageInstenisyToLow();
    void clearTotalImageIntensity();
    void addToTotalImageIntensity(int _value);

private:


};

#endif // TBIPARAMETERCLASS_GAUSIANDECLUSTER_H
