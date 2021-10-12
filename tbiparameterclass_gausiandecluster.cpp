#include "tbiparameterclass_gausiandecluster.h"
#include <QDebug>



TBIGausianDeclusteringParameters::TBIGausianDeclusteringParameters(QObject *parent)
{
    this->setDefautValues();
}

void TBIGausianDeclusteringParameters::setMaxClusterSize(int _size)
{
    m_max_cluster_size = _size;
    emit maxClusterSizeChanged();
}

void TBIGausianDeclusteringParameters::setMinClusterSize(int _size)
{
    m_min_cluster_size = _size;
    emit minClusterSizeChanged();
}

void TBIGausianDeclusteringParameters::setMaxClustersInColumn(int _value)
{
    m_max_clusters_in_col = _value;
    emit maxClustersInColumnChanged();
}

void TBIGausianDeclusteringParameters::setPreBlurValue(int _value)
{
    m_pre_blur_value = _value;
    emit preBlurValueChanged();
}

void TBIGausianDeclusteringParameters::setPostBlurValue(int _value)
{
    m_post_blur_value = _value;
    emit postBlurValueChanged();
}

void TBIGausianDeclusteringParameters::setMinThresholdValue(int _value)
{
    m_threshold_min_value = _value;
    emit minThresholdValueChanged();
}

void TBIGausianDeclusteringParameters::setMaxThresholdValue(int _value)
{
    m_threshold_max_value = _value;
    emit maxThresholdValueChanged();
}

void TBIGausianDeclusteringParameters::setDeclusterDeviation(float _value)
{
    m_max_decluster_distro_deviation = _value;
    emit deClusterDeviationChanged();
}

void TBIGausianDeclusteringParameters::setMaxImageIntensity(quint64 _value)
{
    m_max_image_intensity = _value;
    emit maxImageIntensityChanged();
}

void TBIGausianDeclusteringParameters::setMinImageIntensity(quint64 _value)
{
    m_min_image_intensity = _value;
    emit minImageIntensityChanged();
}

void TBIGausianDeclusteringParameters::setTotalImageIntensity(quint64 _value)
{
    m_total_image_intensity = _value;
    emit totalImageIntensityChanged();
}

void TBIGausianDeclusteringParameters::setErodeIterations(int _value)
{
    m_erode_iterations = _value;
    emit erodeIterationsChanged();
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
    this->setTotalImageIntensity(0);
}

void TBIGausianDeclusteringParameters::addToTotalImageIntensity(int _value)
{
    this->setTotalImageIntensity(m_total_image_intensity + _value);
}

void TBIGausianDeclusteringParameters::setDefautValues()
{
    this->setMaxClustersInColumn(2);
    this->setMinClusterSize(1);
    this->setMaxClusterSize(75);
    this->setPreBlurValue(3);
    this->setPostBlurValue(3);
    this->setMinThresholdValue(100);
    this->setMaxThresholdValue(255);
    this->setMinImageIntensity(1);
    this->setMaxImageIntensity(2000000);
    this->setTotalImageIntensity(0);
    this->setDeclusterDeviation(15.0);
    this->setErodeIterations(4);
}

void TBIGausianDeclusteringParameters::saveToFile(QDataStream &_filedatastream)
{

    _filedatastream << m_min_cluster_size;
    _filedatastream << m_max_cluster_size;
    _filedatastream << m_max_clusters_in_col;
    _filedatastream << m_pre_blur_value;
    _filedatastream << m_post_blur_value;
    _filedatastream << m_erode_iterations;
    _filedatastream << m_threshold_min_value;
    _filedatastream << m_threshold_max_value;
    _filedatastream << m_max_decluster_distro_deviation;
    _filedatastream << m_max_image_intensity;
    _filedatastream << m_min_image_intensity;

}

void TBIGausianDeclusteringParameters::loadFromFile(QDataStream &_filedatastream)
{
    _filedatastream >> m_min_cluster_size;
    _filedatastream >> m_max_cluster_size;
    _filedatastream >> m_max_clusters_in_col;
    _filedatastream >> m_pre_blur_value;
    _filedatastream >> m_post_blur_value;
    _filedatastream >> m_erode_iterations;
    _filedatastream >> m_threshold_min_value;
    _filedatastream >> m_threshold_max_value;
    _filedatastream >> m_max_decluster_distro_deviation;
    _filedatastream >> m_max_image_intensity;
    _filedatastream >> m_min_image_intensity;

    emit minClusterSizeChanged();
    emit maxClusterSizeChanged();
    emit maxClustersInColumnChanged();
    emit preBlurValueChanged();
    emit postBlurValueChanged();
    emit erodeIterationsChanged();
    emit minThresholdValueChanged();
    emit maxThresholdValueChanged();
    emit deClusterDeviationChanged();
    emit maxImageIntensityChanged();
    emit minImageIntensityChanged();
}


