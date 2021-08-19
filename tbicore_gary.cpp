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
    m_controller_status = GaryControllerStatus::TBI_CONTROL_NONE;
    m_controller_autorepeat_timer = new QTimer(this);
    m_controller_autorepeatdelay_timer = new QTimer(this);
    m_controller_autorepeat_timer->setInterval(50);
    m_controller_autorepeatdelay_timer->setInterval(500);
    connect(m_controller_autorepeatdelay_timer, SIGNAL(timeout()), this, SLOT(onControllerRepeatDelayTimer()));
    connect(m_controller_autorepeat_timer, SIGNAL(timeout()), this, SLOT(onControllerRepeatTimer()));
    m_microcontroller_heartbeat_timer = new QTimer(this);
    m_microcontroller_heartbeat_timer->setInterval(100);
    connect(m_microcontroller_heartbeat_timer, SIGNAL(timeout()), this, SLOT(onKeepAliveTimer()));
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
            if((_serialinfo.vendorIdentifier() == m_arduino_uno_vendorID) || (_serialinfo.hasVendorIdentifier() == m_ftdi_FT232_UART_vendorID))
            {
                if((_serialinfo.productIdentifier() == m_arduino_uno_productID) || (_serialinfo.hasProductIdentifier() == m_ftdi_FT232_UART_productID))
                {
                    m_serial_port = new QSerialPort(_serialinfo, this);
                    m_serial_port->setBaudRate(2000000);
                    m_serial_port->setStopBits(QSerialPort::OneStop);
                    m_serial_port->setParity(QSerialPort::EvenParity);
                    m_serial_port->setDataBits(QSerialPort::Data8);
                    m_serial_port->setFlowControl(QSerialPort::NoFlowControl);
                    QObject::connect(m_serial_port, SIGNAL(readyRead()), this, SLOT(readSerial()));
                    if(m_serial_port->open(QIODevice::ReadWrite))
                    {
                        m_microcontroller_heartbeat_timer->start();
                        acknowledgeStatusPacket();
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
void Gary::acknowledgeStatusPacket()
{
    QByteArray _cmd;
    _cmd.append(uint(GaryCommands::TBI_CMD_ACKNOWLEDGE_STATUS_PACKET));
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

void Gary::sendProceedNextMotorPhase()
{
    QByteArray _cmd;
    _cmd.append((char)GaryCommands::TBI_CMD_CONTINUE_MOTOR_CALIBRATION);
    qDebug() << "Sent GaryCommands::TBI_CMD_CONTINUE_MOTOR_CALIBRATION";
    sendSerialCommand(_cmd);
}

void Gary::startMotorCalibration(qint32 _steps)
{
    QByteArray _cmd;
    _cmd.append((char)GaryCommands::TBI_CMD_CONTINUE_MOTOR_CALIBRATION);
    _cmd.append((char)((quint8)((_steps & 0xFF000000) >> 24)));
    _cmd.append((char)((quint8)((_steps & 0x00FF0000) >> 16)));
    _cmd.append((char)((quint8)((_steps & 0x0000FF00) >> 8)));
    _cmd.append((char)((quint8)(_steps & 0x000000FF)));
    sendSerialCommand(_cmd);
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
            _data.append(uint(255));
        }
        m_serial_port->write(_data);
        if(_data[0].operator!=(uint(GaryCommands::TBI_CMD_KEEP_ALIVE)))
        {
            //qDebug() << "Gary::sendSerialCommand() Sending Command to Controller: " << _data.toHex(',');
        }

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
processControllerInput()
Slot
Description:
  Function to process in Joystick input from the microcontroller
 **************************************************************/
void Gary::processControllerInput()
{

    //A New Control Event has Happened Set the appropriate controller variables
    //Start The Autodelay timer
    GaryControllerQMLSignals _gcs;
    _gcs.m_is_autorepeat = false;
    switch(m_controller_status)
    {
    case GaryControllerStatus::TBI_CONTROL_NONE: //It should never hit here.
        //Stop the autorepeat timers
        m_controller_autorepeat_timer->stop();
        m_controller_autorepeatdelay_timer->stop();

        //Fire The Release Event if needed
        if(m_controller_event_fired == true)
        {
            GaryControllerQMLSignals _gcs;
            _gcs.m_is_autorepeat = false;
            m_controller_event_fired = false;
            switch(m_last_controller_status)
            {
            case GaryControllerStatus::TBI_CONTROL_NONE:
                break;
            case GaryControllerStatus::TBI_CONTROL_JOYUP:
                _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_JOYUP_RELEASED;
                emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
                break;
            case GaryControllerStatus::TBI_CONTROL_JOYDOWN:
                _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_JOYDOWN_RELEASED;
                emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
                break;
            case GaryControllerStatus::TBI_CONTROL_JOYLEFT:
                _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_JOYLEFT_RELEASED;
                emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
                break;
            case GaryControllerStatus::TBI_CONTROL_JOYRIGHT:
                _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_JOYRIGHT_RELEASED;
                emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
                break;
            case GaryControllerStatus::TBI_CONTROL_GREENBTN:
                _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_GREENBTN_RELEASED;
                emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
                break;
            case GaryControllerStatus::TBI_CONTROL_REDBTN:
                _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_REDBTN_RELEASED;
                emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
                break;
            case GaryControllerStatus::TBI_CONTROL_BLACKBTN:
                _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_BLACKBTN_RELEASED;
                emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
                break;
            }
            m_last_controller_status = GaryControllerStatus::TBI_CONTROL_NONE;
        }
        break;
    case GaryControllerStatus::TBI_CONTROL_JOYUP:
        if(!m_controller_event_fired)
        {
            m_controller_autorepeatdelay_timer->start();
            m_controller_event_fired = true;
            m_controller_status = GaryControllerStatus::TBI_CONTROL_JOYUP;
            _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_JOYUP_PRESSED;
            emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
            m_controller_autorepeatdelay_timer->start();
            m_last_controller_status = m_controller_status;
        }
        break;
    case GaryControllerStatus::TBI_CONTROL_JOYDOWN:
        if(!m_controller_event_fired)
        {
            m_controller_autorepeatdelay_timer->start();
            m_controller_event_fired = true;
            m_controller_status = GaryControllerStatus::TBI_CONTROL_JOYDOWN;
            _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_JOYDOWN_PRESSED;
            emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
            m_last_controller_status = m_controller_status;
            m_controller_autorepeatdelay_timer->start();
        }
        break;
    case GaryControllerStatus::TBI_CONTROL_JOYRIGHT:
        if(!m_controller_event_fired)
        {
            m_controller_autorepeatdelay_timer->start();
            m_controller_event_fired = true;
            m_controller_status = GaryControllerStatus::TBI_CONTROL_JOYRIGHT;
            _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_JOYRIGHT_PRESSED;
            emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
            m_last_controller_status = m_controller_status;
            m_controller_autorepeatdelay_timer->start();
        }
        break;
    case GaryControllerStatus::TBI_CONTROL_JOYLEFT:
        if(!m_controller_event_fired)
        {
            m_controller_autorepeatdelay_timer->start();
            m_controller_event_fired = true;
            m_controller_status = GaryControllerStatus::TBI_CONTROL_JOYLEFT;
            _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_JOYLEFT_PRESSED;
            emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
            m_last_controller_status = m_controller_status;
            m_controller_autorepeatdelay_timer->start();
        }
        break;
    case GaryControllerStatus::TBI_CONTROL_GREENBTN:
        if(!m_controller_event_fired)
        {
            m_controller_autorepeatdelay_timer->start();
            m_controller_event_fired = true;
            m_controller_status = GaryControllerStatus::TBI_CONTROL_GREENBTN;
            _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_GREENBTN_PRESSED;
            emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
            m_last_controller_status = m_controller_status;
            m_controller_autorepeatdelay_timer->start();
        }
        break;
    case GaryControllerStatus::TBI_CONTROL_REDBTN:
        if(!m_controller_event_fired)
        {
            m_controller_autorepeatdelay_timer->start();
            m_controller_event_fired = true;
            m_controller_status = GaryControllerStatus::TBI_CONTROL_REDBTN;
            _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_REDBTN_PRESSED;
            emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
            m_last_controller_status = m_controller_status;
            m_controller_autorepeatdelay_timer->start();
        }
        break;
    case GaryControllerStatus::TBI_CONTROL_BLACKBTN:
        if(!m_controller_event_fired)
        {
            m_controller_autorepeatdelay_timer->start();
            m_controller_event_fired = true;
            m_controller_status = GaryControllerStatus::TBI_CONTROL_BLACKBTN;
            _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_BLACKBTN_PRESSED;
            emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
            m_last_controller_status = m_controller_status;
            m_controller_autorepeatdelay_timer->start();
        }
        break;
    }
}

void Gary::onControllerRepeatDelayTimer()
{

    m_controller_autorepeatdelay_timer->stop();
    GaryControllerQMLSignals _gcs;
    _gcs.m_is_autorepeat = true;
    switch(m_controller_status)
    {
    case GaryControllerStatus::TBI_CONTROL_NONE:
        break;
    case GaryControllerStatus::TBI_CONTROL_JOYUP:
        _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_JOYUP_PRESSED;
        emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
        break;
    case GaryControllerStatus::TBI_CONTROL_JOYDOWN:
        _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_JOYDOWN_PRESSED;
        emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
        break;
    case GaryControllerStatus::TBI_CONTROL_JOYLEFT:
        _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_JOYLEFT_PRESSED;
        emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
        break;
    case GaryControllerStatus::TBI_CONTROL_JOYRIGHT:
        _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_JOYRIGHT_PRESSED;
        emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
        break;
    case GaryControllerStatus::TBI_CONTROL_GREENBTN:
        _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_GREENBTN_PRESSED;
        emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
        break;
    case GaryControllerStatus::TBI_CONTROL_REDBTN:
        _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_REDBTN_PRESSED;
        emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
        break;
    case GaryControllerStatus::TBI_CONTROL_BLACKBTN:
        _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_BLACKBTN_PRESSED;
        emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
        break;
    }
    m_controller_autorepeat_timer->start();
}

void Gary::onControllerRepeatTimer()
{
    GaryControllerQMLSignals _gcs;
    _gcs.m_is_autorepeat = true;
    switch(m_controller_status)
    {
    case GaryControllerStatus::TBI_CONTROL_NONE:
        break;
    case GaryControllerStatus::TBI_CONTROL_JOYUP:
        _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_JOYUP_PRESSED;
        emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
        break;
    case GaryControllerStatus::TBI_CONTROL_JOYDOWN:
        _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_JOYDOWN_PRESSED;
        emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
        break;
    case GaryControllerStatus::TBI_CONTROL_JOYLEFT:
        _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_JOYLEFT_PRESSED;
        emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
        break;
    case GaryControllerStatus::TBI_CONTROL_JOYRIGHT:
        _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_JOYRIGHT_PRESSED;
        emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
        break;
    case GaryControllerStatus::TBI_CONTROL_GREENBTN:
        _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_GREENBTN_PRESSED;
        emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
        break;
    case GaryControllerStatus::TBI_CONTROL_REDBTN:
        _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_REDBTN_PRESSED;
        emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
        break;
    case GaryControllerStatus::TBI_CONTROL_BLACKBTN:
        _gcs.m_controller_event = GaryControllerQMLSignals::TBI_CONTROLLER_BLACKBTN_PRESSED;
        emit garyControllerFired(_gcs.m_controller_event, _gcs.m_is_autorepeat);
        break;
    }
}

void Gary::onKeepAliveTimer()
{
    QByteArray _cmd;
    _cmd.append(char(GaryCommands::TBI_CMD_KEEP_ALIVE));
    sendSerialCommand(_cmd);
}

/**************************************************************
readSerial()
Slot
Description:
  Handles the readyRead signal from the Serial Port.
 **************************************************************/
void Gary::readSerial()
{
    //The Microcontroller Will send 23 Byte Packets.
    //It Will Not Send Another One Until the Packet Has Been Acknowledged.

    QByteArray _data = m_serial_port->readAll();
    m_recieved_serial.append(_data);
    //See the Aruino Sketch For the Status Buffer Structure

    if(m_recieved_serial.size() == 23)
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

        m_controller_status = (GaryControllerStatus::ControllerStatus_t) ((quint8)m_recieved_serial[22]);
        //qDebug() << "Controller Status: " << m_controller_status;
        //qDebug() << "m_recieved_serial=" << m_recieved_serial.toHex(',');
        m_recieved_serial.clear();
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
        processControllerInput();
        acknowledgeStatusPacket();
    }

}
//--------------------------------------------------------------







