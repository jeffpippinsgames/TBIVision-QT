#include "tbiparameterclass_pyloncameraparams.h"

TBIPylonCameraParamers::TBIPylonCameraParamers(QObject *parent) : QObject(parent)
{
    this->setDefautValues();
}

void TBIPylonCameraParamers::setMaxHeight(int _height)
{
    m_max_height = _height;
    emit maxHeightChanged();
}

void TBIPylonCameraParamers::setMaxWidth(int _width)
{
    m_max_width = _width;
    emit maxWidthChanged();
}

void TBIPylonCameraParamers::setCameraName(QString _name)
{
    m_camera_name = _name;
    emit cameraNameChanged();
}

void TBIPylonCameraParamers::setAOIWidth(int _width)
{
    m_aoi_width = _width;
    emit aoiWidthChanged();
}

void TBIPylonCameraParamers::setAOIHeight(int _height)
{
    m_aoi_height = _height;
    emit aoiHeightChanged();
}

void TBIPylonCameraParamers::setExposure(double _exposure)
{
    m_exposure = _exposure;
    emit exposureChanged();
}

void TBIPylonCameraParamers::setGain(int _gain)
{
    m_gain = _gain;
    emit gainChanged();
}

void TBIPylonCameraParamers::setDefautValues()
{
    this->setMaxHeight(TBIConstants::Max_Camera_Height);
    this->setMaxWidth(TBIConstants::Max_Camera_Width);
    this->setCameraName("No Camera Opened.");
    this->setAOIWidth(m_max_width);
    this->setAOIHeight(m_max_height);
    this->setExposure(2000);
    this->setGain(0);
}

void TBIPylonCameraParamers::saveToFile(QDataStream &_filedatastream)
{
    _filedatastream << m_exposure;
    _filedatastream << m_gain;
}

void TBIPylonCameraParamers::loadFromFile(QDataStream &_filedatastream)
{
    _filedatastream >> m_exposure;
    _filedatastream >> m_gain;
    emit exposureChanged();
    emit gainChanged();
}

