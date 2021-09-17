#ifndef TBICLASS_GARYBASETYPES_H
#define TBICLASS_GARYBASETYPES_H

#include <QObject>
#include <QQuickItem>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QByteArray>
#include "tbicore_constants.h"
#include <QTimer>
#include <QVariant>

/**************************************************************
GaryHomingStatus
Description:
  Gary is the Photographer.
  Gary Directly Interacts With Mary.

  A class to encapsulate the requirments for keeping track
  of the homing status codes of the Microcontroller.
 **************************************************************/
class GaryHomingStatus: public QObject
{
    Q_OBJECT
public:
    GaryHomingStatus() : QObject() {}
    enum HomingStatus_t {TBI_HOMING_STATUS_HOMED = 0x01,
                         TBI_HOMING_STATUS_NOT_HOMED = 0x02};
    Q_ENUMS(HomingStatus_t)
    static void declareQML()
    {
        qmlRegisterType<GaryHomingStatus>("tbi.vision.components", 1, 0, "GaryHomingStatus");
    }
    HomingStatus_t m_homing_status;
};

/**************************************************************
GaryControlErrorCode
Description:
  A class to encapsulate the requirments for keeping track
  of the error state codes of the Microcontroller.
 **************************************************************/
class GaryControlErrorCode: public QObject
{
    Q_OBJECT
public:
    GaryControlErrorCode() : QObject() {}
    enum ControlErrorCode_t {TBI_ERROR_ALL_OK = 0x01,
                             TBI_ERROR_LIMIT_TRIPPED = 0x02,
                             TBI_ERROR_CANNOT_PROCESS_LAST_MOVEMENT = 0x03,
                             TBI_ERROR_LOST_SERIAL_NO_DTR = 0x04};
    Q_ENUMS(ControlErrorCode_t)
    static void declareQML()
    {
        qmlRegisterType<GaryControlErrorCode>("tbi.vision.components", 1, 0, "GaryControlErrorCode");
    }
};

/**************************************************************
GaryOperationStatus
Description:
  A class to encapsulate the requirments for keeping track
  of the operation status codes of the Microcontroller.
 **************************************************************/
class GaryOperationStatus: public QObject
{
    Q_OBJECT
public:
    GaryOperationStatus() : QObject() {}
    enum OperationStatus_t {TBI_OPERATION_OK = 0x01,
                            TBI_OPERATION_ERROR = 0x02};
    Q_ENUMS(OperationiStatus_t)
    static void declareQML()
    {
        qmlRegisterType<GaryOperationStatus>("tbi.vision.components", 1, 0, "GaryOperationStatus");
    }
    OperationStatus_t m_operation_status;
};

/**************************************************************
GaryLimitSwitch
Description:
  A class to encapsulate the requirments for keeping track
  of the limit switch status codes of the motor axis.
 **************************************************************/
class GaryLimitSwitch: public QObject
{
    Q_OBJECT
public:
    GaryLimitSwitch() : QObject() {}
    enum LimitSwitchState_t {TBI_LIMIT_SWITCH_STATE_TRIPPED = 0x01,
                             TBI_LIMIT_SWITCH_STATE_OK = 0x02};
    Q_ENUMS(LimitSwitchState_t)
    static void declareQML()
    {
        qmlRegisterType<GaryLimitSwitch>("tbi.vision.components", 1, 0, "GaryLimitSwitch");
    }
    LimitSwitchState_t m_limit_switch_state;
};

/**************************************************************
GaryControlMode
Description:
  A class to encapsulate the requirments for keeping track
  of the control modes of the microcontroller
 **************************************************************/
class GaryControlMode: public QObject
{
    Q_OBJECT
public:
    GaryControlMode() : QObject() {}
    enum ControlMode_t {TBI_CONTROL_MODE_MANUAL_MODE = 0x01,
                        TBI_CONTROL_MODE_FULLAUTO_MODE = 0x02,
                        TBI_CONTROL_MODE_HEIGHTONLY = 0x03,
                        TBI_CONTROL_MODE_MOTORCALIBRATION = 0x04,
                        TBI_CONTROL_MODE_AWAITING_MANUAL = 0x50,
                        TBI_CONTROL_MODE_AWAITING_FULLAUTO = 0x51,
                        TBI_CONTROL_MODE_AWAITING_HEIGHTONLY = 0x52,
                        TBI_CONTROL_MODE_AWAITING_NONE = 0x99};
    Q_ENUMS(ControlMode_t)
    static void declareQML()
    {
        qmlRegisterType<GaryControlMode>("tbi.vision.components", 1, 0, "GaryControlMode");
    }
    ControlMode_t m_control_mode;

};

/**************************************************************
GaryMotionStatus
Description:
  A class to encapsulate the requirments for keeping track
  of the motion status codes of the Microcontroller.
 **************************************************************/
