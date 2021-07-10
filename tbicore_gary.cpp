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
    m_x_motion_status = GaryMotionStatus::TBI_MOTION_STATUS_IDLE;
    m_z_motion_status = GaryMotionStatus::TBI_MOTION_STATUS_IDLE;
    m_control_mode = GaryControlMode::TBI_CONTROL_MODE_MANUAL_MODE;
    m_x_axis_limit = GaryLimitSwitch::TBI_LIMIT_SWITCH_STATE_OK;
    m_z_axis_limit = GaryLimitSwitch::TBI_LIMIT_SWITCH_STATE_OK;
    m_operation_status = GaryOperationStatus::TBI_OPERATION_OK;
    m_homing_status = GaryHomingStatus::TBI_HOMING_STATUS_HOMED;
    m_last_status_guid = 4294967295;
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
setXMotionStatus(GaryMotionStatus *_ms)
Public
Description:
  Public set Property method.
 **************************************************************/
void Gary::setXMotionStatus(GaryMotionStatus::MotionStatus_t _ms_t)
{
    m_x_motion_status = _ms_t;
    emit xMotionStatusChanged();
}
//--------------------------------------------------------------

/**************************************************************
setZMotionStatus(GaryMotionStatus *_ms)
Public
Description:
  Public set Property method.
 **************************************************************/
void Gary::setZMotionStatus(GaryMotionStatus::MotionStatus_t _ms_t)
{
    m_z_motion_status = _ms_t;
    emit zMotionStatusChanged();
}
//--------------------------------------------------------------



/**************************************************************
setControlMode(GaryControlMode *_cm)
Public
Description:
  Public set Property method.
 **************************************************************/
void Gary::setControlMode(GaryControlMode::ControlMode_t _cm_t)
{
    QByteArray _cmd;
    _cmd.append(GaryCommands::TBI_CMD_SETCONTROLMODE);
    _cmd.append(_cm_t);
    sendSerialCommand(_cmd);
}
//--------------------------------------------------------------


/**************************************************************
setHomingStatus(GaryHomingStatus *_hs)
Public
Description:
  Public set Property method.
 **************************************************************/
void Gary::setHomingStatus(GaryHomingStatus::HomingStatus_t _hs_t)
{
    m_homing_status = _hs_t;
    emit homingStatusChanged();
}
//--------------------------------------------------------------


/**************************************************************
setXLimitSwitch(GaryLimitSwitch *_ls)
Public
Description:
  Public set Property method.
 **************************************************************/
void Gary::setXLimitSwitch(GaryLimitSwitch::LimitSwitchState_t _ls_t)
{
    m_x_axis_limit = _ls_t;
    emit xLimitSwitchChanged();
}
//--------------------------------------------------------------

/**************************************************************
setZLimitSwitch(GaryLimitSwitch *_ls)
Public
Description:
  Public set Property method.
 **************************************************************/
void Gary::setZLimitSwitch(GaryLimitSwitch::LimitSwitchState_t _ls_t)
{
    m_z_axis_limit = _ls_t;
    emit zLimitSwitchChanged();
}
//--------------------------------------------------------------


/**************************************************************
setOperationStatus(GaryOperationStatus *_os)
Public
Description:
  Public set Property method.
 **************************************************************/
void Gary::setOperationStatus(GaryOperationStatus::OperationStatus_t _os_t)
{
    m_operation_status = _os_t;
    emit operationStatusChanged();
}

/**************************************************************
setLaserStatus(GaryLaserStatus::LaserStatus_t _ls_t)
Public
Description:
  Public set Property method.
 **************************************************************/
void Gary::setLaserStatus(GaryLaserStatus::LaserStatus_t _ls_t)
{
    m_laser_status = _ls_t;
    emit laserStatusChanged();
}



void Gary::setXPosition(qint32 _x_pos)
{
    m_x_position = _x_pos;
    emit xPositionChanged();
}
//--------------------------------------------------------------


