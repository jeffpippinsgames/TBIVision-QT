#ifndef TBIPARAMETERCLASS_GAUSIANDECLUSTER_H
#define TBIPARAMETERCLASS_GAUSIANDECLUSTER_H

#include "tbicore_constants.h"
#include <cstdint>
#include <QObject>
#include <QDataStream>


class TBIGausianDeclusteringParameters: public QObject
{

    Q_OBJECT

    Q_PROPERTY(int maxClusterSize WRITE setMaxClusterSize READ getMaxClusterSize NOTIFY maxClusterSizeChanged)
    Q_PROPERTY(int minClusterSize WRITE setMinClusterSize READ getMinClusterSize NOTIFY minClusterSizeChanged)
    Q_PROPERTY(int maxClustersInColumn WRITE setMaxClustersInColumn READ getMaxClustersInColumn NOTIFY maxClustersInColumnChanged)
    Q_PROPERTY(int preBlurValue WRITE setPreBlurValue READ getPreBlurValue NOTIFY preBlurValueChanged)
    Q_PROPERTY(int postBlurValue WRITE setPostBlurValue READ getPostBlurValue NOTIFY postBlurValueChanged)
    Q_PROPERTY(int erodeIterations WRITE setErodeIterations READ getErodeIterations NOTIFY erodeIterationsChanged)
    Q_PROPERTY(int edgeMin WRITE setEdgeMin READ getEdgeMin NOTIFY edgeMinChanged)
    Q_PROPERTY(int edgeMax WRITE setEdgeMax READ getEdgeMax NOTIFY edgeMaxChanged)
    Q_PROPERTY(int minThresholdValue WRITE setMinThresholdValue READ getMinThresholdValue NOTIFY minThresholdValueChanged)
    Q_PROPERTY(int maxThresholdValue WRITE setMaxThresholdValue READ getMaxThresholdValue NOTIFY maxThresholdValueChanged)
    Q_PROPERTY(float declusterDistrobutionStandardDeviation WRITE setDeclusterDeviation READ getDeclusterDeviation NOTIFY deClusterDeviationChanged)
    Q_PROPERTY(quint64 maxImageIntensity WRITE setMaxImageIntensity READ getMaxImageIntensity NOTIFY maxImageIntensityChanged)
    Q_PROPERTY(quint64 minImageIntensity WRITE setMinImageIntensity READ getMinImageIntensity NOTIFY minImageIntensityChanged)
    Q_PROPERTY(quint64 totalImageIntensity WRITE setTotalImageIntensity READ getTotalImageIntensity NOTIFY totalImageIntensityChanged)

public:

    explicit TBIGausianDeclusteringParameters(QObject *parent = nullptr);

    //Set Methods
    void setMaxClusterSize(int _size);
    void setMinClusterSize(int _size);
    void setMaxClustersInColumn(int _value);
    void setPreBlurValue(int _value);
    void setPostBlurValue(int _value);
    void setMinThresholdValue(int _value);
    void setMaxThresholdValue(int _value);
    void setDeclusterDeviation(float _value);
    void setMaxImageIntensity(quint64 _value);
    void setMinImageIntensity(quint64 _value);
    void setTotalImageIntensity(quint64 _value);
    void setErodeIterations(int _value);
    void setEdgeMin(int _value);
    void setEdgeMax(int _value);

    //Get Methods
    int getMaxClusterSize(){return m_max_cluster_size;}
    int getMinClusterSize(){return m_min_cluster_size;}
    int getMaxClustersInColumn(){return m_max_clusters_in_col;}
    int getPreBlurValue(){return m_pre_blur_value;}
    int getPostBlurValue(){return m_post_blur_value;}
    int getMinThresholdValue(){return m_threshold_min_value;}
    int getMaxThresholdValue(){return m_threshold_max_value;}
    float getDeclusterDeviation(){return m_max_decluster_distro_deviation;}
    quint64 getMaxImageIntensity(){return m_max_image_intensity;}
    quint64 getMinImageIntensity(){return m_min_image_intensity;}
    quint64 getTotalImageIntensity(){return m_total_image_intensity;}
    int getErodeIterations(){return m_erode_iterations;}
    int getEdgeMin(){return m_edge_min;}
    int getEdgeMax(){return m_edge_max;}


    //Utility Methods
    bool totalImageIntensityToHigh();
    bool totalImageInstenisyToLow();
    void clearTotalImageIntensity();
    void addToTotalImageIntensity(int _value);

    //Save File Methods
    void setDefautValues();
    void saveToFile(QDataStream &_filedatastream);
    void loadFromFile(QDataStream &_filedatastream);

signals:
   void maxClusterSizeChanged();
   void minClusterSizeChanged();
   void maxClustersInColumnChanged();
   void preBlurValueChanged();
   void postBlurValueChanged();
   void minThresholdValueChanged();
   void maxThresholdValueChanged();
   void deClusterDeviationChanged();
   void maxImageIntensityChanged();
   void minImageIntensityChanged();
   void totalImageIntensityChanged();
   void erodeIterationsChanged();
   void edgeMinChanged();
   void edgeMaxChanged();

private:

   int m_max_cluster_size;
   int m_min_cluster_size;
   int m_max_clusters_in_col;
   int m_pre_blur_value;
   int m_threshold_min_value;
   int m_threshold_max_value;
   float m_max_decluster_distro_deviation;
   int m_num_of_erodes;
   int m_post_blur_value;
   int m_erode_iterations;
   int m_edge_min;
   int m_edge_max;


   quint64 m_max_image_intensity;
   quint64 m_min_image_intensity;
   quint64 m_total_image_intensity;

};

#endif // TBIPARAMETERCLASS_GAUSIANDECLUSTER_H
