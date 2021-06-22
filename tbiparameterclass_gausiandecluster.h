#ifndef TBIPARAMETERCLASS_GAUSIANDECLUSTER_H
#define TBIPARAMETERCLASS_GAUSIANDECLUSTER_H

#include "tbicore_constants.h"
#include <cstdint>

class TBIGausianDeclusteringParameters
{
public:
    TBIGausianDeclusteringParameters();
    int m_max_cluster_size;
    int m_min_cluster_size;
    int m_max_clusters_in_col;

private:



};

#endif // TBIPARAMETERCLASS_GAUSIANDECLUSTER_H
