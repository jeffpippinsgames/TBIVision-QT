#include "tbicore_mary.h"
#include <QDebug>
#include <QDataStream>


/**************************************************************
Mary(QObject *parent) : QObject(parent)
Public
Description:
  Mary Constructor
**************************************************************/
Mary::Mary(QObject *parent) : QObject(parent)
{

    qDebug() << "Mary::Mary() Mary Object Created.";

}

/**************************************************************
~Mary(QObject *parent) : QObject(parent)
Public
Description:
  Mary Destructor
**************************************************************/
Mary::~Mary()
{
    emit aboutToDestroy();
    qDebug() << "Mary::~Mary Mary Object Destroyed.";
}

/**************************************************************
SetMaryDefaultValues()
Public
Description:
  Sets The Default Values and Triggers the Changes.
**************************************************************/
void Mary::SetMaryDefaultValues()
{
    m_cv_blur = 3;
    m_cv_thresholdmax = 255;
    m_cv_thresholdmin = 0;
    //-----------------------------
    m_gui_showdebuginfo = true;
    //-----------------------------
    m_pylon_aoiheight = m_pylon_maxcameraheight;
    m_pylon_aoiwidth = m_pylon_maxcamerawidth; 
    m_pylon_exposuretime = 2500.0;
    m_pylon_gain = 0;
    //-----------------------------
    m_pc_max_tii = 2500000;
    m_pc_min_tii = 250000;
    m_pc_max_clustersize = 75;
    m_pc_min_clustersize = 10;
    m_pc_max_clustersincolumn = 1;
    //------------------------------
    m_sk_max_discontinuity = 20;
    //------------------------------
    m_left_tsl_ideal_angle = -10;
    m_left_tsl_allowed_angle_variance = 10;
    m_left_tsl_min_votes = 20;
    m_left_tsl_iterations = 50;
    m_left_tsl_distance_threshold = 1.0;

    m_right_tsl_ideal_angle = -10;
    m_right_tsl_allowed_angle_variance = 10;
    m_right_tsl_min_votes = 20;
    m_right_tsl_iterations = 50;
    m_right_tsl_distance_threshold = 1.0;

    m_left_bwl_ideal_angle = -60;
    m_left_bwl_allowed_angle_variance = -30;
    m_left_bwl_min_votes = 20;
    m_left_bwl_iterations = 50;
    m_left_bwl_distance_threshold = 1.0;

    m_right_bwl_ideal_angle = 30;
    m_right_bwl_allowed_angle_variance = 60;
    m_right_bwl_min_votes = 20;
    m_right_bwl_iterations = 50;
    m_right_bwl_distance_threshold = 1.0;
    //---------------------------------------
    m_split_distance = 8.0;
    m_split_length = 10.0;
}

/**************************************************************
UpdateCameraAOIToMarysSettings()
Signal
Description:
  Fires a the signalChangeCameraAOI signal
**************************************************************/
void Mary::updateCameraAOIToMarysSettings()
{
    emit signalChangeCameraAOI(m_pylon_aoiwidth, m_pylon_aoiheight);
}

