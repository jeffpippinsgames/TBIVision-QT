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
                        TBI_CONTROL_MODE_HEIGHTONLY = 0x03};
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
                         TBI_MOTIION_STATUS_HOMING = 0x05};
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
                          TBI_CMD_SEND_STATUS = 0x06,
                          TBI_CMD_HOME_X = 0x07,
                          TBI_CMD_HOME_Z = 0x08,
                          TBI_CMD_TOGGLE_LASER_POWER = 0x09,
                          TBI_CMD_MOVEXSTEPS = 0x0B,
                          TBI_CMD_MOVEZSTEPS = 0x0C};

    Q_ENUMS(SerialCommand_t)

    static void declareQML()
    {
        qmlRegisterType<GaryCommands>("tbi.vision.components", 1, 0, "GaryCommandTypes");
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
    Q_OBJECT //QOBJECT MACRO
    Q_PROPERTY(GaryMotionStatus* xMotionStatus READ getXMotionStatus WRITE setXMotionStatus NOTIFY xMotionStatusChanged)
    Q_PROPERTY(GaryMotionStatus* zMotionStatus READ getZMotionStatus WRITE setYMotionStatus NOTIFY yMotionStatusChanged)
    Q_PROPERTY(GaryControlMode* controlMode READ getControlMode WRITE setControlMode NOTIFY controlModeChanged)
    Q_PROPERTY(GaryHomingStatus* homingStatus READ getHomingStatus WRITE setHomingStatus NOTIFY homingStatusChanged)
    Q_PROPERTY(GaryLimitSwitch* xLimitSwitch READ getXLimitSwitch WRITE setXLimitSwitch NOTIFY xLimitSwitchChanged)
    Q_PROPERTY(GaryLimitSwitch* zLimitSwitch READ getZLimitSwitch WRITE setZLimitSwitch NOTIFY zLimitSwitchChanged)
    Q_PROPERTY(GaryOpererationStatus* operationStatus READ getOperationStatus WRITE setOperationStatus NOTIFY operationStatusChanged)
    Q_PROPERTY(qint32 xPosition READ getXPosition WRITE setXPosition NOTIFY xPositionChanged)
    Q_PROPERTY(qint32 zPosition READ getZPosition WRITE setZPosition NOTIFY zPositionChanged)
    //------------------------------------------------------------

public:

    //Public Constructors, Destructors and Init Methods-----------
    explicit Gary(QObject *parent = nullptr);
    ~Gary();
    static void declareQML();
    //------------------------------------------------------------

    //PropertySetMethods
    void setMotionStatus(GaryMotionStatus*_ms);
    void setControlMode(GaryControlMode* _cm);
    void setHomingStatus(GaryHomingStatus*_hs);
    void setXLimitSwitch(GaryLimitSwitch *_ls);
    void setZLimitSwitch(GaryLimitSwitch *_ls);
    void setOperationStatus(GaryOperationStatus *_os);
    void setXPosition(qint32 _x_pos);
    void setZPosition(qint32 _z_pos);
    //------------------------------------------------------------
    //Property Read Methods
    GaryMotionStatus* motionStatus(){return m_motion_status;}
    GaryControlMode* controlMode(){return m_control_mode;}
    GaryHomingStatus* homingStatus(){return m_homing_status;}
    GaryLimitSwitch* xLimitSwitch(){return m_x_axis_limit;}
    GaryLimitSwitch* zLimitSwitch(){return m_z_axis_limit;}
    int getOperationStatus(){return m_operation_status;}
    float xPosition(){return m_x_position;}
    //------------------------------------------------------------

    //Public Send Command Methods----------------------------------------------
    Q_INVOKABLE void sendStopMovement();
    Q_INVOKABLE void sendJogUp();
    Q_INVOKABLE void sendJogDown();
    Q_INVOKABLE void sendJogLeft();
    Q_INVOKABLE void sendJogRight();

    Q_INVOKABLE void sendToggleLaserPower();
    //------------------------------------------------------------

private:
    //Private Data Members ----------------------------------------
    QByteArray m_recieved_serial;
    const quint16 m_teensy32_vendorID = 0x16C0;
    const quint16 m_teensy32_productID = 0x0483;
    const quint16 m_arduino_uno_vendorID = 0x2341;
    const quint16 m_arduino_uno_productID =0x0043;
    QSerialPortInfo *m_serial_info;
    QSerialPort *m_serial_port;
    GaryMotionStatus *m_x_motion_status;
    GaryMotionStatus *m_z_motion_status;
    GaryControlMode *m_control_mode;
    GaryHomingStatus *m_homing_status;
    GaryLimitSwitch *m_x_axis_limit;
    GaryLimitSwitch *m_z_axis_limit;
    int m_operation_status;
    float m_x_position;
    //-------------------------------------------------------------

    //Private Methods----------------------------------------------
    bool findOpenTeensy();
    bool findOpenArduinoUno();
    void sendSerialCommand(QByteArray &_data);
    //--------------------------------------------------------------

    //Signals-------------------------------------------------------
signals:
    void aboutToDestroy();
    void completed();
    void motionStatusChanged();
    void controlModeChanged();
    void homingStatusChanged();
    void xLimitSwitchChanged();
    void zLimitSwitchChanged();
    void operationStatusChanged();
    void xPositionChanged();
    //--------------------------------------------------------------

public slots:
    void serialError(QSerialPort::SerialPortError _error);
    void readSerial();
    void autoMoveXAxis(qint32 _steps);
    void autoMoveZAxis(qint32 _steps);


};

#endif // TBICORE_GARY_H
