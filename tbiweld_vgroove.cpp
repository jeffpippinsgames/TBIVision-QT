#include "tbiweld_vgroove.h"


using namespace std;

TBIWeld_VGroove::TBIWeld_VGroove()
{
    m_master_inlier_tsl_ds = new TBIDataSet();
    m_break_index_ds = new TBIDataSet();
    m_left_inlier_tsl_ds = new TBIDataSet();
    m_right_inlier_tsl_ds = new TBIDataSet();
    m_dummy_set1 = new TBIDataSet();
    m_dummy_set2 = new TBIDataSet();
    this->setDefautValues();
}

TBIWeld_VGroove::~TBIWeld_VGroove()
{
    delete m_master_inlier_tsl_ds;
    delete m_break_index_ds;
    delete m_left_inlier_tsl_ds;
    delete m_right_inlier_tsl_ds;
    delete m_dummy_set1;
    delete m_dummy_set2;
}

void TBIWeld_VGroove::setRootQMLContextProperties(QQmlApplicationEngine &_engine)
{
    _engine.rootContext()->setContextProperty("MasterTSLRansacParameters", &m_master_tsl_ransac_params);
    _engine.rootContext()->setContextProperty("VGrooveLeftTSLRansacParameters", &m_left_tsl_ransac_params);
    _engine.rootContext()->setContextProperty("VGrooveRightTSLRansacParameters", &m_right_tsl_ransac_params);
    _engine.rootContext()->setContextProperty("BreakIndexRansacParameters", &m_break_index_ransac_params);
    _engine.rootContext()->setContextProperty("VGrooveGausianDistroParameters", this->getGausianDeclusterParametersPointer());
}

TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t TBIWeld_VGroove::processPipeline(TBIClass_OpenCVMatContainer &_mats)
{

    this->clearDataSets();
    this->clearRansacLines();
    this->clearGeometricEntities();

    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t _result;
    TBIDataSetReturnType _dsresult;

    //Stage 1 Build Gausian DeCluster DataSet, Find Master Inlier Set.
    _result = this->doDeGausianClustering(_mats);
    if(_result != TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
    {
        return _result;
    }

    //Build Master TSL Line
    _result = this->buildRansacLine(this->getGausianDeclusterSetPointer(), m_master_tsl_ransac_line, m_master_tsl_ransac_params);
    if(_result != TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
    {
        return _result;
    }
    //Draw Master Ransac Line
    m_master_tsl_ransac_line.drawOnMat(_mats.m_ransac, m_master_tsl_cv_color);
    //Build Master Inlier DataSet
    _result = this->extractInlierDataSet(this->getGausianDeclusterSetPointer(), m_master_inlier_tsl_ds, m_master_tsl_ransac_line, m_master_tsl_ransac_params.getDistanceThreshold());
    if(_result != TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
    {
        return _result;
    }
    //Draw Master Inlier Dataset to Mat
    m_master_inlier_tsl_ds->drawToMat(_mats.m_inliers, m_master_tsl_cv_color);
    m_master_inlier_tsl_ds->drawToMat(_mats.m_operation, m_master_tsl_cv_color);

    //Stage 2 Determine Break Index
    //Find Break Index Ransac and DataSet
    _result = this->buildRansacLine(m_master_inlier_tsl_ds, m_break_index_ransac_line, m_break_index_ransac_params);
    if(_result != TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
    {
        return _result;
    }
    m_break_index_ransac_line.drawOnMat(_mats.m_ransac, m_break_index_cv_color);
    //Find BreakIndex Inlier Set
    _result = this->extractInlierDataSet(m_master_inlier_tsl_ds, m_break_index_ds, m_break_index_ransac_line, m_break_index_ransac_params.getDistanceThreshold());
    if(_result != TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
    {
        return _result;
    }
    //m_break_index_ds->drawToMat(_mats.m_inliers, m_left_tsl_cv_color);

    //Analyze The Break DataSet The Break Index.
    int _xvalbreak = m_break_index_ds->extractVGrooveBreakXValue(20);
    qDebug() << "_xvalbreak = " << _xvalbreak;
    if(_xvalbreak == -1)
    {
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILED_TO_EXTRACTVGROOVEBREAKINDEX;
    }
    //Find the Index of the Inlier DataSet
    int _breakindex = m_master_inlier_tsl_ds->getIndexofFirstXValue(_xvalbreak);
    qDebug() << "_breakindex = " << _breakindex;
    if(_breakindex == -1)
    {
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILED_TO_EXTRACTVGROOVEBREAKINDEX;
    }
    //Draw The BreakIndex Marker
    cv::drawMarker(_mats.m_ransac, m_master_inlier_tsl_ds->getPoint(_breakindex).toCVPoint(), m_break_index_cv_color, cv::MARKER_CROSS, 40);

    //Stage 3 Determine The Left Side Data Set.
    m_dummy_set1->clear();
    _dsresult = m_master_inlier_tsl_ds->extractDataSubSet(*m_dummy_set1, 0, _breakindex);
    if(_dsresult != TBIDataSetReturnType::Ok)
    {
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDTOEXTRACTLEFTTSLINLIERDATASET;
    }
    _result = this->buildRansacLine(m_dummy_set1, m_left_tsl_ransac_line, m_left_tsl_ransac_params);
    if(_result != TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
    {
        return _result;
    }
    m_left_tsl_ransac_line.drawOnMat(_mats.m_ransac, m_left_tsl_cv_color);
    _result = this->extractInlierDataSet(m_dummy_set1, m_left_inlier_tsl_ds, m_left_tsl_ransac_line, m_left_tsl_ransac_params.getDistanceThreshold());
    if(_result != TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK)
    {
        return _result;
    }
    m_left_inlier_tsl_ds->drawToMat(_mats.m_inliers, m_left_tsl_cv_color);








    return _result;

}

void TBIWeld_VGroove::setDefautValues()
{
    m_left_tsl_ransac_params.setDefautValues();
    m_right_tsl_ransac_params.setDefautValues();
    m_break_index_ransac_params.setDefautValues();
    this->setGausianDeClusterParamDefaultValues();
}

void TBIWeld_VGroove::saveToFile(QDataStream &_filedatastream)
{
    this->saveGausianDeClusterParamsToFile(_filedatastream);
    m_master_tsl_ransac_params.saveToFile(_filedatastream);
    m_left_tsl_ransac_params.saveToFile(_filedatastream);
    m_right_tsl_ransac_params.saveToFile(_filedatastream);
    m_break_index_ransac_params.saveToFile(_filedatastream);
}

void TBIWeld_VGroove::loadFromFile(QDataStream &_filedatastream)
{
    this->loadGausianDeClusterParamsFromFile(_filedatastream);
    m_master_tsl_ransac_params.loadFromFile(_filedatastream);
    m_left_tsl_ransac_params.loadFromFile(_filedatastream);
    m_right_tsl_ransac_params.loadFromFile(_filedatastream);
    m_break_index_ransac_params.loadFromFile(_filedatastream);
}

TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t TBIWeld_VGroove::buildRansacLine(TBIDataSet *_ds, TBILine &_line, TBIRansacParameter &_params)
{
    TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t _result = TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK;
    if(!TBIRansac::doRansac(_line, _params, *_ds)) _result = TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILEDUNABLETOFINDINLIERRANSACLINE;
    _line.remakeLine(0, TBIConstants::Max_Camera_Width);
    return _result;
}

TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t TBIWeld_VGroove::extractInlierDataSet(TBIDataSet *_src, TBIDataSet *_dst, TBILine &_ransac, float _distthreshold)
{
    if(_src->size() < 10) return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILED_TO_EXTRACTINLIERDATASET;
    if(!_ransac.isValid()) return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILED_TO_EXTRACTINLIERDATASET;
    if(_distthreshold < 0) return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILED_TO_EXTRACTINLIERDATASET;
    if(_dst->size() != 0 ) return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILED_TO_EXTRACTINLIERDATASET;

    if(_src->extractDataSetForInliers(*_dst, _ransac, _distthreshold) != TBIDataSetReturnType::Ok) return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILED_TO_EXTRACTINLIERDATASET;

    return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK;

}

TBIWeld_ProcessingPipeLineReturnType::PipelineReturnType_t TBIWeld_VGroove::extractOutlierDataSet(TBIDataSet *_srcds, TBIDataSet *_inlierds, TBIDataSet *_outlierds)
{
  TBIDataSetReturnType _dsresult;

    _dsresult = _srcds->extractOutlierDataSet(*_inlierds, *_outlierds);
    if(_dsresult != TBIDataSetReturnType::Ok)
    {
        return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_FAILED_TO_EXTRACTOUTLIERDATASET;
    }
    return TBIWeld_ProcessingPipeLineReturnType::TBI_PIPELINE_OK;
}

void TBIWeld_VGroove::clearDataSets()
{
    m_master_inlier_tsl_ds->clear();
    m_left_inlier_tsl_ds->clear();
    m_right_inlier_tsl_ds->clear();
    m_break_index_ds->clear();
    m_dummy_set1->clear();
    m_dummy_set2->clear();
}

void TBIWeld_VGroove::clearRansacLines()
{
    m_master_tsl_ransac_line.clear();
    m_left_tsl_ransac_line.clear();
    m_right_tsl_ransac_line.clear();
    m_break_index_ransac_line.clear();
}

void TBIWeld_VGroove::clearGeometricEntities()
{
    m_left_tsl_geo_line.clear();
    m_right_tsl_geo_line.clear();
}

