#ifndef PCLUSTER_H
#define PCLUSTER_H

#include <QObject>
#include <QList>
#include "pfundamental.h"

class PCluster : public QObject
{
    Q_OBJECT
public:
    explicit PCluster(QObject *parent = nullptr);
    void empty();
    void addFundamental(PFundamental &_fundamental);
    float getIntensityCentroid();
    unsigned long getTotalClusterIntensity(){return m_totalclusterintensity;}
    int getClusterSize(){return m_cluster.size();}
    bool IsClusterGausian();


private:
    unsigned long m_totalclusterintensity;
    QList<PFundamental> m_cluster;


signals:


};

#endif // PCLUSTER_H