/**************************************************************
saveMaryToFile()
Signal
Description:
  saves Mary To the default File.
**************************************************************/
void Mary::saveMaryToFile()
{
    QString _filepath = qApp->applicationDirPath() + "/savefile/marydefault.tbi";
    QFile _savefile(_filepath);
    _savefile.open(QIODevice::WriteOnly);
    QDataStream _ds(&_savefile);
    _ds << m_pylon_aoiwidth;
    _ds << m_pylon_aoiheight;
    _ds << m_pylon_exposuretime;
    _ds << m_pylon_gain;
    _ds << m_cv_blur;
    _ds << m_cv_thresholdmin;
    _ds << m_cv_thresholdmax;
    _ds << m_gui_showdebuginfo;
    _ds << m_pc_max_tii;
    _ds << m_pc_min_tii;
    _ds << m_pc_max_clustersize;
    _ds << m_pc_min_clustersize;
    _ds << m_pc_max_clustersincolumn;
    _ds << m_sk_max_discontinuity;
    _ds << m_left_tsl_ideal_angle;
    _ds << m_left_tsl_allowed_angle_variance;
    _ds << m_left_tsl_min_votes;
    _ds << m_left_tsl_iterations;
    _ds << m_left_tsl_distance_threshold;
    _ds << m_right_tsl_ideal_angle;
    _ds << m_right_tsl_allowed_angle_variance;
    _ds << m_right_tsl_min_votes;
    _ds << m_right_tsl_iterations;
    _ds << m_right_tsl_distance_threshold;
    _ds << m_left_bwl_ideal_angle;
    _ds << m_left_bwl_allowed_angle_variance;
    _ds << m_left_bwl_min_votes;
    _ds << m_left_bwl_iterations;
    _ds << m_left_bwl_distance_threshold;
    _ds << m_right_bwl_ideal_angle;
    _ds << m_right_bwl_allowed_angle_variance;
    _ds << m_right_bwl_min_votes;
    _ds << m_right_bwl_iterations;
    _ds << m_right_bwl_distance_threshold;
    _ds.setVersion(QDataStream::Qt_5_12);
    _savefile.close();
    qDebug() << "Mary::saveMaryToFile() Settings Saved To " << _filepath;



}

/**************************************************************
loadMaryFromFile()
Signal
Description:
  Loads Mary From The Default File
**************************************************************/
void Mary::loadMaryFromFile()
{
    QString _filepath = qApp->applicationDirPath() + "/savefile/marydefault.tbi";
    QFile _savefile(_filepath);
    if(!_savefile.exists())
    {
        SetMaryDefaultValues();
        qDebug("Mary::loadMaryFromFile() marydefualt.tbi Does Not Exsist. Loading Default Values.");
        return;
    }
    _savefile.open(QIODevice::ReadOnly);
    QDataStream _ds(&_savefile);
    _ds >> m_pylon_aoiwidth;
    _ds >> m_pylon_aoiheight;
    _ds >> m_pylon_exposuretime;
    _ds >> m_pylon_gain;
    _ds >> m_cv_blur;
    _ds >> m_cv_thresholdmin;
    _ds >> m_cv_thresholdmax;
    _ds >> m_gui_showdebuginfo;
    _ds >> m_pc_max_tii;
    _ds >> m_pc_min_tii;
    _ds >> m_pc_max_clustersize;
    _ds >> m_pc_min_clustersize;
    _ds >> m_pc_max_clustersincolumn;
    _ds >> m_sk_max_discontinuity;
    _ds >> m_left_tsl_ideal_angle;
    _ds >> m_left_tsl_allowed_angle_variance;
    _ds >> m_left_tsl_min_votes;
    _ds >> m_left_tsl_iterations;
    _ds >> m_left_tsl_distance_threshold;
    _ds >> m_right_tsl_ideal_angle;
    _ds >> m_right_tsl_allowed_angle_variance;
    _ds >> m_right_tsl_min_votes;
    _ds >> m_right_tsl_iterations;
    _ds >> m_right_tsl_distance_threshold;
    _ds >> m_left_bwl_ideal_angle;
    _ds >> m_left_bwl_allowed_angle_variance;
    _ds >> m_left_bwl_min_votes;
    _ds >> m_left_bwl_iterations;
    _ds >> m_left_bwl_distance_threshold;
    _ds >> m_right_bwl_ideal_angle;
    _ds >> m_right_bwl_allowed_angle_variance;
    _ds >> m_right_bwl_min_votes;
    _ds >> m_right_bwl_iterations;
    _ds >> m_right_bwl_distance_threshold;
    _savefile.close();
    qDebug("Mary::loadMaryFromFile() marydefualt.tbi Loaded.");
    broadcastQMLSignals();
    broadcastSingletonSignals();
}

/**************************************************************
setCameraAOIWidth(int _width)
Public
Description:
  Sets The Width of the Camera AOI
**************************************************************/
void Mary::setCameraAOIWidth(int _width)
{       if(m_pylon_aoiwidth == _width) return;
        if(_width < 5) return;
        if(_width > m_pylon_maxcamerawidth) return;
        m_pylon_aoiwidth = _width;
        emit pylonCameraAOIWidthChanged();
}

