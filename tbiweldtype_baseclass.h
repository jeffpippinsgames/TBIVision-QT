#ifndef TBIWELDTYPE_BASECLASS_H
#define TBIWELDTYPE_BASECLASS_H

#include <QObject>
#include <QQuickItem>
#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/imgproc/imgproc.hpp"
#include "tbiclass_dataset.h"

class tbiweldtype_baseclass : public QObject
{
    Q_OBJECT
public:
    explicit tbiweldtype_baseclass(QObject *parent = nullptr);
    void doDeGausianClustering(const cv::Mat _incommingmat);


private:

        TBIDataSet *m_gausian_decluster_ds;
        TBIDataSet *m_filtered_gausian_decluster_ds;
        TBIDataDistributionSet *m_gausian_decluster_distro;

        cv::Mat m_raw_mat;
        cv::Mat m_blurr_mat;
        cv::Mat m_threshold_mat;
        cv::Mat m_gausiandecluster_mat;
        cv::Mat m_inliers_mat;
        cv::Mat m_ransac_mat;
        cv::Mat m_geometricconstruction_mat;
        cv::Mat m_operations_mat;

signals:

};

#endif // TBIWELDTYPE_BASECLASS_H
