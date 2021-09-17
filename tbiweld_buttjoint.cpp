#include "tbiweld_buttjoint.h"

TBIWeld_ButtJoint::TBIWeld_ButtJoint()
{
    m_tsl_inlier_ds = new TBIDataSet();
    this->setDefautValues();
}

TBIWeld_ButtJoint::~TBIWeld_ButtJoint()
{
    delete m_tsl_inlier_ds;
}

TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t TBIWeld_ButtJoint::processPipeline(TBIClass_OpenCVMatContainer &_mats, TBIWeld_ButtJointTrackingContainer &_tracking_container)
{

    this->clearDataSets();
    this->clearRansacLines();
    this->clearGeometricEntities();

    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t _result;

    //Stage 1 Build Gausian DeCluster DataSet.
    _result = this->doDeGausianClustering(_mats);
    if(_result != TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
    {
        return _result;
    }

    //Stage 2a Build Inlier Ransacs
    _result = this->doConstructInlierRansacs(_mats);
    if(_result != TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
    {
        return _result;
    }

    //Stage 2b Build Inlier DataSets
    _result = this->doBuildInlierDataSets(_mats);
    if(_result != TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
    {
        return _result;
    }

    //Stage3 Build Geometric Entities
    _result = this->doConstructGeometricEntities(_mats);
    if(_result != TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
    {
        return _result;
    }


    _result = _tracking_container.extractTrackingPoint(m_tsl_geo_line, _mats);
    if(_result != TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
    {
        return _result;
    }
    cv::putText(_mats.m_operation, _tracking_container.getTrackingPointString().toStdString(), cv::Point(20, 100), cv::FONT_HERSHEY_SIMPLEX, 1, m_left_tsl_cv_color, 1, cv::LINE_AA);
    return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK;
}

void TBIWeld_ButtJoint::setRootQMLContextProperties(QQmlApplicationEngine &_engine)
{
    _engine.rootContext()->setContextProperty("ButtJointRansacParameters", &m_tsl_ransac_params);
    _engine.rootContext()->setContextProperty("ButtJointGausianDistroParameters", this->getGausianDeclusterParametersPointer());
}

void TBIWeld_ButtJoint::setDefautValues()
{
    m_tsl_ransac_params.setDefautValues();
    this->setGausianDeClusterParamDefaultValues();
}

void TBIWeld_ButtJoint::saveToFile(QDataStream &_filedatastream)
{
    this->saveGausianDeClusterParamsToFile(_filedatastream);
    m_tsl_ransac_params.saveToFile(_filedatastream);

}

void TBIWeld_ButtJoint::loadFromFile(QDataStream &_filedatastream)
{
    this->loadGausianDeClusterParamsFromFile(_filedatastream);
    m_tsl_ransac_params.loadFromFile(_filedatastream);
}

void TBIWeld_ButtJoint::clearDataSets()
{
    m_tsl_inlier_ds->clear();
}

void TBIWeld_ButtJoint::clearRansacLines()
{
    m_tsl_ransac_line.clear();
}

void TBIWeld_ButtJoint::clearGeometricEntities()
{
    m_tsl_geo_line.clear();
}

TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t TBIWeld_ButtJoint::doConstructGeometricEntities(TBIClass_OpenCVMatContainer &_mats)
{
    TBIDataSetReturnType _datasetresult;

    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Build Left TSL Geometric Entity
    _datasetresult = m_tsl_inlier_ds->extractLeastSquareLine(m_tsl_geo_line);
    if(_datasetresult != TBIDataSetReturnType::Ok)
    {
        if(m_showdebug) qDebug("TBIWeld_ButtJoint::doConstructGeometricEntities() Failed To Build Left TSL Geometric Entity.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOBUILDLEFTTSLGEOMETRICENTITY;
    }
    //Draw Geometric Entity
    m_tsl_geo_line.drawOnMat(_mats.m_geometricconstruction, m_left_tsl_cv_color);
    return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK;
}

TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t TBIWeld_ButtJoint::doConstructInlierRansacs(TBIClass_OpenCVMatContainer &_mats)
{
    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Copy The Gausian DeCluster Set to the Ransac Mat
    cv::cvtColor(_mats.m_gausiandecluster, _mats.m_ransac, cv::COLOR_GRAY2BGR);

    //Build The Left TSL Ransac Line.
    if(!TBIRansac::doRansac(m_tsl_ransac_line, m_tsl_ransac_params, *this->getGausianDeclusterSetPointer()))
    {
        if(m_showdebug) qDebug("TBIWeld_ButtJoint::doConstructInlierRansacs TSL Ransac Is Invalid.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDUNABLETOFINDINLIERRANSACLINE;
    }
    //Draw The Left TSL Ransac
    m_tsl_ransac_line.remakeLine(0, _mats.m_ransac.cols);
    m_tsl_ransac_line.drawOnMat(_mats.m_ransac, m_left_tsl_cv_color);

    return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK;
}

TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t TBIWeld_ButtJoint::doBuildInlierDataSets(TBIClass_OpenCVMatContainer &_mats)
{
    TBIDataSetReturnType _datasetresult;

    _datasetresult = this->getGausianDeclusterSetPointer()->extractDataSetForInliers(*m_tsl_inlier_ds, m_tsl_ransac_line, 10.0);
    if(_datasetresult != TBIDataSetReturnType::Ok)
    {
        if(m_showdebug) qDebug("TBIWeld_ButtJoint::doBuildInlierDataSets() Failed To Extract Left TSL Inlier Data Set.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOEXTRACTLEFTTSLINLIERDATASET;
    }
    m_tsl_inlier_ds->drawToMat(_mats.m_inliers, m_left_tsl_cv_color);
    return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK;
}


