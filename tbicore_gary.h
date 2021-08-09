/*
QML Import Type.
import "tbi.vision.components" 1 0
 */

#ifndef TBICORE_GARY_H
#define TBICORE_GARY_H

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
                        TBI_CONTROL_MODE_MOTORCALIBRATION = 0x04};
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
                         TBI_MOTION_STATUS_HOMING = 0x05};
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

/**************************************************************
GaryControllerStatus
Description:
  A class to encapsulate the GaryControllerStatus
 **************************************************************/
class GaryControllerStatus : public QObject
{

    Q_OBJECT
public:

    GaryControllerStatus() : QObject() {}

    enum ControllerStatus_t{TBI_CONTROL_NONE = 0x00,
                            TBI_CONTROL_JOYUP = 0x01,
                            TBI_CONTROL_JOYDOWN = 0x02,
                            TBI_CONTROL_JOYLEFT = 0x03,
                            TBI_CONTROL_JOYRIGHT = 0x04,
                            TBI_CONTROL_GREENBTN = 0x05,
                            TBI_CONTROL_REDBTN = 0x06,
                            TBI_CONTROL_BLACKBTN = 0x07};
    Q_ENUMS(ControllerStatus_t)

    static void declareQML()
    {
        qmlRegisterType<GaryControllerStatus>("tbi.vision.components", 1, 0, "GaryControllerStatus");
    }
};

/**************************************************************
GaryControllerQMLSignals
Description:
  A class to encapsulate the GaryControllerStatus
 **************************************************************/
class GaryControllerQMLSignals : public QObject
{
    Q_OBJECT
public:
    GaryControllerQMLSignals() : QObject() {}

    enum ControllerQMLSignal_t{TBI_CONTROLLER_JOYUP_PRESSED = 0x00,
                               TBI_CONTROLLER_JOYUP_RELEASED = 0x01,
                               TBI_CONTROLLER_JOYDOWN_PRESSED = 0x02,
                               TBI_CONTROLLER_JOYDOWN_RELEASED = 0x03,
                               TBI_CONTROLLER_JOYLEFT_PRESSED = 0x04,
                               TBI_CONTROLLER_JOYLEFT_RELEASED = 0x05,
                               TBI_CONTROLLER_JOYRIGHT_PRESSED = 0x06,
                               TBI_CONTROLLER_JOYRIGHT_RELEASED = 0x07,
                               TBI_CONTROLLER_GREENBTN_PRESSED = 0x08,
                               TBI_CONTROLLER_GREENBTN_RELEASED = 0x09,
                               TBI_CONTROLLER_REDBTN_PRESSED = 0x0A,
                               TBI_CONTROLLER_REDBTN_RELEASED = 0x0B,
                               TBI_CONTROLLER_BLACKBTN_PRESSED = 0x0C,
                               TBI_CONTROLLER_BLACKBTN_RELEASED = 0x0D};

    Q_ENUMS(ControllerQMLSignal_t)

    Q_PROPERTY(ControllerQMLSignal_t controlevent READ getControlSignal)
    Q_PROPERTY(bool autorepeat READ getAutoRepeat)

    ControllerQMLSignal_t getControlSignal(){return m_controller_event;}
    bool getAutoRepeat(){return m_is_autorepeat;}

    ControllerQMLSignal_t m_controller_event;
    bool m_is_autorepeat;

    static void declareQML()
    {
        qmlRegisterType<GaryControllerQMLSignals>("tbi.vision.components", 1, 0, "GaryControllerQMLSignals");
    }
};

/**************************************************************
Gary

Gary is the Guy Who Moves The Slides.
He's the interface to the motion system.

Description:
  A class to facilitate the operation and communication to the
  microcontroller responsible for motion control of the TBI seam
  tracker.
 **************************************************************/
