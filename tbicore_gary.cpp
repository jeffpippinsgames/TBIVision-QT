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
    //Setup The Serial Port Thread
    m_serial_port_controller.moveToThread(&m_serial_port_thread);
    //Make Serial Port Connections
    QObject::connect(&m_serial_port_thread, &QThread::started, &m_serial_port_controller, &SerialPortController::startThread); //Connect Thread Start
    QObject::connect(this, &Gary::quitSerialThread, &m_serial_port_controller, &SerialPortController::quitThread); // Connect The End Thread Loop
    QObject::connect(this, &Gary::sendSerialCommand, &m_serial_port_controller, &SerialPortController::sendSerialCommand);
    QObject::connect(&m_serial_port_controller, &SerialPortController::microControllerPacketReady, &m_micro_status_packet, &MicroControllerStatusPacket::fillDataFromSerialRead); //Micro Status Packet
    QObject::connect(&m_serial_port_controller, &SerialPortController::joystickFlagsReady, &m_joystick, &TBIJoystick::updateTBIJoystickState);
    QObject::connect(&m_serial_port_controller, &SerialPortController::serialPortConnected, &m_micro_status_packet, &MicroControllerStatusPacket::onSerialPortConnected);
    QObject::connect(&m_serial_port_controller, &SerialPortController::serialPortDisconnected, &m_micro_status_packet, &MicroControllerStatusPacket::onSerialPortDisconnected);
    m_serial_port_thread.start();


    if(m_showdebug) qDebug() << "Gary::Gary() Object Created.";
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
    emit quitSerialThread();
    while(m_serial_port_thread.isRunning())
    {
    }
    emit aboutToDestroy();
    if(m_showdebug) qDebug() << "Gary::~Gary() Object Destroyed.";
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
    if(m_showdebug) qDebug() << "Gary::declareQML(): Registered Gary As QML Type.";

}
//--------------------------------------------------------------

void Gary::setRootQMLContextProperties(QQmlApplicationEngine &_engine)
{
    _engine.rootContext()->setContextProperty("MicroControllerStatusPacket", &m_micro_status_packet);
    _engine.rootContext()->setContextProperty("TBIJoystick", &m_joystick);
    if(m_showdebug) qDebug() << "Gary::setRootQMLContextProperties(): Registered Gary QML Context Properties.";
}

/**************************************************************
sendJogStop()
Public, Q_INVOKABLE
Description:
  Public Method that sends the Movement Stop Command to the
  microcontroller
 **************************************************************/
Q_INVOKABLE void Gary::sendStopMovement()
{
    //if(m_micro_status_packet.isXMotionStatusIdle() && m_micro_status_packet.isZMotionStatusIdle()) return;
    QByteArray _cmd;
    _cmd.append(char(GaryCommands::TBI_CMD_STOP_MOVEMENT));
    //m_micro_status_packet.setAwaitingXMotionStatus(GaryMotionStatus::TBI_MOTION_STATUS_IDLE);
    //m_micro_status_packet.setAwaitingZMotionStatus(GaryMotionStatus::TBI_MOTION_STATUS_IDLE);
    if(m_showdebug) qDebug() << "Gary::sendStopMovement(): Sending MicroController Stop Movement Command.";
    m_serial_port_controller.sendSerialCommand(_cmd);

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
    if(!m_micro_status_packet.isZMotionStatusIdle()) return;
    QByteArray _cmd;
    _cmd.append(char(GaryCommands::TBI_CMD_JOG_UP));
    //m_micro_status_packet.setAwaitingZMotionStatus(GaryMotionStatus::TBI_MOTION_STATUS_AWAITING_JOGGING);
    if(m_showdebug) qDebug() << "Gary::sendJogUp(): Sending MicroController JogUp Command.";
    emit sendSerialCommand(_cmd);
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
    if(!m_micro_status_packet.isZMotionStatusIdle()) return;
    QByteArray _cmd;
    _cmd.append(char(GaryCommands::TBI_CMD_JOG_DOWN));
    //m_micro_status_packet.setAwaitingZMotionStatus(GaryMotionStatus::TBI_MOTION_STATUS_AWAITING_JOGGING);
    if(m_showdebug) qDebug() << "Gary::sendJogDown(): Sending MicroController Jog Down Command.";
    emit sendSerialCommand(_cmd);
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
    if(!m_micro_status_packet.isXMotionStatusIdle()) return;
    QByteArray _cmd;
    _cmd.append(char(GaryCommands::TBI_CMD_JOG_LEFT));
    //m_micro_status_packet.setAwaitingXMotionStatus(GaryMotionStatus::TBI_MOTION_STATUS_AWAITING_JOGGING);
    if(m_showdebug) qDebug() << "Gary::sendJogLeft(): Sending MicroController Jog Left Command.";
    emit sendSerialCommand(_cmd);
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
    if(!m_micro_status_packet.isXMotionStatusIdle()) return;
    QByteArray _cmd;
    _cmd.append(char(GaryCommands::TBI_CMD_JOG_RIGHT));
    //m_micro_status_packet.setAwaitingXMotionStatus(GaryMotionStatus::TBI_MOTION_STATUS_AWAITING_JOGGING);
    if(m_showdebug) qDebug() << "Gary::sendJogRight(): Sending MicroController Jog Right Command.";
    emit sendSerialCommand(_cmd);
}


/**************************************************************
autoMoveXAxis()
Public, Q_INVOKABLE
Description:
  Public Method that sends the Move X steps command
 **************************************************************/
