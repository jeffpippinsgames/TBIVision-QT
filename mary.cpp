#include "mary.h"
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

    qDebug() << "Mary: Mary Object Created.";
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
    qDebug() << "Mary: Mary Object Destroyed.";
}


/**************************************************************
SetMaryDefaultValues()
Public
Description:
  Sets The Default Values and Triggers the Changes.
**************************************************************/
void Mary::SetMaryDefaultValues()
{
    m_gui_showdebuginfo = true;
    emit showDebugInfoChanged();
    m_pylon_aoiheight = m_pylon_maxcameraheight;
    m_pylon_aoiwidth = m_pylon_maxcamerawidth;
    emit pylonCameraAOIHeightChanged();
    emit pylonCameraAOIWidthChanged();
    updateCameraAOIToMarysSettings();
    m_pylon_exposuretime = 3000.0;
    emit pylonCameraExposureChanged();
    m_pylon_gain = 0;
    emit pylonCameraGainChanged();
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
    _ds.setVersion(QDataStream::Qt_5_12);
    _savefile.close();
    qDebug() << "Mary: Mary Saved To " << _filepath;
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
        qDebug("Mary: marydefualt.tbi does not exsist.");
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
    _savefile.close();
    qDebug("Mary: marydefualt.tbi Loaded.");
}


/**************************************************************
setCameraAOIWidth(int _width)
Public
Description:
  Sets The Width of the Camera AOI
**************************************************************/
void Mary::setCameraAOIWidth(int _width)
{
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
    if((_exposure <= 22.0 && _exposure <= 100000000))
    {
        m_pylon_exposuretime = _exposure;
        emit pylonCameraExposureChanged();
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
    if((_gain >= 0) && (_gain <= 360))
    {
        m_pylon_gain = _gain;
        emit pylonCameraGainChanged();
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
    emit cvBlurValueChanged();
}


/**************************************************************
setCVThresholdMinValue(int _value)
Public
Description:
  Sets The Min Threshold Value for the CV Filter
**************************************************************/
void Mary::setCVThresholdMinValue(int _value)
{
    emit cvThresholdMinValueChanged();
}


/**************************************************************
setCVThresholdMaxValue(int _value)
Public
Description:
  Sets The Max Threshold Value for the CV Filter
**************************************************************/
void Mary::setCVThresholdMaxValue(int _value)
{
    emit cvThresholdMaxValueChanged();
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
