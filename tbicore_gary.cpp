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
    m_x_homing_status = GaryHomingStatus::TBI_HOMING_STATUS_HOMED;
    m_z_homing_status = GaryHomingStatus::TBI_HOMING_STATUS_HOMED;
    m_last_status_guid = 4294967295;
    m_last_status_guid_compared_by_gary = 4294967295;
    m_motor_calib_cycle = GaryMotorCalibrationCycleStatus::TBI_MOTORCAL_OFF;
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
                        return true;
                    }
                    else
                    {
                        qDebug() << "Gary::findOpenTeensy() Error Opening Serial Port";
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
                        return true;
                    }
                    else
                    {
                        qDebug() << "Gary::findOpenArduinoUno() Error Opening Serial Port";
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
        _cmd.append((char)GaryControlMode::TBI_CONTROL_MODE_FULLAUTO_MODE);
        break;
    case GaryControlMode::TBI_CONTROL_MODE_MOTORCALIBRATION:
        return;
        break;
    };
    sendSerialCommand(_cmd);
}

void Gary::setControlModeToFullAuto()
{
    QByteArray _cmd;
    _cmd.append((char)GaryCommands::TBI_CMD_SETCONTROLMODE);
    _cmd.append((char)GaryControlMode::TBI_CONTROL_MODE_FULLAUTO_MODE);
    sendSerialCommand(_cmd);
}

void Gary::setControlModeToHeightOnly()
{
    QByteArray _cmd;
    _cmd.append((char)GaryCommands::TBI_CMD_SETCONTROLMODE);
    _cmd.append((char)GaryControlMode::TBI_CONTROL_MODE_HEIGHTONLY);
    sendSerialCommand(_cmd);
}

void Gary::setControlModeToManual()
{
    QByteArray _cmd;
    _cmd.append((char)GaryCommands::TBI_CMD_SETCONTROLMODE);
    _cmd.append((char)GaryControlMode::TBI_CONTROL_MODE_MANUAL_MODE);
    sendSerialCommand(_cmd);
}

void Gary::setControlModeToMotorCalibration()
{
    QByteArray _cmd;
    _cmd.append((char)GaryCommands::TBI_CMD_SETCONTROLMODE);
    _cmd.append((char)GaryControlMode::TBI_CONTROL_MODE_MOTORCALIBRATION);
    sendSerialCommand(_cmd);
}

void Gary::setControlMode(GaryControlMode::ControlMode_t _mode)
{
    QByteArray _cmd;
    _cmd.append((char)GaryCommands::TBI_CMD_SETCONTROLMODE);
    _cmd.append((char)_mode);
    sendSerialCommand(_cmd);
}

bool Gary::isStatusGUIDDifferentFromLastCompare()
{
    if(m_last_status_guid == m_last_status_guid_compared_by_gary)
    {
        return false;
    }
    else
    {
        m_last_status_guid_compared_by_gary = m_last_status_guid;
        return true;
    }
    return false;
}


/**************************************************************
sendToggleLaserPower()
Public, Q_INVOKABLE
Description:
  Public Method that sends the Toggle Laser Power Signal to the
  Microcontroller
 **************************************************************/
void Gary::toggleLaserPower()
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
    if(_data.size() > TBIConstants::TBI_COMMAND_BUFFER_SIZE)
    {
        qDebug() << "Gary: sendSerialCommand() Error. _data is larger than allowed size.";
        return;
    }
    if(m_serial_port->isOpen())
    {
        //Make Sure the Data Array is TBI_COMMAND_BUFFER_SIZE in size.
        //The Microcontroller an incomming packet to be this size.
        while(_data.size() < TBIConstants::TBI_COMMAND_BUFFER_SIZE)
        {
            _data.append(char(0xFF));
        }
        m_serial_port->write(_data);
        //qDebug() << "Gary::sendSerialCommand() Sending Command to Controller: " << _data.toHex(',');
    }
    else
    {
        qDebug() << "Gary::sendSerialCommand() Error in sendSerialCommand: m_serial_port is not open";
    }
}

