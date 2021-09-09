#include "tbiparameterclass_motioncontrolparams.h"

TBIMotionControlParameters::TBIMotionControlParameters(QObject *parent) : QObject(parent)
{
    this->setDefautValues();
}

void TBIMotionControlParameters::setXStepsPerPixel(float _value)
{
    m_x_steps_per_pixel = _value;
    emit xStepsPerPixelChanged();
}

void TBIMotionControlParameters::setZStepsPerPixel(float _value)
{
    m_z_steps_per_pixel = _value;
    emit zStepsPerPixelChanged();
}

void TBIMotionControlParameters::setDefautValues()
{
    this->setXStepsPerPixel(0);
    this->setZStepsPerPixel(0);
}

void TBIMotionControlParameters::saveToFile(QDataStream &_filedatastream)
{
    _filedatastream << m_x_steps_per_pixel;
    _filedatastream << m_z_steps_per_pixel;
}

void TBIMotionControlParameters::loadFromFile(QDataStream &_filedatastream)
{
    _filedatastream >> m_x_steps_per_pixel;
    _filedatastream >> m_z_steps_per_pixel;
    emit xStepsPerPixelChanged();
    emit zStepsPerPixelChanged();
}
