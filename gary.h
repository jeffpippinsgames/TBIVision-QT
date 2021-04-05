#ifndef GARY_H
#define GARY_H

#include <QObject>
#include <QQuickItem>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class GaryHomingStatus: public QObject
{
    Q_OBJECT
public:
    GaryHomingStatus() : QObject() {}
    enum HomingStatus_t {TBI_HOMING_STATUS_HOMED = 0x01, TBI_HOMING_STATUS_NOT_HOMED = 0x02};
    Q_ENUMS(HomingStatus_t)
    static void declareQML()
    {
        qmlRegisterType<GaryHomingStatus>("tbi.vision.components", 1, 0, "GaryHomingStatus");
    }
};

//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
class GaryOperationStatus: public QObject
{
    Q_OBJECT
public:
    GaryOperationStatus() : QObject() {}
    enum OperationStatus_t {TBI_OPERATION_OK = 0x01, TBI_OPERATION_ERROR = 0x02};
    Q_ENUMS(OperationiStatus_t)
    static void declareQML()
    {
        qmlRegisterType<GaryOperationStatus>("tbi.vision.components", 1, 0, "GaryOperationStatus");
    }
};

//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
class GaryLimitSwitch: public QObject
{
    Q_OBJECT
public:
    GaryLimitSwitch() : QObject() {}
    enum LimitSwitchState_t {TBI_LIMIT_SWITCH_STATE_TRIPPED = 0x01, TBI_LIMIT_SWITCH_STATE_OK = 0x02};
    Q_ENUMS(LimitSwitchState_t)
    static void declareQML()
    {
        qmlRegisterType<GaryLimitSwitch>("tbi.vision.components", 1, 0, "GaryLimitSwitch");
    }
};

//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
class GaryControlMode: public QObject
{
    Q_OBJECT
public:
    GaryControlMode() : QObject() {}
    enum ControlMode_t {TBI_CONTROL_MODE_MANUAL_MODE = 0x01, TBI_CONTROL_MODE_PID_MODE = 0x02};
    Q_ENUMS(ControlMode_t)
    static void declareQML()
    {
        qmlRegisterType<GaryControlMode>("tbi.vision.components", 1, 0, "GaryControlMode");
    }

};

//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
class GaryMotionStatus: public QObject
{
    Q_OBJECT

public:
    GaryMotionStatus() : QObject() {}
    enum MotionStatus_t {TBI_MOTION_STATUS_IDLE = 0x00,
                         TBI_MOTION_STATUS_MOVING_PID = 0x01,
                         TBI_MOTION_STATUS_JOGGING = 0x02,
                         TBI_MOTION_STATUS_GENERAL_ERROR = 0x03,
                         TBI_MOTION_STATUS_LIMIT_TRIPPED = 0x04};
    Q_ENUMS(MotionStatus_t)
    static void declareQML()
    {
        qmlRegisterType<GaryMotionStatus>("tbi.vision.components", 1, 0, "GaryMotionStatus");
    }
};

//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
class GaryCommands : public QObject
{

    Q_OBJECT
   public:

    GaryCommands() : QObject() {}

    enum SerialCommand_t{ TBI_CMD_DELIMITER_CR = 0x0D,
                          TBI_CMD_DELMIITER_LF = 0x0A,
                          TBI_CMD_SEND_WATCHDOG = 0x01,
                          TBI_CMD_STOP_MOVEMENT = 0x02,
                          TBI_CMD_JOG_UP = 0x03,
                          TBI_CMD_JOG_DOWN = 0x04,
                          TBI_CMD_JOG_LEFT = 0x05,
                          TBI_CMD_JOG_RIGHT = 0x06,
                          TBI_SET_X_ERROR = 0x07,
                          TBI_SET_Z_ERROR = 0x08,
                          TBI_CMD_SEND_X_ERROR = 0x09,
                          TBI_CMD_SEND_Z_ERROR = 0x0B,
                          TBI_CMD_SEND_X_LIMIT_TRIPPED = 0x0C,
                          TBI_CMD_SEND_Z_LIMIT_TRIPPED = 0x0E,
                          TBI_CMD_SEND_X_POSITIION = 0x0F,
                          TBI_CMD_SEND_Z_POSITION = 0x10,
                          TBI_CMD_SET_X_POSITION = 0x11,
                          TBI_CMD_SET_Z_POSITION = 0x12,
                          TBI_CMD_SEND_X_LIMIT_STATUS = 0x13,
                          TBI_CMD_SEND_Z_LIMIT_STATUS = 0x14,
                          TBI_CMD_SET_KP_XCONTROL = 0x15,
                          TBI_CMD_SET_KD_XCONTROL = 0x16,
                          TBI_CMD_SET_KI_XCONTROL = 0x17,
                          TBI_CMD_SEND_KP_XCONTROL = 0x18,
                          TBI_CMD_SEND_KD_XCONTROL = 0x19,
                          TBI_CMD_SEND_KI_XCONTROL = 0x1A,
                          TBI_CMD_PID_ON = 0x1B,
                          TBI_CMD_PID_OFF = 0x1C,
                          TBI_CMD_SEND_STATUS = 0x1D,
                          TBI_CMD_HOME_X = 0x1E,
                          TBI_CMD_HOME_Z = 0x1F,
                          TBI_CMD_SET_KP_ZCONTROL = 0x20,
                          TBI_CMD_SET_KD_ZCONTROL = 0x21,
                          TBI_CMD_SET_KI_ZCONTROL = 0x22,
                          TBI_CMD_SEND_KP_ZCONTROL = 0x23,
                          TBI_CMD_SEND_KD_ZCONTROL = 0x24,
                          TBI_CMD_SEND_KI_ZCONTROL = 0x25};

    Q_ENUMS(SerialCommand_t)

    static void declareQML()
    {
        qmlRegisterType<GaryCommands>("tbi.vision.components", 1, 0, "GaryCommandTypes");
    }
};

//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
class Gary : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool jogStop READ jogStop WRITE setJogStop NOTIFY jogStopChanged)
    Q_PROPERTY(bool jogUp READ jogUp WRITE setJogUp NOTIFY jogUpChanged)
    //Q_PROPERTY(GaryMotionStatus motionstatus READ motionstatus WRITE setMotionStatus )





public:
    explicit Gary(QObject *parent = nullptr);
    ~Gary();
    static void declareQML();

    QString name() const;
    bool jogStop() const;
    bool jogUp() const;

private:
    QString m_name;
    QSerialPortInfo *m_serial_info;
    QSerialPort *m_serial_port;
    GaryMotionStatus m_motion_status;
    GaryControlMode m_control_mode;
    GaryHomingStatus m_homing_status;
    GaryLimitSwitch m_x_axis_limit;
    GaryLimitSwitch m_z_axis_limit;
    GaryOperationStatus m_operation_status;

    bool m_jogStop;
    bool m_jogUp;
    const quint16 m_teensy_vendorID = 0x16C0;
    const quint16 m_teensy_productID = 0x0483;

    bool findOpenTeensy();
    void setName(const QString &_name);
    void setJogStop(const bool _jogstop);
    void setJogUp(const bool _jogup);

    void sendSerialCommand(QByteArray &_data);


signals:
    void nameChanged(void);
    void jogStopChanged(void);
    void jogUpChanged(void);

};

#endif // GARY_H