void Gary::printControlStatusVariables()
{

    qDebug() << "";
    qDebug() << "-------------------------------------------------------";
    qDebug() << "Control Status Debug Information";
    qDebug() << "m_last_status_guid = " << m_last_status_guid;
    switch(m_x_motion_status)
    {
    case GaryMotionStatus::TBI_MOTION_STATUS_IDLE:
        qDebug() << "m_x_motion_status = TBI_MOTION_STATUS_IDLE";
        break;
    case GaryMotionStatus::TBI_MOTION_STATUS_HOMING:
        qDebug() << "m_x_motion_status = TBI_MOTIION_STATUS_HOMING";
        break;
    case GaryMotionStatus::TBI_MOTION_STATUS_GENERAL_ERROR:
        qDebug() << "m_x_motion_status = TBI_MOTIION_STATUS_HOMING";
        break;
    case GaryMotionStatus::TBI_MOTION_STATUS_JOGGING:
        qDebug() << "m_x_motion_status = TBI_MOTION_STATUS_JOGGING";
        break;
    case GaryMotionStatus::TBI_MOTION_STATUS_LIMIT_TRIPPED:
        qDebug() << "m_x_motion_status = TBI_MOTION_STATUS_LIMIT_TRIPPED";
        break;
    case GaryMotionStatus::TBI_MOTION_STATUS_MOVING:
        qDebug() << "m_x_motion_status = TBI_MOTION_STATUS_MOVING";
        break;
    }

    switch(m_z_motion_status)
    {
    case GaryMotionStatus::TBI_MOTION_STATUS_IDLE:
        qDebug() << "m_z_motion_status = TBI_MOTION_STATUS_IDLE";
        break;
    case GaryMotionStatus::TBI_MOTION_STATUS_HOMING:
        qDebug() << "m_z_motion_status = TBI_MOTIION_STATUS_HOMING";
        break;
    case GaryMotionStatus::TBI_MOTION_STATUS_GENERAL_ERROR:
        qDebug() << "m_z_motion_status = TBI_MOTIION_STATUS_HOMING";
        break;
    case GaryMotionStatus::TBI_MOTION_STATUS_JOGGING:
        qDebug() << "m_z_motion_status = TBI_MOTION_STATUS_JOGGING";
        break;
    case GaryMotionStatus::TBI_MOTION_STATUS_LIMIT_TRIPPED:
        qDebug() << "m_z_motion_status = TBI_MOTION_STATUS_LIMIT_TRIPPED";
        break;
    case GaryMotionStatus::TBI_MOTION_STATUS_MOVING:
        qDebug() << "m_z_motion_status = TBI_MOTION_STATUS_MOVING";
        break;
    }

    qDebug() << "m_x_position = " << m_x_position;
    qDebug() << "m_z_position = " << m_z_position;

    switch(m_control_mode)
    {
    case GaryControlMode::TBI_CONTROL_MODE_MANUAL_MODE:
        qDebug() << "m_control_mode = TBI_CONTROL_MODE_MANUAL_MODE";
        break;
    case GaryControlMode::TBI_CONTROL_MODE_FULLAUTO_MODE:
        qDebug() << "m_control_mode = TBI_CONTROL_MODE_FULLAUTO_MODE";
        break;
    case GaryControlMode::TBI_CONTROL_MODE_HEIGHTONLY:
        qDebug() << "m_control_mode = TBI_CONTROL_MODE_HEIGHTONLY";
        break;
    case GaryControlMode::TBI_CONTROL_MODE_MOTORCALIBRATION:
        qDebug() << "m_control_mode = TBI_CONTROL_MODE_MOTORCALIBRATION";
        break;
    }

    switch(m_x_axis_limit)
    {
    case GaryLimitSwitch::TBI_LIMIT_SWITCH_STATE_OK:
        qDebug() << "m_x_axis_limit = TBI_LIMIT_SWITCH_STATE_OK";
        break;
    case GaryLimitSwitch::TBI_LIMIT_SWITCH_STATE_TRIPPED:
        qDebug() << "m_x_axis_limit = TBI_LIMIT_SWITCH_STATE_TRIPPED";
        break;
    }

    switch(m_z_axis_limit)
    {
    case GaryLimitSwitch::TBI_LIMIT_SWITCH_STATE_OK:
        qDebug() << "m_z_axis_limit = TBI_LIMIT_SWITCH_STATE_OK";
        break;
    case GaryLimitSwitch::TBI_LIMIT_SWITCH_STATE_TRIPPED:
        qDebug() << "m_z_axis_limit = TBI_LIMIT_SWITCH_STATE_TRIPPED";
        break;
    }

    switch(m_operation_status)
    {
    case GaryOperationStatus::TBI_OPERATION_OK:
        qDebug() << "m_operation_status = TBI_OPERATION_OK";
        break;
    case GaryOperationStatus::TBI_OPERATION_ERROR:
        qDebug() << "m_operation_status = TBI_OPERATION_ERROR";
        break;
    }

    switch(m_laser_status)
    {
    case GaryLaserStatus::TBI_LASER_STATUS_ON:
        qDebug() << "m_laser_status = TBI_LASER_STATUS_ON";
        break;
    case GaryLaserStatus::TBI_LASER_STATUS_OFF:
        qDebug() << "m_laser_status = TBI_LASER_STATUS_OFF";
        break;
    }
    qDebug() << "-------------------------------------------------------";
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

    //The Controllers Status Packet is 21 Bytes Long.
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


    This mandates the TBI_CONTROL_STATUS_BUFFER_SIZE Including be 21 bytes.
    */
    if(m_recieved_serial.size() >= 21)
    {
        //All of the typecasting is required.
        m_last_status_guid = ((quint32)((quint8)m_recieved_serial[0] << 24)
                + (quint32)((quint8)m_recieved_serial[1] << 16)
                + (quint32)((quint8)m_recieved_serial[2] << 8)
                + (quint32)((quint8)m_recieved_serial[3]));

        m_x_motion_status = (GaryMotionStatus::MotionStatus_t) ((quint8)m_recieved_serial[4]);
        m_z_motion_status = (GaryMotionStatus::MotionStatus_t) ((quint8)m_recieved_serial[5]);

        m_x_position = ((qint32)((quint8)m_recieved_serial[6] << 24)
                + (qint32)((quint8)m_recieved_serial[7] << 16)
                + (qint32)((quint8)m_recieved_serial[8] << 8)
                + (qint32)((quint8)m_recieved_serial[9]));

        m_z_position = ((qint32)((quint8)m_recieved_serial[10] << 24)
                + (qint32)((quint8)m_recieved_serial[11] << 16)
                + (qint32)((quint8)m_recieved_serial[12] << 8)
                + (qint32)((quint8)m_recieved_serial[13]));

        m_control_mode = (GaryControlMode::ControlMode_t) ((quint8)m_recieved_serial[14]);

        m_x_axis_limit = (GaryLimitSwitch::LimitSwitchState_t) ((quint8)m_recieved_serial[15]);
        m_z_axis_limit = (GaryLimitSwitch::LimitSwitchState_t) ((quint8)m_recieved_serial[16]);

        m_operation_status = (GaryOperationStatus::OperationStatus_t) ((quint8)m_recieved_serial[17]);

        m_laser_status = (GaryLaserStatus::LaserStatus_t) ((quint8)m_recieved_serial[18]);

        m_x_homing_status = (GaryHomingStatus::HomingStatus_t) ((quint8)m_recieved_serial[19]);
        m_z_homing_status = (GaryHomingStatus::HomingStatus_t) ((quint8)m_recieved_serial[20]);

        m_motor_calib_cycle = (GaryMotorCalibrationCycleStatus::MotorCalibration_Cycle_t) ((quint8)m_recieved_serial[21]);

        emit xMotionStatusChanged();
        emit zMotionStatusChanged();
        emit xPositionChanged();
        emit zPositionChanged();
        emit controlModeChanged();
        emit xLimitSwitchChanged();
        emit zLimitSwitchChanged();
        emit operationStatusChanged();
        emit laserStatusChanged();
        emit currentStatusGUIDChanged();
        emit motorCalibrationCycleChanged();
        emit xHomingStatusChanged();
        emit zHomingStatusChanged();
        //Done Processing clear the data array
        //printControlStatusVariables();
        m_recieved_serial.clear();
    }

}
//--------------------------------------------------------------