/**************************************************************
setCameraAOIHeight(int _height)
Public
Description:
  Sets The Height of the Camera AOI
**************************************************************/
void Mary::setCameraAOIHeight(int _height)
{
    if(_height == m_pylon_aoiheight) return;
    if(_height < 5) return;
    if(_height > m_pylon_maxcameraheight) return;
    m_pylon_aoiheight = _height;
    emit pylonCameraAOIHeightChanged();
}

/**************************************************************
setCameraExposure(double _exposure)
Public
Description:
  Sets The Camera Exposure Data Container.
**************************************************************/
void Mary::setCameraExposure(double _exposure)
{
    if(_exposure == m_pylon_exposuretime) return;
    if((_exposure >= 22.0 && _exposure < 20000))
    {
        m_pylon_exposuretime = _exposure;
        emit pylonCameraExposureChanged();
        emit signalChangeCameraExposure(m_pylon_exposuretime);
    }
}

/**************************************************************
setCameraGain(int _gain)
Public
Description:
  Sets The Camera Gain Data Container.
**************************************************************/
void Mary::setCameraGain(int _gain)
{
    if(_gain == m_pylon_gain) return;
    if((_gain >= 0) && (_gain < 360))
    {

        m_pylon_gain = _gain;
        emit pylonCameraGainChanged();
        emit signalChangeCameraGain(_gain);
    }
}

/**************************************************************
setCVBlurValue(int _value)
Public
Description:
  Sets The Blur Value For the CV Blur Filter.
**************************************************************/
void Mary::setCVBlurValue(int _value)
{
    if(_value == m_cv_blur) return;
    if((_value >= 1) && (_value <= 50))
    {
        if((_value%2) != 1) ++_value;
        m_cv_blur = _value;
        emit cvBlurValueChanged();
        emit signalChangeBlur(m_cv_blur);
    }


}

/**************************************************************
setCVThresholdMinValue(int _value)
Public
Description:
  Sets The Min Threshold Value for the CV Filter
**************************************************************/
void Mary::setCVThresholdMinValue(int _value)
{
    if(_value == m_cv_thresholdmin) return;
    if((_value >= 0) && (_value < m_cv_thresholdmax))
    {
        m_cv_thresholdmin = _value;
        emit cvThresholdMinValueChanged();
        emit signalChangeThresholdMin(m_cv_thresholdmin);
    }

}

/**************************************************************
setCVThresholdMaxValue(int _value)
Public
Description:
  Sets The Max Threshold Value for the CV Filter
**************************************************************/
void Mary::setCVThresholdMaxValue(int _value)
{
    if(_value == m_cv_thresholdmax) return;
    if((_value > m_cv_thresholdmin) && (_value <= 255))
    {
        m_cv_thresholdmax = _value;
        emit cvThresholdMaxValueChanged();
        emit signalChangeThresholdMax(m_cv_thresholdmax);
    }
}

/**************************************************************
setMaxTII
Set Function
Description:
  Sets the Min TII
**************************************************************/
void Mary::setMaxTII(quint64 _maxtii)
{
    if(_maxtii > m_pc_min_tii)
    {
      m_pc_max_tii = _maxtii;
      emit pcMaxTIIChanged();
      emit signalChangeMaxTII(m_pc_max_tii);
    }
}

/**************************************************************
setMinTII
Set Function
Description:
  Sets the Min TII
**************************************************************/
void Mary::setMinTII(quint64 _mintii)
{
    if(_mintii > 0)
    {
        if(_mintii < m_pc_max_tii)
        {
            m_pc_min_tii = _mintii;
            emit pcMinTIIChanged();
            emit signalChangeMinTII(m_pc_min_tii);
        }
    }
}

/**************************************************************
setMinClusterSize
Set Function
Description:
  Sets the Min Cluster Size
**************************************************************/
void Mary::setMinClusterSize(int _cs)
{
    if(_cs > 0)
    {
        if(_cs < m_pc_max_clustersize)
        {
            m_pc_min_clustersize = _cs;
            emit pcMinClusterSizeChanged();
            emit signalChangeMinClusterSize(m_pc_min_clustersize);
        }
    }
}

