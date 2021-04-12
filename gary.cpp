#include "gary.h"
#include <QDebug>


/**************************************************************
Gary(QObject *parent) : QObject(parent)
Public
Description:
  Gary Constructor
 **************************************************************/
Gary::Gary(QObject *parent) : QObject(parent)
{
    m_motion_status = new GaryMotionStatus();
    m_control_mode = new GaryControlMode();
    m_x_axis_limit = new GaryLimitSwitch();
    m_z_axis_limit = new GaryLimitSwitch();
    m_operation_status = new GaryOperationStatus();
    m_homing_status = new GaryHomingStatus();
    m_x_position = 10.2345;
    this->findOpenTeensy();
    emit completed();
    qDebug() << "Gary Object Instantiated.";
}
//--------------------------------------------------------------

/**************************************************************
~Gary()
Public
Description:
  Gary Destructor
 **************************************************************/
Gary::~Gary()
{
    emit this->aboutToDestroy();
    qDebug() << "Gary Object Destroyed.";
    m_serial_port->close();
    if(m_serial_port)
    {
        if(m_serial_port->isOpen())
        {
            m_serial_port->close();
            qDebug() << "Serial Port Closed.";
        }
    }
}
//--------------------------------------------------------------


/**************************************************************
declareQML()
Public
Description:
  Public Method that Reqisters the Gary Type into the QML Engine.
 **************************************************************/
void Gary::declareQML()
{
    qmlRegisterType<Gary>("tbi.vision.components", 1, 0, "Gary");
}
//--------------------------------------------------------------


/**************************************************************
setMotionStatus(GaryMotionStatus *_ms)
Public
Description:
  Public set Property method.
 **************************************************************/
void Gary::setMotionStatus(GaryMotionStatus *_ms)
{

}
//--------------------------------------------------------------


/**************************************************************
setControlMode(GaryControlMode *_cm)
Public
Description:
  Public set Property method.
 **************************************************************/
void Gary::setControlMode(GaryControlMode *_cm)
{

}
//--------------------------------------------------------------


/**************************************************************
setHomingStatus(GaryHomingStatus *_hs)
Public
Description:
  Public set Property method.
 **************************************************************/
void Gary::setHomingStatus(GaryHomingStatus *_hs)
{

}
//--------------------------------------------------------------


/**************************************************************
setXLimitSwitch(GaryLimitSwitch *_ls)
Public
Description:
  Public set Property method.
 **************************************************************/
void Gary::setXLimitSwitch(GaryLimitSwitch *_ls)
{

}
//--------------------------------------------------------------


/**************************************************************
setZLimitSwitch(GaryLimitSwitch *_ls)
Public
Description:
  Public set Property method.
 **************************************************************/
void Gary::setZLimitSwitch(GaryLimitSwitch *_ls)
{

}
//--------------------------------------------------------------


/**************************************************************
setOperationStatus(GaryOperationStatus *_os)
Public
Description:
  Public set Property method.
 **************************************************************/
void Gary::setOperationStatus(GaryOperationStatus *_os)
{

}

void Gary::setXPosition(float _x_pos)
{
    m_x_position = _x_pos;
}
//--------------------------------------------------------------


/**************************************************************
findOpenTeensy()
Public
Description:
  Public Method that finds the Teensy 3.2 USB device and opens
  a serial connection to it.
 **************************************************************/
bool Gary::findOpenTeensy()
{
    m_serial_port = nullptr;
    m_serial_info = new QSerialPortInfo();
    //Poll Each Serial Port Look For The Teensy 3.2
    foreach(QSerialPortInfo _serialinfo, m_serial_info->availablePorts())
    {
       if(_serialinfo.hasProductIdentifier() && _serialinfo.hasVendorIdentifier() )
       {
           if(_serialinfo.vendorIdentifier() == m_teensy32_vendorID)
           {
               if(_serialinfo.productIdentifier() == m_teensy32_productID)
               {
                   m_serial_port = new QSerialPort(_serialinfo, this);
                   m_serial_port->setBaudRate(QSerialPort::Baud115200);
                   m_serial_port->setStopBits(QSerialPort::OneStop);
                   m_serial_port->setParity(QSerialPort::EvenParity);
                   m_serial_port->setDataBits(QSerialPort::Data8);
                   m_serial_port->setFlowControl(QSerialPort::NoFlowControl);
                   if(m_serial_port->open(QIODevice::ReadWrite))
                   {
                       qDebug() << "Serial Port Opened.";
                       return true;
                   }
                   else
                   {
                       qDebug() << "Error Opening Serial Port";
                       return false;
                   }
               }
           }
       }
    }
    qDebug() << "Could Not Find Teensy 3.2 Serial Device. Microcontroller not Available.";
    return false;
}
//--------------------------------------------------------------


/**************************************************************
sendJogStop()
Public, Q_INVOKABLE
Description:
  Public Method that sends the Movement Stop Command to the
  microcontroller
 **************************************************************/
Q_INVOKABLE void Gary::sendStopMovement()
{
        QByteArray _cmd;
        _cmd.append(char(GaryCommands::TBI_CMD_STOP_MOVEMENT));
        sendSerialCommand(_cmd);
}
//--------------------------------------------------------------


/**************************************************************
sendJogUp()
Public, Q_INVOKABLE
Description:
  Public Method that sends the Jog Up Command to the
  microcontroller
 **************************************************************/
Q_INVOKABLE void Gary::sendJogUp()
{
       QByteArray _cmd;
       _cmd.append(char(GaryCommands::TBI_CMD_JOG_UP));
       sendSerialCommand(_cmd);
}


/**************************************************************
sendJogDown()
Public, Q_INVOKABLE
Description:
  Public Method that sends the Jog Down Command to the
  microcontroller
 **************************************************************/
Q_INVOKABLE void Gary::sendJogDown()
{
    QByteArray _cmd;
    _cmd.append(char(GaryCommands::TBI_CMD_JOG_DOWN));
    sendSerialCommand(_cmd);
}


/**************************************************************
sendJogLeft()
Public, Q_INVOKABLE
Description:
  Public Method that sends the Jog Left Command to the
  microcontroller
 **************************************************************/
Q_INVOKABLE void Gary::sendJogLeft()
{
    QByteArray _cmd;
    _cmd.append(char(GaryCommands::TBI_CMD_JOG_LEFT));
    sendSerialCommand(_cmd);
}


/**************************************************************
sendJogRight()
Public, Q_INVOKABLE
Description:
  Public Method that sends the Jog Right Command to the
  microcontroller
 **************************************************************/
Q_INVOKABLE void Gary::sendJogRight()
{
    QByteArray _cmd;
    _cmd.append(char(GaryCommands::TBI_CMD_JOG_RIGHT));
    sendSerialCommand(_cmd);
}
//--------------------------------------------------------------


/**************************************************************
sendSerialCommand(QByteArray &_data)
Private
Description:
  Private Method that facilitaes all the serial send
  functionality.
 **************************************************************/
void Gary::sendSerialCommand(QByteArray &_data)
{

    if(m_serial_port == nullptr)
    {
        qDebug() << "Error in sendSerialCommand: m_serial_port is null";
        return;
    }
    if(m_serial_port->isOpen())
    {
        m_serial_port->write(_data);
        qDebug() << "Sending Command to Controller: " << _data.toHex(',');
    }
    else
    {
        qDebug() << "Error in sendSerialCommand: m_serial_port is not open";
    }
}
//--------------------------------------------------------------