void Gary::autoMoveXAxis(qint32 _steps)
{
    if(!m_micro_status_packet.isXMotionStatusIdle()) return;
    QByteArray _cmd;
    _cmd.append((char)GaryCommands::TBI_CMD_MOVEXSTEPS);
    _cmd.append((char*)&_steps, sizeof (_steps));
    //m_micro_status_packet.setAwaitingXMotionStatus(GaryMotionStatus::TBI_MOTION_STATUS_AWAITING_MOVING);
    if(m_showdebug) qDebug() << "Gary::autoMoveXAxis(): Sending MicroController Auto Move X Axis Command.";
    emit sendSerialCommand(_cmd);

}

/**************************************************************
autoMoveZAxis()
Public, Q_INVOKABLE
Description:
  Public Method that sends the Move Z steps command
 **************************************************************/
void Gary::autoMoveZAxis(qint32 _steps)
{
    if(!m_micro_status_packet.isZMotionStatusIdle()) return;

    QByteArray _cmd;
    _cmd.append((char)GaryCommands::TBI_CMD_MOVEZSTEPS);
    _cmd.append((char*)&_steps, sizeof (_steps));
    //m_micro_status_packet.setAwaitingZMotionStatus(GaryMotionStatus::TBI_MOTION_STATUS_AWAITING_MOVING);
    if(m_showdebug) qDebug() << "Gary::autoMoveZAxis(): Sending MicroController Auto Move Z Axis Command.";
    emit sendSerialCommand(_cmd);
}

void Gary::setControllerToCalibrationSpeed()
{
    QByteArray _cmd;
    _cmd.append((char)GaryCommands::TBI_CMD_SET_CALIBRATION_SPEED);
    if(m_showdebug) qDebug() << "Gary::setControllerToCalibrationSpeed(): Sending MicroController Set Calibration Speed Command.";
    emit sendSerialCommand(_cmd);
}

void Gary::setControllerToOperationSpeed()
{
    QByteArray _cmd;
    _cmd.append((char)GaryCommands::TBI_CMD_SET_OPERATION_SPEED);
    if(m_showdebug) qDebug() << "Gary::setControllerToOperationSpeed(): Sending MicroController Operation Speed Command.";
    emit sendSerialCommand(_cmd);
}

void Gary::setControlModeToFullAuto()
{
    QByteArray _cmd;
    _cmd.append((char)GaryCommands::TBI_CMD_SETCONTROLMODE);
    _cmd.append((char)GaryControlMode::TBI_CONTROL_MODE_FULLAUTO_MODE);
    //m_micro_status_packet.setAwatingControlMode(GaryControlMode::TBI_CONTROL_MODE_AWAITING_FULLAUTO);
    if(m_showdebug) qDebug() << "Gary::setControlModeToFullAuto(): Sending MicroController Set Full Auto Mode Command.";
    emit sendSerialCommand(_cmd);
}

void Gary::setControlModeToHeightOnly()
{
    QByteArray _cmd;
    _cmd.append((char)GaryCommands::TBI_CMD_SETCONTROLMODE);
    _cmd.append((char)GaryControlMode::TBI_CONTROL_MODE_HEIGHTONLY);
    if(m_showdebug) qDebug() << "Gary::setControlModeToHeightOnly(): Sending MicroController Set Height Only Mode Command.";
    emit sendSerialCommand(_cmd);
}

void Gary::setControlModeToManual()
{
    QByteArray _cmd;
    _cmd.append((char)GaryCommands::TBI_CMD_SETCONTROLMODE);
    _cmd.append((char)GaryControlMode::TBI_CONTROL_MODE_MANUAL_MODE);
    if(m_showdebug) qDebug() << "Gary::setControlModeToManual(): Sending MicroController Manual Mode Command.";
    emit sendSerialCommand(_cmd);
}

void Gary::setControlModeToMotorCalibration()
{
    QByteArray _cmd;
    _cmd.append((char)GaryCommands::TBI_CMD_SETCONTROLMODE);
    _cmd.append((char)GaryControlMode::TBI_CONTROL_MODE_MOTORCALIBRATION);
    if(m_showdebug) qDebug() << "Gary::setControlModeToMotorCalibration(): Sending MicroController Motor Calibration Mode Command.";
    emit sendSerialCommand(_cmd);
}

void Gary::setControlMode(GaryControlMode::ControlMode_t _mode)
{
    QByteArray _cmd;
    _cmd.append((char)GaryCommands::TBI_CMD_SETCONTROLMODE);
    _cmd.append((char)_mode);
    if(m_showdebug) qDebug() << "Gary::setControlMode(): Sending MicroController Set Control Mode Command.";
    emit sendSerialCommand(_cmd);
}

void Gary::sendProceedNextMotorPhase()
{
    QByteArray _cmd;
    _cmd.append((char)GaryCommands::TBI_CMD_CONTINUE_MOTOR_CALIBRATION);
    if(m_showdebug) qDebug() << "Gary::sendProceedNextMotorPhase(): Sending MicroController Proceed Next Motor Phase Command.";
    emit sendSerialCommand(_cmd);
}

void Gary::startMotorCalibration(qint32 _steps)
{
    QByteArray _cmd;
    _cmd.append((char)GaryCommands::TBI_CMD_CONTINUE_MOTOR_CALIBRATION);
    _cmd.append((char)((quint8)((_steps & 0xFF000000) >> 24)));
    _cmd.append((char)((quint8)((_steps & 0x00FF0000) >> 16)));
    _cmd.append((char)((quint8)((_steps & 0x0000FF00) >> 8)));
    _cmd.append((char)((quint8)(_steps & 0x000000FF)));
    if(m_showdebug) qDebug() << "Gary::startMotorCalibration(): Sending MicroController Continue Motor Calibration Command.";
    emit sendSerialCommand(_cmd);
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
    if(m_showdebug) qDebug() << "Gary::toggleLaserPower): Sending MicroController Toggle Laser Power Command.";
    emit sendSerialCommand(_cmd);
}
//--------------------------------------------------------------