/**************************************************************
setMaxClusterSize
Set Function
Description:
  Sets the Max Cluster Size Setting
**************************************************************/
void Mary::setMaxClusterSize(int _cs)
{
    if(_cs > m_pc_min_clustersize)
    {
        m_pc_max_clustersize = _cs;
        emit pcMaxClusterSizeChanged();
        emit signalChangeMaxClusterSize(m_pc_max_clustersize);
    }
}

/**************************************************************
setMaxClusterInCol
Set Function
Description:
  Sets the Max Clusters Per Column Setting
**************************************************************/
void Mary::setMaxClusterInCol(int _csincol)
{
    if(_csincol >= 1)
    {
        m_pc_max_clustersincolumn = _csincol;
        emit pcMaxClusterInColChanged();
        emit signalChangeMaxClustersInColumn(m_pc_max_clustersincolumn);
    }
}

/**************************************************************
setMaxDiscontinuity
Set Function
Description:
  Sets the Max Allowable Discontinuity For The Skeletal
  Processing.
**************************************************************/
void Mary::setMaxDiscontinuity(int _disc)
{
    if((_disc < 0) || (_disc > 701)) return;
    m_sk_max_discontinuity = _disc;
    emit skMaxDiscontuityChanged();
    emit signalChangeMaxDiscontinuity(m_sk_max_discontinuity);
}

/**************************************************************
setLeftTSLIdealAngle
Set Function
Description:
  Sets the Left TSL Min Angle Setting
**************************************************************/
void Mary::setLeftTSLIdealAngle(float _IdealAngle)
{
    if((_IdealAngle >= -90) && (_IdealAngle <= 90))
    {
        m_left_tsl_ideal_angle = _IdealAngle;
        emit rnLeftTSLIdealAngleChanged();
        emit signalLeftTSLIdealAngle(m_left_tsl_ideal_angle);
    }
}

/**************************************************************
setLeftTSLAllowedAngleVariance
Set Function
Description:
  Sets the Left TSL Max Angle Setting
**************************************************************/
void Mary::setLeftTSLAllowedAngleVariance(float _AllowedAngleVariance)
{
    if((_AllowedAngleVariance >= 0) && (_AllowedAngleVariance <= 90))
    {
        m_left_tsl_allowed_angle_variance = _AllowedAngleVariance;
        emit rnLeftTSLAllowedAngleVarianceChanged();
        emit signalLeftTSLAllowedAngleVariance(m_left_tsl_allowed_angle_variance);
    }
}

/**************************************************************
setLeftTSLMinVotes
Set Function
Description:
  Sets the Left TSL Votes
**************************************************************/
void Mary::setLeftTSLMinVotes(int _minvotes)
{
    if(_minvotes > 1)
    {
        m_left_tsl_min_votes = _minvotes;
        emit rnLeftTSLMinVotesChanged();
        emit signalLeftTSLMinVotes(m_left_tsl_min_votes);
    }
}

/**************************************************************
setLeftTSLMinVotes
Set Function
Description:
  Sets the Left TSL Distance Threshold
**************************************************************/
void Mary::setLeftTSLDistanceThreshold(float _distthreshold)
{
    if((_distthreshold >= 0))
    {
        m_left_tsl_distance_threshold = _distthreshold;
        emit rnLeftTSLDistanceThresholdChanged();
        emit signalLeftTSLDistanceThreshold(m_left_tsl_distance_threshold);
    }
}

/**************************************************************
setLeftTSLMinVotes
Set Function
Description:
  Sets the Left TSL Iterations
**************************************************************/
void Mary::setLeftTSLIterations(int _iterations)
{
    if(_iterations > 0)
    {
        m_left_tsl_iterations = _iterations;
        emit rnLeftTSLIterationsChanged();
        emit signalLeftTSLIterations(m_left_tsl_iterations);
    }
}

