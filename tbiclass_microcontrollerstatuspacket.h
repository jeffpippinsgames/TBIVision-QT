#ifndef GARYCONTROLLERSTATUSPACKET_H
#define GARYCONTROLLERSTATUSPACKET_H

#include <QObject>
#include <QQuickItem>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QByteArray>
#include "tbicore_constants.h"
#include <QTimer>
#include <QVariant>
#include "tbiclass_garybasetypes.h"

/**************************************************************
GaryControllerStatusPacket
Description:
  Gary Interacts with the motion Controller.


  A class to encapsulate the requirments for keeping track
  of the homing status codes of the Microcontroller.
 **************************************************************/
class MicroControllerStatusPacket: public QObject
{
    Q_OBJECT

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

public:
    MicroControllerStatusPacket(QObject *parent = nullptr);
    quint32 getcurrentControlStatusGUID(){return m_status_packet_guid;}
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
    quint8 getJoystickFlags(){return m_joystick_flags;}
    qint32 getXPosition(){return m_x_position;}
    qint32 getZPosition(){return m_z_position;}
    void setAwaitingXMotionStatus(GaryMotionStatus::MotionStatus_t _status);
    void setAwaitingZMotionStatus(GaryMotionStatus::MotionStatus_t _status);
    void setAwatingControlMode(GaryControlMode::ControlMode_t _mode);
    void fillDataFromSerialRead(QByteArray &_data);
    void copyStatusPacketTo(MicroControllerStatusPacket &_packet);

    bool isXMotionStatusIdle();
    bool isZMotionStatusIdle();

private:
    quint32 m_status_packet_guid;
    GaryMotionStatus::MotionStatus_t m_x_motion_status;
    GaryMotionStatus::MotionStatus_t m_z_motion_status;
    GaryControlMode::ControlMode_t m_control_mode;
    GaryHomingStatus::HomingStatus_t m_x_homing_status;
    GaryHomingStatus::HomingStatus_t m_z_homing_status;
    GaryLimitSwitch::LimitSwitchState_t m_x_axis_limit;
    GaryLimitSwitch::LimitSwitchState_t m_z_axis_limit;
    GaryOperationStatus::OperationStatus_t m_operation_status;
    GaryLaserStatus::LaserStatus_t m_laser_status;
    quint8 m_joystick_flags;
    GaryMotorCalibrationCycleStatus::MotorCalibration_Cycle_t m_motor_calib_cycle;
    quint32 m_x_position;
    quint32 m_z_position;

    GaryMotionStatus::MotionStatus_t m_awaiting_x_motion_status;
    GaryMotionStatus::MotionStatus_t m_awaiting_z_motion_status;
    GaryControlMode::ControlMode_t m_awaiting_control_mode;

    const bool m_show_debug = false;

signals:
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

    void joystickFlagsUpdated(quint8 _flags);
};

#endif // GARYCONTROLLERSTATUSPACKET_H
