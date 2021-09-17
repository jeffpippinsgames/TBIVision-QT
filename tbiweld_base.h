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
#include "tbiclass_opencvmatcontainer.h"
#include <QDataStream>



class TBIWeld_Base : public QObject
{
    Q_OBJECT
public:
    //Constructors and Destructors
    explicit TBIWeld_Base(QObject *parent = nullptr);
    ~TBIWeld_Base();

    //PipeLine Methods
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t doDeGausianClustering(TBIClass_OpenCVMatContainer &_mats);

    //Set Methods
    void setGausianDeclusterParameters(TBIGausianDeclusteringParameters &_gausian_decluster_param);

    //Utility Methods
    int getGausianDeclusterDataSetMaxIndex() {return m_gausian_decluster_ds->size()-1;}
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t getGausianDeClusterSubSet(TBIDataSet &_dst, int _startindex, int _endindex);
    TBIDataSet * getGausianDeclusterSetPointer(){return m_gausian_decluster_ds;}

    //Vgroove Specific Methods
    void drawVGrooveBreakPointIndex(TBIClass_OpenCVMatContainer &_mats, int _breakindex);
    int getVGrooveBreakIndex();
    int getDeClusterSetMaxIndex(){return m_gausian_decluster_ds->size() - 1;}
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t extractVGrooveJointDataSet(TBIDataSet &_dst, const TBILine &_lefttsl, const TBILine &_righttsl, const float _inlierdistancethreshold);
    TBIGausianDeclusteringParameters* getGausianDeclusterParametersPointer(){return &m_gausiandecluster_params;}
    void setGausianDeClusterParamDefaultValues(){m_gausiandecluster_params.setDefautValues();}
    void saveGausianDeClusterParamsToFile(QDataStream &_filedatastream);
    void loadGausianDeClusterParamsFromFile(QDataStream &_filedatastream);

private:
    static const bool m_showdebug = false;
        //Datasets
        TBIDataSet *m_gausian_decluster_ds;
        TBIDataSet *m_filtered_gausian_decluster_ds;

        //Distrobution Set
        TBIDataDistributionSet *m_gausian_decluster_distro;

        //GausianDeclustering Settings
        TBIGausianDeclusteringParameters m_gausiandecluster_params;
signals:

};

#endif // TBIWELD_BASE_H