class Gary : public QObject
{
    //QT Properties and Macros-------------------------------------
    //The Controller Status Objects Are Set From the Microcontroller.
    //They cannot be changed from the QML.
    Q_OBJECT //QOBJECT MACRO
    Q_PROPERTY(quint32 currentControlStatusGUID READ getcurrentControlStatusGUID NOTIFY currentStatusGUIDChanged)
    Q_PROPERTY(GaryMotionStatus::MotionStatus_t xMotionStatus READ getXMotionStatus  NOTIFY xMotionStatusChanged)
    Q_PROPERTY(GaryMotionStatus::MotionStatus_t zMotionStatus READ getZMotionStatus  NOTIFY zMotionStatusChanged)
    Q_PROPERTY(GaryControlMode::ControlMode_t controlMode READ getControlMode  NOTIFY controlModeChanged)
    Q_PROPERTY(GaryLimitSwitch::LimitSwitchState_t xLimitSwitch READ getXLimitSwitch  NOTIFY xLimitSwitchChanged)
    Q_PROPERTY(GaryLimitSwitch::LimitSwitchState_t zLimitSwitch READ getZLimitSwitch  NOTIFY zLimitSwitchChanged)
    Q_PROPERTY(GaryOperationStatus::OperationStatus_t operationStatus READ getOperationStatus  NOTIFY operationStatusChanged)
    Q_PROPERTY(GaryLaserStatus::LaserStatus_t laserStatus READ getLaserStatus  NOTIFY laserStatusChanged)
    Q_PROPERTY(GaryHomingStatus::HomingStatus_t xHomingStatus READ getXHomingStatus NOTIFY xHomingStatusChanged)
    Q_PROPERTY(GaryHomingStatus::HomingStatus_t zHomingStatus READ getZHomingStatus  NOTIFY zHomingStatusChanged)
    Q_PROPERTY(qint32 xPosition READ getXPosition NOTIFY xPositionChanged)
    Q_PROPERTY(qint32 zPosition READ getZPosition NOTIFY zPositionChanged)
    Q_PROPERTY(GaryMotorCalibrationCycleStatus::MotorCalibration_Cycle_t motorCalibrationCycle READ getMotorCalibrationCycle NOTIFY motorCalibrationCycleChanged)
    //------------------------------------------------------------

public:

    //Public Constructors, Destructors and Init Methods-----------
    explicit Gary(QObject *parent = nullptr);
    ~Gary();
    static void declareQML();

    //------------------------------------------------------------


    //------------------------------------------------------------
    //Property Read Methods
    quint32 getcurrentControlStatusGUID(){return m_last_status_guid;}
    GaryMotionStatus::MotionStatus_t getXMotionStatus(){return m_x_motion_status;}
    GaryMotionStatus::MotionStatus_t getZMotionStatus(){return m_z_motion_status;}
    GaryControlMode::ControlMode_t getControlMode(){return m_control_mode;}
    GaryHomingStatus::HomingStatus_t getXHomingStatus(){return m_x_homing_status;}
    GaryHomingStatus::HomingStatus_t getZHomingStatus(){return m_z_homing_status;}
    GaryLimitSwitch::LimitSwitchState_t getXLimitSwitch(){return m_x_axis_limit;}
    GaryLimitSwitch::LimitSwitchState_t getZLimitSwitch(){return m_z_axis_limit;}
    GaryOperationStatus::OperationStatus_t getOperationStatus(){return m_operation_status;}
    GaryLaserStatus::LaserStatus_t getLaserStatus(){return m_laser_status;}
    GaryMotorCalibrationCycleStatus::MotorCalibration_Cycle_t getMotorCalibrationCycle(){return m_motor_calib_cycle;}
    qint32 getXPosition(){return m_x_position;}
    qint32 getZPosition(){return m_z_position;}
    //------------------------------------------------------------

