#include "tbicore_gary.h"
#include <QDebug>
#include <QTextCodec>


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
    setOperationStatus(GaryOperationStatus::TBI_OPERATION_ERROR);
    m_homing_status = new GaryHomingStatus();
    m_x_position = 10.2345;
    //this->findOpenTeensy();
    this->findOpenArduinoUno();
    emit completed();
    qDebug() << "Gary::Gary() Object Created.";
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
    emit aboutToDestroy();
    if(m_serial_port)
    {
        if(m_serial_port->isOpen())
        {
            m_serial_port->close();
            qDebug() << "Gary::~Gary() Serial Port Closed.";
        }
    }
    qDebug() << "Gary::~Gary() Object Destroyed.";
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
void Gary::setOperationStatus(int _os)
{
    m_operation_status = _os;
    emit operationStatusChanged();
}
//--------------------------------------------------------------


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
                       qDebug() << "Gary::findOpenTeensy() Serial Port Opened.";
                       QObject::connect(m_serial_port, SIGNAL(errorOccured(QSerialPort::SerialPortError)), this, SLOT(serialError(QSerialPort::SerialPortError)));
                       setOperationStatus(GaryOperationStatus::TBI_OPERATION_OK);
                       return true;
                   }
                   else
                   {
                       qDebug() << "Gary::findOpenTeensy() Error Opening Serial Port";
                       setOperationStatus(GaryOperationStatus::TBI_OPERATION_ERROR);
                       return false;
                   }
               }
           }
       }
    }
    qDebug() << "Gary::findOpenTeensy() Could Not Find Teensy 3.2 Serial Device.";
    return false;
}


/**************************************************************
findOpenArduinoUno()
Public
Description:
  Public Method that finds the Arduino Uno USB device and opens
  a serial connection to it.
 **************************************************************/
bool Gary::findOpenArduinoUno()
{
    m_serial_port = nullptr;
    m_serial_info = new QSerialPortInfo();
    foreach(QSerialPortInfo _serialinfo, m_serial_info->availablePorts())
    {
       if(_serialinfo.hasProductIdentifier() && _serialinfo.hasVendorIdentifier() )
       {
           if(_serialinfo.vendorIdentifier() == m_arduino_uno_vendorID)
           {
               if(_serialinfo.productIdentifier() == m_arduino_uno_productID)
               {
                   m_serial_port = new QSerialPort(_serialinfo, this);
                   m_serial_port->setBaudRate(QSerialPort::Baud115200);
                   m_serial_port->setStopBits(QSerialPort::OneStop);
                   m_serial_port->setParity(QSerialPort::EvenParity);
                   m_serial_port->setDataBits(QSerialPort::Data8);
                   m_serial_port->setFlowControl(QSerialPort::NoFlowControl);
                   QObject::connect(m_serial_port, SIGNAL(readyRead()), this, SLOT(readSerial()));
                   if(m_serial_port->open(QIODevice::ReadWrite))
                   {
                       qDebug() << "Gary::findOpenArduinoUno() Serial Port Opened.";
                       setOperationStatus(GaryOperationStatus::TBI_OPERATION_OK);
                       return true;
                   }
                   else
                   {
                       qDebug() << "Gary::findOpenArduinoUno() Error Opening Serial Port";
                       setOperationStatus(GaryOperationStatus::TBI_OPERATION_ERROR);
                       return false;
                   }
               }
           }
       }
    }
    qDebug() << "Gary::findOpenArduinoUno() Could Not Find Arduino Uno.";
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
//--------------------------------------------------------------


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
//--------------------------------------------------------------


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
//--------------------------------------------------------------


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

/**************************************************************
sendToggleLaserPower()
Public, Q_INVOKABLE
Description:
  Public Method that sends the Toggle Laser Power Signal to the
  Microcontroller
 **************************************************************/
void Gary::sendToggleLaserPower()
{
    QByteArray _cmd;
    _cmd.append(char(GaryCommands::TBI_CMD_TOGGLE_LASER_POWER));
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
        qDebug() << "Gary::sendSerialCommand() Error in sendSerialCommand: m_serial_port is null";
        return;
    }
    if(m_serial_port->isOpen())
    {
       _data.append(char(GaryCommands::TBI_CMD_DELMIITER_LF));
        m_serial_port->write(_data);
        qDebug() << "Gary::sendSerialCommand() Sending Command to Controller: " << _data.toHex(',');
    }
    else
    {
        qDebug() << "Gary::sendSerialCommand() Error in sendSerialCommand: m_serial_port is not open";
    }
}
//--------------------------------------------------------------


/**************************************************************
serialError(QSerialPort::SerialPortError _error)
Slot
Description:
  Handler for the QSerialPort::errorOccured Signal
 **************************************************************/
void Gary::serialError(QSerialPort::SerialPortError _error)
{
    setOperationStatus(GaryOperationStatus::TBI_OPERATION_ERROR);
}


/**************************************************************
readSerial()
Slot
Description:
  Handles the readyRead signal from the Serial Port.
 **************************************************************/
void Gary::readSerial()
{
    m_serial_port->re
    QByteArray _data = m_serial_port->readAll();
    //You can use QTextCodec to convert the bytearray to a string:
    // (1015 is UTF-16, 1014 UTF-16LE, 1013 UTF-16BE, 106 UTF-8)
    //QString _datas = QTextCodec::codecForMib(1015)->toUnicode(_data);
    //QString _datas = QString::from
    qDebug() << "Gary: readSerial() Fired";
   // qDebug() << _datas;
    qDebug() << QString::fromStdString(_data.toStdString());
}
//--------------------------------------------------------------







