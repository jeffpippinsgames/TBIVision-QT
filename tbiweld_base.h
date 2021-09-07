#ifndef TBIWELD_BASE_H
#define TBIWELD_BASE_H

#include <QObject>
#include <QQuickItem>
#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/imgproc/imgproc.hpp"
#include "tbicore_constants.h"
#include "tbiclass_dataset.h"
#include "tbiweld_pipelineprocessingreturntype.h"
#include "tbiparameterclass_gausiandecluster.h"


class TBIWeld_Base : public QObject
{
    Q_OBJECT
public:
    //Constructors and Destructors
    explicit TBIWeld_Base(QObject *parent = nullptr);
    ~TBIWeld_Base();

    //Set Methods
    void setGausianDeclusterParameters(TBIGausianDeclusteringParameters &_gausian_decluster_param);

    //Utility Methods
    int getVGrooveBreakIndex();
    void drawVGrooveBreakPointIndex(cv::Mat &_ransacmat, int _breakindex);
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t getGausianDeClusterSubSet(TBIDataSet &_dst, int _startindex, int _endindex);
    int getGausianDeclusterDataSetMaxIndex() {return m_gausian_decluster_ds->size()-1;}


private:
        //Private Seam Tracking Methods
        TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t doDeGausianClustering(cv::Mat &_rawmat, cv::Mat &_blurmat, cv::Mat &_thresholdmat, cv::Mat &_declustermat);

        //Datasets
        TBIDataSet *m_gausian_decluster_ds;
        TBIDataSet *m_filtered_gausian_decluster_ds;

        //Distrobution Set
        TBIDataDistributionSet *m_gausian_decluster_distro;

        //GausianDeclustering Settings
        TBIGausianDeclusteringParameters m_gausiandecluster_settings;
signals:

};

#endif // TBIWELD_BASE_H
