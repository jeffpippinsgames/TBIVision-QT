#include "tbiclass_microcontrollerstatuspacket.h"

MicroControllerStatusPacket::MicroControllerStatusPacket(QObject *parent) : QObject(parent)
{
    m_x_motion_status = GaryMotionStatus::TBI_MOTION_STATUS_IDLE;
    m_z_motion_status = GaryMotionStatus::TBI_MOTION_STATUS_IDLE;
    m_control_mode = GaryControlMode::TBI_CONTROL_MODE_MANUAL_MODE;
    m_x_axis_limit = GaryLimitSwitch::TBI_LIMIT_SWITCH_STATE_OK;
    m_z_axis_limit = GaryLimitSwitch::TBI_LIMIT_SWITCH_STATE_OK;
    m_operation_status = GaryOperationStatus::TBI_OPERATION_OK;
    m_x_homing_status = GaryHomingStatus::TBI_HOMING_STATUS_HOMED;
    m_z_homing_status = GaryHomingStatus::TBI_HOMING_STATUS_HOMED;
    m_status_packet_guid = 0;
    m_motor_calib_cycle = GaryMotorCalibrationCycleStatus::TBI_MOTORCAL_OFF;
    m_joystick_flags = 0;

}

void MicroControllerStatusPacket::fillDataFromSerialRead(QByteArray &_data)
{

    if(_data.size() != TBIConstants::TBI_CONTROL_STATUS_BUFFER_SIZE) return;

    //GaryMotionStatus::MotionStatus_t _readxmotionstatus;
    //GaryMotionStatus::MotionStatus_t _readzmotionstatus;
    //GaryControlMode::ControlMode_t _readcontrolmode;

    m_status_packet_guid = ((quint32)((quint8)_data[0] << 24)
            + (quint32)((quint8)_data[1] << 16)
            + (quint32)((quint8)_data[2] << 8)
            + (quint32)((quint8)_data[3]));

    //_readxmotionstatus = (GaryMotionStatus::MotionStatus_t) ((quint8)_data[4]);
    //_readzmotionstatus = (GaryMotionStatus::MotionStatus_t) ((quint8)_data[5]);
    m_x_motion_status = (GaryMotionStatus::MotionStatus_t) ((quint8)_data[4]);
    m_z_motion_status = (GaryMotionStatus::MotionStatus_t) ((quint8)_data[5]);

    m_x_position = ((qint32)((quint8)_data[6] << 24)
            + (qint32)((quint8)_data[7] << 16)
            + (qint32)((quint8)_data[8] << 8)
            + (qint32)((quint8)_data[9]));

    m_z_position = ((qint32)((quint8)_data[10] << 24)
            + (qint32)((quint8)_data[11] << 16)
            + (qint32)((quint8)_data[12] << 8)
            + (qint32)((quint8)_data[13]));

    //_readcontrolmode = (GaryControlMode::ControlMode_t) ((quint8)_data[14]);
    m_control_mode = (GaryControlMode::ControlMode_t) ((quint8)_data[14]);

    m_x_axis_limit = (GaryLimitSwitch::LimitSwitchState_t) ((quint8)_data[15]);
    m_z_axis_limit = (GaryLimitSwitch::LimitSwitchState_t) ((quint8)_data[16]);

    m_operation_status = (GaryOperationStatus::OperationStatus_t) ((quint8)_data[17]);

    m_laser_status = (GaryLaserStatus::LaserStatus_t) ((quint8)_data[18]);

    m_x_homing_status = (GaryHomingStatus::HomingStatus_t) ((quint8)_data[19]);
    m_z_homing_status = (GaryHomingStatus::HomingStatus_t) ((quint8)_data[20]);

    m_motor_calib_cycle = (GaryMotorCalibrationCycleStatus::MotorCalibration_Cycle_t) ((quint8)_data[21]);

    m_joystick_flags = ((quint8)_data[22]);

    if(m_show_debug)
    {
        qDebug() << "-------------------------------------------------------";
        qDebug() << "MicroControllerStatusPacket::fillDataFromSerialRead()";
        qDebug() << "Status Packet GUID: " << m_status_packet_guid;
        qDebug() << "X Motion Status: " << m_x_motion_status;
        qDebug() << "Z Motion Status: " << m_z_motion_status;
        qDebug() <<" X Position: " << m_x_position;
        qDebug() << "Z Position: " << m_z_position;
        qDebug() << "Control Mode: " << m_control_mode;
        qDebug() << "X Axis Limit State: " << m_x_axis_limit;
        qDebug() << "Z Axis Limit State: " << m_z_axis_limit;
        qDebug() << "Operation Status: " << m_operation_status;
        qDebug() << "Laser Status: " << m_laser_status;
        qDebug() << "X Homing Status: " << m_x_homing_status;
        qDebug() << "Z Homing Status: " << m_z_homing_status;
        qDebug() << "Motor Calibration Cycle: " << m_motor_calib_cycle;
        qDebug() << "Joystick Flags: " << m_joystick_flags;
        qDebug() << "-------------------------------------------------------";
        qDebug() << "QByteArray _data = " << _data.toHex(',');
        qDebug() << "-------------------------------------------------------";
    }

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

    emit joystickFlagsUpdated(m_joystick_flags);
}

void MicroControllerStatusPacket::copyStatusPacketTo(MicroControllerStatusPacket &_packet)
{
    _packet.m_status_packet_guid = this->m_status_packet_guid;
    _packet.m_x_motion_status = this->m_x_motion_status;
    _packet.m_z_motion_status = this->m_z_motion_status;
    _packet.m_x_position = this->m_x_position;
    _packet.m_z_position = this->m_z_position;
    _packet.m_control_mode = this->m_control_mode;
    _packet.m_x_axis_limit = this->m_x_axis_limit;
    _packet.m_z_axis_limit = this->m_z_axis_limit;
    _packet.m_operation_status = this->m_operation_status;
    _packet.m_laser_status = this->m_laser_status;
    _packet.m_x_homing_status = this->m_x_homing_status;
    _packet.m_z_homing_status = this->m_z_homing_status;
    _packet.m_motor_calib_cycle = this->m_motor_calib_cycle;
    _packet.m_joystick_flags = this->m_joystick_flags;

    emit _packet.xMotionStatusChanged();
    emit _packet.zMotionStatusChanged();
    emit _packet.xPositionChanged();
    emit _packet.zPositionChanged();
    emit _packet.controlModeChanged();
    emit _packet.xLimitSwitchChanged();
    emit _packet.zLimitSwitchChanged();
    emit _packet.operationStatusChanged();
    emit _packet.laserStatusChanged();
    emit _packet.currentStatusGUIDChanged();
    emit _packet.motorCalibrationCycleChanged();
    emit _packet.xHomingStatusChanged();
    emit _packet.zHomingStatusChanged();

    emit joystickFlagsUpdated(m_joystick_flags);
}

bool MicroControllerStatusPacket::isXMotionStatusIdle()
{
    if(m_x_motion_status == GaryMotionStatus::TBI_MOTION_STATUS_IDLE) return true;
    return false;
}

bool MicroControllerStatusPacket::isZMotionStatusIdle()
{
    if(m_z_motion_status == GaryMotionStatus::TBI_MOTION_STATUS_IDLE) return true;
    return false;
}