void Gary::setZPosition(qint32 _z_pos)
{
    m_z_position = _z_pos;
    emit zPositionChanged();
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

                    m_serial_port->setBaudRate(2000000);
                    //m_serial_port->setBaudRate(QSerialPort::Baud115200);
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
    if(m_z_motion_status != GaryMotionStatus::TBI_MOTION_STATUS_IDLE) return;
    QByteArray _cmd;
    _cmd.append(char(GaryCommands::TBI_CMD_JOG_UP));
    m_z_motion_status = GaryMotionStatus::TBI_MOTION_STATUS_JOGGING;
    emit zMotionStatusChanged();
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
    if(m_z_motion_status != GaryMotionStatus::TBI_MOTION_STATUS_IDLE) return;
    QByteArray _cmd;
    _cmd.append(char(GaryCommands::TBI_CMD_JOG_DOWN));
    m_z_motion_status = GaryMotionStatus::TBI_MOTION_STATUS_JOGGING;
    emit zMotionStatusChanged();
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
    if(m_x_motion_status != GaryMotionStatus::TBI_MOTION_STATUS_IDLE) return;
    QByteArray _cmd;
    _cmd.append(char(GaryCommands::TBI_CMD_JOG_LEFT));
    m_x_motion_status = GaryMotionStatus::TBI_MOTION_STATUS_JOGGING;
    emit xMotionStatusChanged();
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
    if(m_x_motion_status != GaryMotionStatus::TBI_MOTION_STATUS_IDLE) return;
    QByteArray _cmd;
    _cmd.append(char(GaryCommands::TBI_CMD_JOG_RIGHT));
    m_x_motion_status = GaryMotionStatus::TBI_MOTION_STATUS_JOGGING;
    emit xMotionStatusChanged();
    sendSerialCommand(_cmd);
}


/**************************************************************
autoMoveXAxis()
Public, Q_INVOKABLE
Description:
  Public Method that sends the Move X steps command
 **************************************************************/
void Gary::autoMoveXAxis(qint32 _steps)
{
    if(m_x_motion_status != GaryMotionStatus::TBI_MOTION_STATUS_IDLE) return;
    QByteArray _cmd;
    _cmd.append((char)GaryCommands::TBI_CMD_MOVEXSTEPS);
    _cmd.append((char*)&_steps, sizeof (_steps));
    m_x_motion_status = GaryMotionStatus::TBI_MOTION_STATUS_MOVING;
    emit xMotionStatusChanged();
    sendSerialCommand(_cmd);

}

/**************************************************************
autoMoveZAxis()
Public, Q_INVOKABLE
Description:
  Public Method that sends the Move Z steps command
 **************************************************************/
void Gary::autoMoveZAxis(qint32 _steps)
{
    if(m_z_motion_status != GaryMotionStatus::TBI_MOTION_STATUS_IDLE) return;
    QByteArray _cmd;
    _cmd.append((char)GaryCommands::TBI_CMD_MOVEZSTEPS);
    _cmd.append((char*)&_steps, sizeof (_steps));
    m_z_motion_status = GaryMotionStatus::TBI_MOTION_STATUS_MOVING;
    emit zMotionStatusChanged();
    sendSerialCommand(_cmd);
}


/**************************************************************
autoMoveZAxis()
Public, Q_INVOKABLE
Description:
  Public Method that sends the Move Z steps command
 **************************************************************/
void Gary::sendStatusPacket()
{
    QByteArray _cmd;
    _cmd.append((char)GaryCommands::TBI_CMD_SEND_STATUS);
    sendSerialCommand(_cmd);
}

void Gary::setControllerToCalibrationSpeed()
{
    QByteArray _cmd;
    _cmd.append((char)GaryCommands::TBI_CMD_SET_CALIBRATION_SPEED);
    sendSerialCommand(_cmd);
}

void Gary::setControllerToOperationSpeed()
{
    QByteArray _cmd;
    _cmd.append((char)GaryCommands::TBI_CMD_SET_OPERATION_SPEED);
    sendSerialCommand(_cmd);
}


/**************************************************************
cycleControlModes()
Public, Q_INVOKABLE
Description:
  Debug Function For Cycling Thru Control Modes
 **************************************************************/
