/*
QML Import Type.
import "tbi.vision.components" 1 0
 */

#ifndef TBICORE_GARY_H
#define TBICORE_GARY_H

#include <QObject>
#include <QQuickItem>
#include <QByteArray>
#include "tbicore_constants.h"
#include <QTimer>
#include <QVariant>
#include "tbiclass_garybasetypes.h"
#include "tbiclass_microcontrollerstatuspacket.h"
#include "tbiclass_serialportcontroller.h"
#include "tbiclass_joystick.h"
#include <QQmlApplicationEngine>
#include <QQmlContext>

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
    Q_OBJECT //QOBJECT MACRO

public:

    //Public Constructors, Destructors and Init Methods-----------
    explicit Gary(QObject *parent = nullptr);
    ~Gary();
    static void declareQML();
    void setRootQMLContextProperties(QQmlApplicationEngine &engine);
    MicroControllerStatusPacket* getMicroControllerStatusPacketPointer(){return &m_micro_status_packet;}
    SerialPortController* getSerialPortControllerPointer(){return &m_serial_port_controller;}

    //Public Methods to Control the MicroController
    Q_INVOKABLE void sendStopMovement();
    Q_INVOKABLE void sendJogUp();
    Q_INVOKABLE void sendJogDown();
    Q_INVOKABLE void sendJogLeft();
    Q_INVOKABLE void sendJogRight();
    Q_INVOKABLE void toggleLaserPower();
    Q_INVOKABLE void autoMoveXAxis(qint32 _steps);
    Q_INVOKABLE void autoMoveZAxis(qint32 _steps);
    Q_INVOKABLE void setControllerToCalibrationSpeed();
    Q_INVOKABLE void setControllerToOperationSpeed();
    Q_INVOKABLE void setControlModeToFullAuto();
    Q_INVOKABLE void setControlModeToHeightOnly();
    Q_INVOKABLE void setControlModeToManual();
    Q_INVOKABLE void setControlModeToMotorCalibration();
    Q_INVOKABLE void setControlMode(GaryControlMode::ControlMode_t _mode);
    Q_INVOKABLE void sendProceedNextMotorPhase();
    Q_INVOKABLE void startMotorCalibration(qint32 _steps);

    //------------------------------------------------------------

private:
    static const bool m_showdebug = false;

    //Microcontroller Status Packet Manager
    MicroControllerStatusPacket m_micro_status_packet;
    SerialPortController m_serial_port_controller;
    TBIJoystick m_joystick;
    //-------------------------------------------------------------

    //Private Methods----------------------------------------------
    void processControllerInput();
    //--------------------------------------------------------------

    //Signals-------------------------------------------------------
signals:
    void aboutToDestroy();
    void completed();
    void garyControllerFired(QVariant controlsignal, QVariant autorepeat);
    //--------------------------------------------------------------

public slots:
    void updateMicroControllerStatusPacket(MicroControllerStatusPacket &_packet);




};

#endif // TBICORE_GARY_H