/**************************************************************
setRightTSLIdealAngle
Set Function
Description:
  Sets the Right TSL Min Angle Setting
**************************************************************/
void Mary::setRightTSLIdealAngle(float _IdealAngle)
{
    if((_IdealAngle >= -90) && (_IdealAngle <= 90))
    {
        m_right_tsl_ideal_angle = _IdealAngle;
        emit rnRightTSLIdealAngleChanged();
        emit signalRightTSLIdealAngle(m_right_tsl_ideal_angle);
    }
}

/**************************************************************
setRightTSLAllowedAngleVariance
Set Function
Description:
  Sets the Right TSL Max Angle Setting
**************************************************************/
void Mary::setRightTSLAllowedAngleVariance(float _AllowedAngleVariance)
{
    if((_AllowedAngleVariance >= 0) && (_AllowedAngleVariance <= 90))
    {
        m_right_tsl_allowed_angle_variance = _AllowedAngleVariance;
        emit rnRightTSLAllowedAngleVarianceChanged();
        emit signalRightTSLAllowedAngleVariance(m_right_tsl_allowed_angle_variance);
    }
}

/**************************************************************
setRightTSLMinVotes
Set Function
Description:
  Sets the Right TSL Votes
**************************************************************/
void Mary::setRightTSLMinVotes(int _minvotes)
{
    if(_minvotes > 1)
    {
        m_right_tsl_min_votes = _minvotes;
        emit rnRightTSLMinVotesChanged();
        emit signalRightTSLMinVotes(m_right_tsl_min_votes);
    }
}

/**************************************************************
setRightTSLMinVotes
Set Function
Description:
  Sets the Right TSL Distance Threshold
**************************************************************/
void Mary::setRightTSLDistanceThreshold(float _distthreshold)
{
    if((_distthreshold >= 0))
    {
        m_right_tsl_distance_threshold = _distthreshold;
        emit rnRightTSLDistanceThresholdChanged();
        emit signalRightTSLDistanceThreshold(m_right_tsl_distance_threshold);
    }
}

/**************************************************************
setRightTSLMinVotes
Set Function
Description:
  Sets the Right TSL Iterations
**************************************************************/
void Mary::setRightTSLIterations(int _iterations)
{
    if(_iterations > 0)
    {
        m_right_tsl_iterations = _iterations;
        emit rnRightTSLIterationsChanged();
        emit signalRightTSLIterations(m_right_tsl_iterations);
    }
}

/**************************************************************
setLeftBWLIdealAngle
Set Function
Description:
  Sets the Left BWL Min Angle Setting
**************************************************************/
void Mary::setLeftBWLIdealAngle(float _IdealAngle)
{
    if((_IdealAngle >= -90) && (_IdealAngle <= 90))
    {
        m_left_bwl_ideal_angle = _IdealAngle;
        emit rnLeftBWLIdealAngleChanged();
        emit signalLeftBWLIdealAngle(m_left_bwl_ideal_angle);
    }
}

/**************************************************************
setLeftBWLAllowedAngleVariance
Set Function
Description:
  Sets the Left BWL Max Angle Setting
**************************************************************/
void Mary::setLeftBWLAllowedAngleVariance(float _AllowedAngleVariance)
{
    if((_AllowedAngleVariance >= 0) && (_AllowedAngleVariance <= 90))
    {
        m_left_bwl_allowed_angle_variance = _AllowedAngleVariance;
        emit rnLeftBWLAllowedAngleVarianceChanged();
        emit signalLeftBWLAllowedAngleVariance(m_left_bwl_allowed_angle_variance);
    }
}

/**************************************************************
setLeftBWLMinVotes
Set Function
Description:
  Sets the Left BWL Votes
**************************************************************/
void Mary::setLeftBWLMinVotes(int _minvotes)
{
    if(_minvotes > 1)
    {
        m_left_bwl_min_votes = _minvotes;
        emit rnLeftBWLMinVotesChanged();
        emit signalLeftBWLMinVotes(m_left_bwl_min_votes);
    }
}

/**************************************************************
setLeftBWLMinVotes
Set Function
Description:
  Sets the Left BWL Distance Threshold
**************************************************************/
void Mary::setLeftBWLDistanceThreshold(float _distthreshold)
{
    if((_distthreshold >= 0))
    {
        m_left_bwl_distance_threshold = _distthreshold;
        emit rnLeftBWLDistanceThresholdChanged();
        emit signalLeftBWLDistanceThreshold(m_left_bwl_distance_threshold);
    }
}