class GaryMotionStatus: public QObject
{
    Q_OBJECT

public:
    GaryMotionStatus() : QObject() {}
    enum MotionStatus_t {TBI_MOTION_STATUS_IDLE = 0x00,
                         TBI_MOTION_STATUS_MOVING = 0x01,
                         TBI_MOTION_STATUS_JOGGING = 0x02,
                         TBI_MOTION_STATUS_GENERAL_ERROR = 0x03,
                         TBI_MOTION_STATUS_LIMIT_TRIPPED = 0x04,
                         TBI_MOTION_STATUS_HOMING = 0x05,
                         TBI_MOTION_STATUS_AWAITING_IDLE = 0x50,
                         TBI_MOTION_STATUS_AWAITING_MOVING =0x51,
                         TBI_MOTION_STATUS_AWAITING_JOGGING = 0x52,
                         TBI_MOTION_STATUS_AWAITING_NONE = 0x99};

    Q_ENUMS(MotionStatus_t)
    static void declareQML()
    {
        qmlRegisterType<GaryMotionStatus>("tbi.vision.components", 1, 0, "GaryMotionStatus");
    }
    MotionStatus_t m_motion_status;
};

/**************************************************************
GaryCommands
Description:
  A class to encapsulate the requirments for keeping track
  of the serial command codes for the Microcontroller.
 **************************************************************/
class GaryCommands : public QObject
{

    Q_OBJECT
public:

    GaryCommands() : QObject() {}

    enum SerialCommand_t{ TBI_CMD_DELIMITER_CR = 0x0D,
                          TBI_CMD_DELMIITER_LF = 0x0A,
                          TBI_CMD_STOP_MOVEMENT = 0x01,
                          TBI_CMD_JOG_UP = 0x02,
                          TBI_CMD_JOG_DOWN = 0x03,
                          TBI_CMD_JOG_LEFT = 0x04,
                          TBI_CMD_JOG_RIGHT = 0x05,
                          TBI_CMD_ACKNOWLEDGE_STATUS_PACKET = 0x06,
                          TBI_CMD_HOME_X = 0x07,
                          TBI_CMD_HOME_Z = 0x08,
                          TBI_CMD_TOGGLE_LASER_POWER = 0x09,
                          TBI_CMD_MOVEXSTEPS = 0x0B,
                          TBI_CMD_MOVEZSTEPS = 0x0C,
                          TBI_CMD_SETCONTROLMODE = 0x0E,
                          TBI_CMD_SET_CALIBRATION_SPEED = 0x0F,
                          TBI_CMD_SET_OPERATION_SPEED = 0x10,
                          TBI_CMD_DO_MOTOR_CALIBRATION = 0x11,
                          TBI_CMD_CONTINUE_MOTOR_CALIBRATION = 0x12,
                          TBI_CMD_KEEP_ALIVE = 0x13};

    Q_ENUMS(SerialCommand_t)

    static void declareQML()
    {
        qmlRegisterType<GaryCommands>("tbi.vision.components", 1, 0, "GaryCommandTypes");
    }
};

/**************************************************************
GaryCommands
Description:
  A class to encapsulate the requirments for keeping track
  of the serial command codes for the Microcontroller.
 **************************************************************/
class GaryLaserStatus : public QObject
{

    Q_OBJECT
public:

    GaryLaserStatus() : QObject() {}

    enum LaserStatus_t{ TBI_LASER_STATUS_OFF = 0x00,
                        TBI_LASER_STATUS_ON = 0x01};

    Q_ENUMS(SerialCommand_t)

    static void declareQML()
    {
        qmlRegisterType<GaryLaserStatus>("tbi.vision.components", 1, 0, "GaryLaserStatus");
    }
};

/**************************************************************
GaryMotorCalibrationCycleStatus
Description:
  A class to encapsulate the GaryMotorCalibrationCycleStatus
 **************************************************************/
class GaryMotorCalibrationCycleStatus : public QObject
{

    Q_OBJECT
public:

    GaryMotorCalibrationCycleStatus() : QObject() {}

    enum MotorCalibration_Cycle_t{ TBI_MOTORCAL_OFF = 0x00,
                                   TBI_MOTORCAL_GETPNT1PAUSE = 0x01,
                                   TBI_MOTORCAL_MAKINGFIRSTMOVE = 0x02,
                                   TBI_MOTORCAL_GETPNT2PAUSE = 0x03,
                                   TBI_MOTORCAL_FINISHING = 0x04};
    Q_ENUMS(MotorCalibration_Cycle_t)

    static void declareQML()
    {
        qmlRegisterType<GaryMotorCalibrationCycleStatus>("tbi.vision.components", 1, 0, "GaryMotorCalibrationCycleStatus");
    }
};


#endif // TBICLASS_GARYBASETYPES_H