void Gary::cycleControlModes()
{
    QByteArray _cmd;
    _cmd.append((char)GaryCommands::TBI_CMD_SETCONTROLMODE);
    switch(m_control_mode)
    {
    case GaryControlMode::TBI_CONTROL_MODE_FULLAUTO_MODE:
        _cmd.append((char)GaryControlMode::TBI_CONTROL_MODE_HEIGHTONLY);
        break;
    case GaryControlMode::TBI_CONTROL_MODE_HEIGHTONLY:
        _cmd.append((char)GaryControlMode::TBI_CONTROL_MODE_MANUAL_MODE);
        break;
    case GaryControlMode::TBI_CONTROL_MODE_MANUAL_MODE:
        _cmd.append(GaryControlMode::TBI_CONTROL_MODE_MOTORCALIBRATION);
        break;
    case GaryControlMode::TBI_CONTROL_MODE_MOTORCALIBRATION:
        _cmd.append((char)GaryControlMode::TBI_CONTROL_MODE_FULLAUTO_MODE);
        break;
    };
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


    QByteArray _data = m_serial_port->readAll();
    m_recieved_serial.append(_data);

    //The Controllers Status Packet is 19 Bytes Long.
    //After 19 Bytes Have Transfered Process The Info.

    /*
    NOTE: The Only Thing The Controller Sends Back To The QT Application is a Status Packet.
    Below is the Structure of the Status Packet

    [Byte 0,1,2,3] - uint32_t Status GUID
    [Byte 4] - The Controller Motion Status of X Axis. Of Type MotionStatus_t.
    [Byte 5] - The Controller Motion Status of Z Axis. Of Type MotionStatus_t.
    [Byte 6,7,8,9] X Position of Controller as int32_t.
    [Byte 10,11,12,13] Z Position of Controller as int32_t.
    [Byte 14] - The Controller Control Mode. Of Type ControlMode_t
    [Byte 15] - The Controller X Axis Limit Switch State - Of Type LimitSwitchState_t
    [Byte 16] - The Controller Z Axis Switch State - Of Type LimitSwitchState_t
    [Byte 17] - The Controller Operation Status Condition. Of Type OperationStatus_t.
    [Byte 18] - Laser Power Status. 0 - Off, 1 - On.


    This mandates the TBI_CONTROL_STATUS_BUFFER_SIZE Including be 20 bytes.
    */
    if(m_recieved_serial.size() >= 19)
    {
        char *_dataptr = _data.data();
        m_last_status_guid = (quint32) *(&_dataptr[0]);
        m_x_motion_status = (GaryMotionStatus::MotionStatus_t) _dataptr[4];
        m_z_motion_status = (GaryMotionStatus::MotionStatus_t) _dataptr[5];
        m_x_position = (qint32)*(&_dataptr[6]);
        m_z_position = (qint32)*(&_dataptr[10]);
        m_control_mode = (GaryControlMode::ControlMode_t) _dataptr[14];
        m_x_axis_limit = (GaryLimitSwitch::LimitSwitchState_t) _dataptr[15];
        m_z_axis_limit = (GaryLimitSwitch::LimitSwitchState_t) _dataptr[16];
        m_operation_status = (GaryOperationStatus::OperationStatus_t) _dataptr[17];
        m_laser_status = (GaryLaserStatus::LaserStatus_t) _dataptr[18];

        emit xMotionStatusChanged();
        emit zMotionStatusChanged();
        emit xPositionChanged();
        emit zPositionChanged();
        emit controlModeChanged();
        emit xLimitSwitchChanged();
        emit zLimitSwitchChanged();
        emit operationStatusChanged();
        emit laserStatusChanged();
        //Done Processing clear the data array
        m_recieved_serial.clear();
    }

    //You can use QTextCodec to convert the bytearray to a string:
    // (1015 is UTF-16, 1014 UTF-16LE, 1013 UTF-16BE, 106 UTF-8)
    //QString _datas = QTextCodec::codecForMib(1015)->toUnicode(_data);
    //QString _datas = QString::from
    //qDebug() << "Gary: readSerial() Fired";
    // qDebug() << _datas;
    // qDebug() << QString::fromStdString(_data.toStdString());
}
//--------------------------------------------------------------







