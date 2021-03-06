#ifndef TBIWELD_BUTTJOINTTRACKINGCONTAINER_H
#define TBIWELD_BUTTJOINTTRACKINGCONTAINER_H

#include <QObject>
#include "tbiclass_line.h"
#include "tbiclass_pointint.h"
#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/imgproc/imgproc.hpp"
#include "tbiweld_pipelineprocessingreturntype.h"
#include "tbiparameterclass_motioncontrolparams.h"
#include "tbicore_gary.h"
#include "tbiclass_opencvmatcontainer.h"
#include <QString>


class TBIWeld_ButtJointTrackingContainer : public QObject
{
    Q_OBJECT
public:
    explicit TBIWeld_ButtJointTrackingContainer(QObject *parent = nullptr);

    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t extractTrackingPoint(TBILine &_tsl, TBIClass_OpenCVMatContainer &_mats);
    void setTrackToPoints();
    void drawTrackToPointstoMat(TBIClass_OpenCVMatContainer &_mats);
    int getZMovement(TBIMotionControlParameters &_motioncontrolparams);
    int getZTrackingDifference();
    QString getTrackingPointString(){return QString::number(m_tracking_point.m_x) + "," + QString::number(m_tracking_point.m_y);}

signals:

private:
    const bool m_showdebug = false;
    TBIPoint_Int m_tracking_point;
    TBIPoint_Int m_track_to_point;

    int m_tracking_boundary = 5;

    const cv::Scalar m_tracking_point_cv_color = CV_RGB(0,200,255);
    const cv::Scalar m_tracking_to_point_cv_color = CV_RGB(75,75,255);

};

#endif // TBIWELD_BUTTJOINTTRACKINGCONTAINER_H
