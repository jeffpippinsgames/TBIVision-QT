#include "tbiweld_vgroove.h"
#include "tbiclass_ransac.h"

TBIWeld_VGroove::TBIWeld_VGroove()
{
    m_left_inlier_tsl_ds = new TBIDataSet();
    m_left_inlier_bwl_ds = new TBIDataSet();
    m_right_inlier_tsl_ds = new TBIDataSet();
    m_right_inlier_bwl_ds = new TBIDataSet();
    m_gausiandecluster_leftside_ds = new TBIDataSet();
    m_gausiandecluster_rightside_ds = new TBIDataSet();
    m_joint_ds = new TBIDataSet();
    m_left_joint_ds = new TBIDataSet();
    m_right_joint_ds = new TBIDataSet();
    m_dummy_set1 = new TBIDataSet();
    m_dummy_set2 = new TBIDataSet();
    this->setDefautValues();
}

TBIWeld_VGroove::~TBIWeld_VGroove()
{
    delete m_left_inlier_tsl_ds;
    delete  m_left_inlier_bwl_ds;
    delete m_right_inlier_tsl_ds;
    delete m_right_inlier_bwl_ds;
    delete m_gausiandecluster_leftside_ds;
    delete m_gausiandecluster_rightside_ds;
    delete m_left_joint_ds;
    delete m_right_joint_ds;
    delete m_joint_ds;
    delete m_dummy_set1;
    delete m_dummy_set2;
}

void TBIWeld_VGroove::setRootQMLContextProperties(QQmlApplicationEngine &_engine)
{
    _engine.rootContext()->setContextProperty("VGrooveLeftTSLRansacParameters", &m_left_tsl_ransac_params);
    _engine.rootContext()->setContextProperty("VGrooveRightTSLRansacParameters", &m_right_tsl_ransac_params);
    _engine.rootContext()->setContextProperty("VGrooveLeftBWLRansacParameters", &m_left_bwl_ransac_params);
    _engine.rootContext()->setContextProperty("VGrooveRightBWLRansacParameters", &m_right_bwl_ransac_params);
    _engine.rootContext()->setContextProperty("VGrooveGausianDistroParameters", this->getGausianDeclusterParametersPointer());
}

TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t TBIWeld_VGroove::processPipeline(TBIClass_OpenCVMatContainer &_mats, TBIWeld_VGrooveTrackingContainer &_vgroove_tracking_container)
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

    //Stage4 Find Tracking Points.
    _result = _vgroove_tracking_container.extractTrackingPointsFromGeometricEntities(m_left_tsl_geo_line, m_right_tsl_geo_line, m_left_bwl_geo_line, m_right_bwl_geo_line);
    if(_result != TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
    {
        return _result;
    }



    return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK;
}

void TBIWeld_VGroove::setDefautValues()
{
    m_left_tsl_ransac_params.setDefautValues();
    m_left_bwl_ransac_params.setDefautValues();
    m_right_tsl_ransac_params.setDefautValues();
    m_right_bwl_ransac_params.setDefautValues();
    this->setGausianDeClusterParamDefaultValues();
}

void TBIWeld_VGroove::saveToFile(QDataStream &_filedatastream)
{
    this->saveGausianDeClusterParamsToFile(_filedatastream);
    m_left_tsl_ransac_params.saveToFile(_filedatastream);
    m_left_bwl_ransac_params.saveToFile(_filedatastream);
    m_right_tsl_ransac_params.saveToFile(_filedatastream);
    m_right_bwl_ransac_params.saveToFile(_filedatastream);
}

void TBIWeld_VGroove::loadFromFile(QDataStream &_filedatastream)
{
    this->loadGausianDeClusterParamsFromFile(_filedatastream);
    m_left_tsl_ransac_params.loadFromFile(_filedatastream);
    m_left_bwl_ransac_params.loadFromFile(_filedatastream);
    m_right_tsl_ransac_params.loadFromFile(_filedatastream);
    m_right_bwl_ransac_params.loadFromFile(_filedatastream);
}

TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t TBIWeld_VGroove::doConstructInlierRansacs(TBIClass_OpenCVMatContainer &_mats)
{

    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Get The Break Index So the Distribution Set can be Split into two parts Left and right.
    //This helps to determine the Ransacs.
    m_break_index = this->getVGrooveBreakIndex();
    if(m_break_index == -1)
    {
        if(m_showdebug) qDebug("TBIWeld_VGroove::doConstructInlierRansacs() A Valid Break Index Was Not Found.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_COULDNOTFINDVALIDBREAKINDEXFORVGROOVEDATASET;
    }
    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Build The Left Side Gausian DeCluster Set
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t _result;
    _result = this->getGausianDeClusterSubSet(*m_gausiandecluster_leftside_ds, 0, m_break_index);
    if(_result != TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
    {
        if(m_showdebug) qDebug("TBIWeld_VGroove::doConstructInlierRansacs() Could Not Build Left Gausian Decluster Data SubSet.");
        return _result;
    }
    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Build The Right Side DeCluser SubSet
    _result = this->getGausianDeClusterSubSet(*m_gausiandecluster_rightside_ds, m_break_index, this->getGausianDeclusterDataSetMaxIndex());
    if(_result != TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
    {
        if(m_showdebug) qDebug("TBIWeld_VGroove::doConstructInlierRansacs() Could Not Build Right Gausian Decluster Data SubSet.");
        return _result;
    }
    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Build The Left TSL Ransac Line.
    if(!TBIRansac::doRansac(m_left_tsl_ransac_line, m_left_tsl_ransac_params, *m_gausiandecluster_leftside_ds))
    {
        if(m_showdebug) qDebug("TBIWeld_VGroove::doConstructInlierRansacs() Left TSL Ransac Is Invalid.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDUNABLETOFINDINLIERRANSACLINE;
    }
    //Draw The Left TSL Ransac
    m_left_tsl_ransac_line.remakeLine(0, _mats.m_ransac.cols);
    m_left_tsl_ransac_line.drawOnMat(_mats.m_ransac, m_left_tsl_cv_color);
    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Build The Right TSL Ransac Line
    if(!TBIRansac::doRansac(m_right_tsl_ransac_line, m_right_tsl_ransac_params, *m_gausiandecluster_rightside_ds))
    {
        if(m_showdebug) qDebug("TBIWeld_VGroove::doConstructInlierRansacs() Right TSL Ransac Is Invalid.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDUNABLETOFINDINLIERRANSACLINE;
    }
    //Draw The Right TSL Ransac
    m_right_tsl_ransac_line.remakeLine(0, _mats.m_ransac.cols);
    m_right_tsl_ransac_line.drawOnMat(_mats.m_ransac, m_right_tsl_cv_color);
    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Build Joint DataSet to Make Finding the BWL Ransacs Easier
    _result = this->extractVGrooveJointDataSet(*m_joint_ds, m_left_tsl_ransac_line, m_right_tsl_ransac_line, 1.5);
    if(_result != TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
    {
        if(m_showdebug) qDebug("TBIWeld_VGroove::doConstructInlierRansacs() Could Not Build VGroove Joint DataSet.");
        return _result;
    }

    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Adjust the BWL Ransac Params So That The Angle to Horizon is Adjusted To The Top Surface Lines
    TBIRansacParameter _adjusted_left_bwl_param;
    TBIRansacParameter _adjusted_right_bwl_param;
    _adjusted_left_bwl_param.makeEqual(m_left_bwl_ransac_params);
    _adjusted_right_bwl_param.makeEqual(m_right_bwl_ransac_params);
    _adjusted_left_bwl_param.setIdealAngle(m_left_bwl_ransac_params.getIdealAngle() - m_left_tsl_ransac_line.angleFromHorizontal());
    _adjusted_right_bwl_param.setIdealAngle(m_right_bwl_ransac_params.getIdealAngle() - m_right_tsl_ransac_line.angleFromHorizontal());
    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Find Left BWL Ransac Line
    if(!TBIRansac::doRansac(m_left_bwl_ransac_line, _adjusted_left_bwl_param, *m_joint_ds))
    {
        if(m_showdebug) qDebug("TBIWeld_VGroove::doConstructInlierRansacs() Left BWL Ransac Is Invalid.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDUNABLETOFINDINLIERRANSACLINE;
    }
    //Draw Ransac to Mat
    m_left_bwl_ransac_line.remakeLine(0, _mats.m_ransac.cols);
    m_left_bwl_ransac_line.drawOnMat(_mats.m_ransac, m_left_bwl_cv_color);
    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Find Right BWL Ransac Line
    if(!TBIRansac::doRansac(m_right_bwl_ransac_line, _adjusted_right_bwl_param, *m_joint_ds))
    {
        if(m_showdebug) qDebug("TBIWeld_VGroove::doConstructInlierRansacs() Right BWL Ransac Is Invalid.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDUNABLETOFINDINLIERRANSACLINE;
    }
    //Draw Ransac to Mat
    m_right_bwl_ransac_line.remakeLine(0, _mats.m_ransac.cols);
    m_right_bwl_ransac_line.drawOnMat(_mats.m_ransac, m_right_bwl_cv_color);

    return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK;
}

TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t TBIWeld_VGroove::doBuildInlierDataSets(TBIClass_OpenCVMatContainer &_mats)
{
    TBIDataSetReturnType _datasetresult;

    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Build Left TSL DataSet.
    _datasetresult = m_gausiandecluster_leftside_ds->extractDataSetForInliers(*m_left_inlier_tsl_ds, m_left_tsl_ransac_line, 2.0);
    if(_datasetresult != TBIDataSetReturnType::Ok)
    {
        if(m_showdebug) qDebug("TBIWeld_VGroove::doBuildInlierDataSets() Failed To Extract Left TSL Inlier Data Set.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOEXTRACTLEFTTSLINLIERDATASET;
    }
    //Draw Data Set into Inlier Mat
    m_left_inlier_tsl_ds->drawToMat(_mats.m_inliers, m_left_tsl_cv_color);

    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Build Right TSL DataSet
    _datasetresult = m_gausiandecluster_rightside_ds->extractDataSetForInliers(*m_right_inlier_tsl_ds, m_right_tsl_ransac_line, 2.0);
    if(_datasetresult != TBIDataSetReturnType::Ok)
    {
        if(m_showdebug) qDebug("TBIWeld_VGroove::doBuildInlierDataSets() Failed To Extract Right TSL Inlier Data Set.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOEXTRACTRIGHTTSLINLIERDATASET;
    }
    //Draw Data Set into Inlier Mat
    m_right_inlier_tsl_ds->drawToMat(_mats.m_inliers, m_right_tsl_cv_color);

    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Build Left BWL DataSet.
    _datasetresult = m_joint_ds->extractDataSetForInliers(*m_left_inlier_bwl_ds, m_left_bwl_ransac_line, 2.0);
    if(_datasetresult != TBIDataSetReturnType::Ok)
    {
        if(m_showdebug) qDebug("TBIWeld_VGroove::doBuildInlierDataSets() Failed To Extract Left BWL Inlier Data Set.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOEXTRACTLEFTBWLINLIERDATASET;
    }
    //Draw Data Set into Inlier Mat
    m_left_inlier_bwl_ds->drawToMat(_mats.m_inliers, m_left_bwl_cv_color);

    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Build Right BWL DataSet
    _datasetresult = m_joint_ds->extractDataSetForInliers(*m_right_inlier_bwl_ds, m_right_bwl_ransac_line, 2.0);
    if(_datasetresult != TBIDataSetReturnType::Ok)
    {
        if(m_showdebug) qDebug("TBIWeld_VGroove::doBuildInlierDataSets() Failed To Extract Right BWL Inlier Data Set.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOEXTRACTRIGHTBWLINLIERDATASET;
    }
    //Draw Data Set into Inlier Mat
    m_right_inlier_bwl_ds->drawToMat(_mats.m_inliers, m_right_bwl_cv_color);


    return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK;
}

TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t TBIWeld_VGroove::doConstructGeometricEntities(TBIClass_OpenCVMatContainer &_mats)
{
    TBIDataSetReturnType _datasetresult;

    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Build Left TSL Geometric Entity
    _datasetresult = m_left_inlier_tsl_ds->extractLeastSquareLine(m_left_tsl_geo_line);
    if(_datasetresult != TBIDataSetReturnType::Ok)
    {
        if(m_showdebug) qDebug("TBIWeld_VGroove::doConstructGeometricEntities() Failed To Build Left TSL Geometric Entity.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOBUILDLEFTTSLGEOMETRICENTITY;
    }
    //Draw Geometric Entity
    m_left_tsl_geo_line.drawOnMat(_mats.m_geometricconstruction, m_left_tsl_cv_color);

    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Build Right TSL Geometric Entity
    _datasetresult = m_right_inlier_tsl_ds->extractLeastSquareLine(m_right_tsl_geo_line);
    if(_datasetresult != TBIDataSetReturnType::Ok)
    {
        if(m_showdebug) qDebug("TBIWeld_VGroove::doConstructGeometricEntities() Failed To Build Right TSL Geometric Entity.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOBUILDRIGHTTSLGEOMETRICENTITY;
    }
    //Draw Geometric Entity
    m_right_tsl_geo_line.drawOnMat(_mats.m_geometricconstruction, m_right_tsl_cv_color);

    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Build Left BWL Geometric Entity
    _datasetresult = m_left_inlier_bwl_ds->extractLeastSquareLine(m_left_bwl_geo_line);
    if(_datasetresult != TBIDataSetReturnType::Ok)
    {
        if(m_showdebug) qDebug("TBIWeld_VGroove::doConstructGeometricEntities() Failed To Build Left BWL Geometric Entity.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOBUILDLEFTBWLGEOMETRICENTITY;
    }
    //Draw Geometric Entity
    m_left_bwl_geo_line.drawOnMat(_mats.m_geometricconstruction, m_left_bwl_cv_color);

    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    //Build Right BWL Geometric Entity
    _datasetresult = m_right_inlier_bwl_ds->extractLeastSquareLine(m_right_bwl_geo_line);
    if(_datasetresult != TBIDataSetReturnType::Ok)
    {
        if(m_showdebug) qDebug("TBIWeld_VGroove::doConstructGeometricEntities() Failed To Build Right BWL Geometric Entity.");
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOBUILDRIGHTBWLGEOMETRICENTITY;
    }
    //Draw Geometric Entity
    m_right_bwl_geo_line.drawOnMat(_mats.m_geometricconstruction, m_right_bwl_cv_color);


    return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK;
}

void TBIWeld_VGroove::clearDataSets()
{
    m_left_inlier_tsl_ds->clear();
    m_left_inlier_bwl_ds->clear();
    m_right_inlier_tsl_ds->clear();
    m_right_inlier_bwl_ds->clear();
    m_gausiandecluster_leftside_ds->clear();
    m_gausiandecluster_rightside_ds->clear();
    m_joint_ds->clear();
    m_left_joint_ds->clear();
    m_right_joint_ds->clear();
    m_dummy_set1->clear();
    m_dummy_set2->clear();
}

void TBIWeld_VGroove::clearRansacLines()
{
    m_left_bwl_ransac_line.clear();
    m_left_tsl_ransac_line.clear();
    m_right_tsl_ransac_line.clear();
    m_right_bwl_ransac_line.clear();
}

void TBIWeld_VGroove::clearGeometricEntities()
{
    m_left_tsl_geo_line.clear();
    m_left_bwl_geo_line.clear();
    m_right_tsl_geo_line.clear();
    m_right_bwl_geo_line.clear();
}

