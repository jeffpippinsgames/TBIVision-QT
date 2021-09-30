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

    Q_PROPERTY(bool serialportconnected READ getSerialPortConnectionStatus NOTIFY serialPortConnectionStatusChanged)
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


    //Get Methods
    quint32 getcurrentControlStatusGUID(){return m_status_packet_guid;}
    GaryMotionStatus::MotionStatus_t getXMotionStatus(){return m_x_motion_status;}
    GaryMotionStatus::MotionStatus_t getZMotionStatus(){return m_z_motion_status;}
    GaryControlMode::ControlMode_t getControlMode(){return m_control_mode;}
    GaryHomingStatus::HomingStatus_t getXHomingStatus(){return m_x_homing_status;}
    GaryHomingStatus::HomingStatus_t getZHomingStatus(){return m_z_homing_status;}
    GaryLimitSwitch::LimitSwitchState_t getXLimitSwitch(){return m_x_axis_limit;}
    GaryLimitSwitch::LimitSwitchState_t getZLimitSwitch(){return m_z_axis_limit;}
    GaryOperationStatus::OperationStatus_t getOperationStatus(){return m_operation_status;}
    bool getSerialPortConnectionStatus(){return m_serial_port_connected;}
    GaryLaserStatus::LaserStatus_t getLaserStatus(){return m_laser_status;}
    GaryMotorCalibrationCycleStatus::MotorCalibration_Cycle_t getMotorCalibrationCycle(){return m_motor_calib_cycle;}
    quint8 getJoystickFlags(){return m_joystick_flags;}
    qint32 getXPosition(){return m_x_position;}
    qint32 getZPosition(){return m_z_position;}

    //Set Methods
    void setSerialConnected(bool _val){m_serial_port_connected = _val; emit this->serialPortConnectionStatusChanged();}
    void setGUIDStatus(quint32 _val){m_status_packet_guid=_val; emit this->currentStatusGUIDChanged();}
    void setXMotionStatus(GaryMotionStatus::MotionStatus_t _val){m_x_motion_status = _val; emit this->xMotionStatusChanged();}
    void setZMotionStatus(GaryMotionStatus::MotionStatus_t _val){m_z_motion_status = _val; emit this->zMotionStatusChanged();}
    void setXHomingStatus(GaryHomingStatus::HomingStatus_t _val){m_x_homing_status = _val; emit this->xHomingStatusChanged();}
    void setZHomingStatus(GaryHomingStatus::HomingStatus_t _val){m_z_homing_status = _val; emit this->zHomingStatusChanged();}
    void setXLimitSwitch(GaryLimitSwitch::LimitSwitchState_t _val){m_x_axis_limit = _val; emit this->xLimitSwitchChanged();}
    void setOperationStatus(GaryOperationStatus::OperationStatus_t _val){m_operation_status = _val; emit this->operationStatusChanged();}
    void setLaserStatus(GaryLaserStatus::LaserStatus_t _val){m_laser_status = _val; emit this->laserStatusChanged();}
    void setMotorCalibrationStatus(GaryMotorCalibrationCycleStatus::MotorCalibration_Cycle_t _val){m_motor_calib_cycle = _val; emit this->motorCalibrationCycleChanged();}
    void setJoystickFlags(quint8 _val){m_joystick_flags = _val; emit this->joystickFlagsUpdated(m_joystick_flags);}
    void setXPosition(qint32 _val){m_x_position = _val; emit this->xPositionChanged();}
    void setZPosition(quint32 _val){m_z_position = _val; emit this->zPositionChanged();}


    bool isXMotionStatusIdle();
    bool isZMotionStatusIdle();
    bool isSerialPortConnected(){return m_serial_port_connected;}

    void copyStatusPacketTo(MicroControllerStatusPacket &_packet);


private:
    bool m_serial_port_connected;
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
    void serialPortConnectionStatusChanged();

    void joystickFlagsUpdated(quint8 _flags);

public slots:
    void fillDataFromSerialRead(QByteArray _data);
    void onSerialPortConnected(){m_serial_port_connected = true; emit serialPortConnectionStatusChanged();}
    void onSerialPortDisconnected(){m_serial_port_connected = false; emit serialPortConnectionStatusChanged();}

};

#endif // GARYCONTROLLERSTATUSPACKET_H
