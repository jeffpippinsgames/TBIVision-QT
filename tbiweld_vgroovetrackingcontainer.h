#ifndef TBIWELD_VGROOVETRACKINGCONTAINER_H
#define TBIWELD_VGROOVETRACKINGCONTAINER_H

#include <QObject>
#include "tbiclass_line.h"
#include "tbiclass_pointint.h"
#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/imgproc/imgproc.hpp"
#include "tbiweld_pipelineprocessingreturntype.h"

class TBIWeld_VGrooveTrackingContainer : public QObject
{
    Q_OBJECT
public:
    explicit TBIWeld_VGrooveTrackingContainer(QObject *parent = nullptr);
    void setTrackToPoints();
    void extractTrackingPointsFromGeometricEntities(TBILine &_left_tsl, TBILine &_right_tsl, TBILine &_left_bwl, TBILine &_right_bwl);
    void invalidateTrackToPoints();
    bool trackToPointsValid();
    bool trackingpointsValid();



private:
    TBIPoint_Int m_left_tracking_point;
    TBIPoint_Int m_right_tracking_point;
    TBIPoint_Int m_root_point;
    TBIPoint_Int m_joint_centroid;

    TBIPoint_Int m_left_track_to_point;
    TBIPoint_Int m_right_track_to_point;
    TBIPoint_Int m_root_track_to_point;
    TBIPoint_Int m_joint_track_to_centroid;

signals:

};

#endif // TBIWELD_VGROOVETRACKINGCONTAINER_H
