#ifndef PCOLUMN_H
#define PCOLUMN_H

#include <QObject>
#include "pcluster.h"
#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/imgproc/imgproc.hpp"

class PColumn : public QObject
{
    Q_OBJECT

public:
    explicit PColumn(QObject *parent = nullptr);
    void BuildPColumn(cv::Mat& _src, int _columnnumber);
    void emptyColumn(){m_pcolumn.empty();}
    void addCluster(PCluster &_cluster);
    int getClusterCount(){return m_pcolumn.count();}

    void removeNonGausianClusters();
    void removeClustersWithCountLessThan(int _count);
    void removeClustersWithCountGreaterThan(int _count);

signals:

private:
    QList<PCluster> m_pcolumn;
};

#endif // PCOLUMN_H