/**************************************************************
setLeftBWLMinVotes
Set Function
Description:
  Sets the Left BWL Iterations
**************************************************************/
void Mary::setLeftBWLIterations(int _iterations)
{
    if(_iterations > 0)
    {
        m_left_bwl_iterations = _iterations;
        emit rnLeftBWLIterationsChanged();
        emit signalLeftBWLIterations(m_left_bwl_iterations);
    }
}

/**************************************************************
setRightBWLIdealAngle
Set Function
Description:
  Sets the Right BWL Min Angle Setting
**************************************************************/
void Mary::setRightBWLIdealAngle(float _IdealAngle)
{
    if((_IdealAngle >= -90) && (_IdealAngle <= 90))
    {
        m_right_bwl_ideal_angle = _IdealAngle;
        emit rnRightBWLIdealAngleChanged();
        emit signalRightBWLIdealAngle(m_right_bwl_ideal_angle);
    }
}

/**************************************************************
setRightBWLAllowedAngleVariance
Set Function
Description:
  Sets the Right BWL Max Angle Setting
**************************************************************/
void Mary::setRightBWLAllowedAngleVariance(float _AllowedAngleVariance)
{
    if((_AllowedAngleVariance >= 0) && (_AllowedAngleVariance <= 90))
    {
        m_right_bwl_allowed_angle_variance = _AllowedAngleVariance;
        emit rnRightBWLAllowedAngleVarianceChanged();
        emit signalRightBWLAllowedAngleVariance(m_right_bwl_allowed_angle_variance);
    }
}

/**************************************************************
setRightBWLMinVotes
Set Function
Description:
  Sets the Right BWL Votes
**************************************************************/
void Mary::setRightBWLMinVotes(int _minvotes)
{
    if(_minvotes > 1)
    {
        m_right_bwl_min_votes = _minvotes;
        emit rnRightBWLMinVotesChanged();
        emit signalRightBWLMinVotes(m_right_bwl_min_votes);
    }
}

/**************************************************************
setRightBWLMinVotes
Set Function
Description:
  Sets the Right BWL Distance Threshold
**************************************************************/
void Mary::setRightBWLDistanceThreshold(float _distthreshold)
{
    if((_distthreshold >= 0))
    {
        m_right_bwl_distance_threshold = _distthreshold;
        emit rnRightBWLDistanceThresholdChanged();
        emit signalRightBWLDistanceThreshold(m_right_bwl_distance_threshold);
    }
}

/**************************************************************
setRightBWLMinVotes
Set Function
Description:
  Sets the Right BWL Iterations
**************************************************************/
void Mary::setRightBWLIterations(int _iterations)
{
    if(_iterations > 0)
    {
        m_right_bwl_iterations = _iterations;
        emit rnRightBWLIterationsChanged();
        emit signalRightBWLIterations(m_right_bwl_iterations);
    }
}

/**************************************************************
setShowDebugInfo(bool _value)
Public
Description:
  Sets The Show Debug Info Flag For the GUI Display
**************************************************************/
void Mary::setShowDebugInfo(bool _value)
{
    if(_value != m_gui_showdebuginfo)
    {
        m_gui_showdebuginfo = _value;
        emit showDebugInfoChanged();
    }
}

