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
    m_cv_blur = 3;
    m_cv_thresholdmax = 255;
    m_cv_thresholdmin = 0;
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
sendCameraSettings()
Slot
Description:
  Slot That Resends all the Setting Emits
**************************************************************/
void Mary::broadcastUpdateSignals()
{
    emit signalChangeCameraAOI(m_pylon_aoiwidth, m_pylon_aoiheight);
    emit signalChangeCameraExposure(m_pylon_exposuretime);
    emit signalChangeCameraGain(m_pylon_gain);
    emit signalChangeBlur(m_cv_blur);
    emit signalChangeThresholdMin(m_cv_thresholdmin);
    emit signalChangeThresholdMax(m_cv_thresholdmax);
}
