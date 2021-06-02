#ifndef PIXELCOLUMNCLASS_H
#define PIXELCOLUMNCLASS_H

#include <QObject>
#include "pixelclusterclass.h"
#include "pixelfundamental.h"
#include "opencv4/opencv2/core.hpp"
#include <vector>
#include "pixelcolumnprocessreturn_t.h"

class PixelColumnClass
{
public:
    PixelColumnClass();
    int size();
    void clear();
    void pushClusterToBack(PixelClusterClass _cluster);
    float getCentroidofCluster(int _clusterindex);
    void drawToMat(cv::Mat &_dst);
    PixelColumnProcessingReturn_t  pixelProccessColumn(cv::Mat &_src, uint _column_in_mat_to_process, quint64 *_tii,
                             quint64 _max_tii, int _min_cluster_size, int _max_cluster_size, int _max_clusters_in_column);
    void processForContinuity(float _targetrowcentroid, float _distancethreshold);
    int getIndexOfClosestCentroid(float _targetcentroid, float _distancethreshold);



private:
    std::vector<PixelClusterClass> m_clusters;
};

#endif // PIXELCOLUMNCLASS_H