    //Public Methods to Control the MicroController
    Q_INVOKABLE void sendStopMovement();
    Q_INVOKABLE void sendJogUp();
    Q_INVOKABLE void sendJogDown();
    Q_INVOKABLE void sendJogLeft();
    Q_INVOKABLE void sendJogRight();
    Q_INVOKABLE void toggleLaserPower();
    Q_INVOKABLE void autoMoveXAxis(qint32 _steps);
    Q_INVOKABLE void autoMoveZAxis(qint32 _steps);
    Q_INVOKABLE void acknowledgeStatusPacket();
    Q_INVOKABLE void setControllerToCalibrationSpeed();
    Q_INVOKABLE void setControllerToOperationSpeed();
    Q_INVOKABLE void cycleControlModes();
    Q_INVOKABLE void setControlModeToFullAuto();
    Q_INVOKABLE void setControlModeToHeightOnly();
    Q_INVOKABLE void setControlModeToManual();
    Q_INVOKABLE void setControlModeToMotorCalibration();
    Q_INVOKABLE void setControlMode(GaryControlMode::ControlMode_t _mode);
    Q_INVOKABLE bool isStatusGUIDDifferentFromLastCompare();
    Q_INVOKABLE void sendProceedNextMotorPhase();
    Q_INVOKABLE void startMotorCalibration(qint32 _steps);

    //------------------------------------------------------------

private:
    //Serial Port Variables
    QByteArray m_recieved_serial;
    const quint16 m_teensy32_vendorID = 0x16C0;
    const quint16 m_teensy32_productID = 0x0483;
    const quint16 m_arduino_uno_vendorID = 0x2341;
    const quint16 m_arduino_uno_productID =0x0043;
    QSerialPortInfo *m_serial_info;
    QSerialPort *m_serial_port;

    //Controller State Variables
    GaryMotionStatus::MotionStatus_t m_x_motion_status;
    GaryMotionStatus::MotionStatus_t m_z_motion_status;
    GaryControlMode::ControlMode_t m_control_mode;
    GaryHomingStatus::HomingStatus_t m_x_homing_status;
    GaryHomingStatus::HomingStatus_t m_z_homing_status;
    GaryLimitSwitch::LimitSwitchState_t m_x_axis_limit;
    GaryLimitSwitch::LimitSwitchState_t m_z_axis_limit;
    GaryOperationStatus::OperationStatus_t m_operation_status;
    GaryLaserStatus::LaserStatus_t m_laser_status;
    GaryControllerStatus::ControllerStatus_t m_controller_status;
    GaryControllerStatus::ControllerStatus_t m_last_controller_status;
    QTimer *m_controller_autorepeatdelay_timer;
    QTimer *m_controller_autorepeat_timer;
    bool m_controller_event_fired;

    //Keep Alive Variables
    QTimer *m_microcontroller_heartbeat_timer;

    qint32 m_x_position;
    qint32 m_z_position;
    quint32 m_last_status_guid;
    quint32 m_last_status_guid_compared_by_gary;
    GaryMotorCalibrationCycleStatus::MotorCalibration_Cycle_t m_motor_calib_cycle;
    //-------------------------------------------------------------

    //Private Methods----------------------------------------------
    bool findOpenTeensy();
    bool findOpenArduinoUno();
    void sendSerialCommand(QByteArray &_data);
    void printControlStatusVariables();
    void processControllerInput();



    //--------------------------------------------------------------

    //Signals-------------------------------------------------------
signals:
    void aboutToDestroy();
    void completed();
    void xMotionStatusChanged();
    void zMotionStatusChanged();
    void controlModeChanged();
    void xHomingStatusChanged();
    void zHomingStatusChanged();
    void xLimitSwitchChanged();
    void zLimitSwitchChanged();
    void operationStatusChanged();
    void laserStatusChanged();
    void xPositionChanged();
    void zPositionChanged();
    void currentStatusGUIDChanged();
    void motorCalibrationCycleChanged();
    void garyControllerFired(QVariant controlsignal, QVariant autorepeat);
    //--------------------------------------------------------------

public slots:
    void readSerial();
    void onControllerRepeatDelayTimer();
    void onControllerRepeatTimer();
    void onKeepAliveTimer();



};

#endif // TBICORE_GARY_H