/**************************************************************
broadcastQMLSignals()
Slot
Description:
  Slot That Resends all the Setting Emits To The QML
**************************************************************/
void Mary::broadcastQMLSignals()
{
    emit cvBlurValueChanged();
    emit cvThresholdMaxValueChanged();
    emit cvThresholdMinValueChanged();

    emit showDebugInfoChanged();

    emit pylonCameraAOIHeightChanged();
    emit pylonCameraAOIWidthChanged();
    emit pylonCameraExposureChanged();
    emit pylonCameraGainChanged();

    emit pcMaxTIIChanged();
    emit pcMinTIIChanged();
    emit pcMaxClusterSizeChanged();
    emit pcMinClusterSizeChanged();
    emit pcMaxClusterInColChanged();

    emit skMaxDiscontuityChanged();

    emit rnLeftTSLIdealAngleChanged();
    emit rnLeftTSLAllowedAngleVarianceChanged();
    emit rnLeftTSLMinVotesChanged();
    emit rnLeftTSLDistanceThresholdChanged();
    emit rnLeftTSLIterationsChanged();

    emit rnRightTSLIdealAngleChanged();
    emit rnRightTSLAllowedAngleVarianceChanged();
    emit rnRightTSLMinVotesChanged();
    emit rnRightTSLDistanceThresholdChanged();
    emit rnRightTSLIterationsChanged();

    emit rnLeftBWLIdealAngleChanged();
    emit rnLeftBWLAllowedAngleVarianceChanged();
    emit rnLeftBWLMinVotesChanged();
    emit rnLeftBWLDistanceThresholdChanged();
    emit rnLeftBWLIterationsChanged();

    emit rnRightBWLIdealAngleChanged();
    emit rnRightBWLAllowedAngleVarianceChanged();
    emit rnRightBWLMinVotesChanged();
    emit rnRightBWLDistanceThresholdChanged();
    emit rnRightBWLIterationsChanged();

    emit smSplitDistanceChanged();
    emit smSplitLengthChanged();


}

/**************************************************************
broadcastSingletonSignals()
Slot
Description:
  Slot That Resends all the Setting Emits To The Singletons
**************************************************************/
void Mary::broadcastSingletonSignals()
{
    emit signalChangeCameraAOI(m_pylon_aoiwidth, m_pylon_aoiheight);
    emit signalChangeCameraExposure(m_pylon_exposuretime);
    emit signalChangeCameraGain(m_pylon_gain);

    emit signalChangeBlur(m_cv_blur);
    emit signalChangeThresholdMin(m_cv_thresholdmin);
    emit signalChangeThresholdMax(m_cv_thresholdmax);

    emit signalChangeMaxTII(m_pc_max_tii);
    emit signalChangeMinTII(m_pc_min_tii);
    emit signalChangeMaxClusterSize(m_pc_max_clustersize);
    emit signalChangeMaxClustersInColumn(m_pc_max_clustersincolumn);
    emit signalChangeMinClusterSize(m_pc_min_clustersize);

    emit signalChangeMaxDiscontinuity(m_sk_max_discontinuity);

    emit signalLeftTSLIdealAngle(m_left_tsl_ideal_angle);
    emit signalLeftTSLAllowedAngleVariance(m_left_tsl_allowed_angle_variance);
    emit signalLeftTSLMinVotes(m_left_tsl_min_votes);
    emit signalLeftTSLDistanceThreshold(m_left_tsl_distance_threshold);
    emit signalLeftTSLIterations(m_left_tsl_iterations);

    emit signalRightTSLIdealAngle(m_right_tsl_ideal_angle);
    emit signalRightTSLAllowedAngleVariance(m_right_tsl_allowed_angle_variance);
    emit signalRightTSLMinVotes(m_right_tsl_min_votes);
    emit signalRightTSLDistanceThreshold(m_right_tsl_distance_threshold);
    emit signalRightTSLIterations(m_right_tsl_iterations);

    emit signalLeftBWLIdealAngle(m_left_bwl_ideal_angle);
    emit signalLeftBWLAllowedAngleVariance(m_left_bwl_allowed_angle_variance);
    emit signalLeftBWLMinVotes(m_left_bwl_min_votes);
    emit signalLeftBWLDistanceThreshold(m_left_bwl_distance_threshold);
    emit signalLeftBWLIterations(m_left_bwl_iterations);

    emit signalRightBWLIdealAngle(m_right_bwl_ideal_angle);
    emit signalRightBWLAllowedAngleVariance(m_right_bwl_allowed_angle_variance);
    emit signalRightBWLMinVotes(m_right_bwl_min_votes);
    emit signalRightBWLDistanceThreshold(m_right_bwl_distance_threshold);
    emit signalRightBWLIterations(m_right_bwl_iterations);


}


