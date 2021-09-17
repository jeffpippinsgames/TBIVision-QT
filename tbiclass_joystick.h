#ifndef TBIJOYSTICK_H
#define TBIJOYSTICK_H

#include <QObject>
#include <QTimer>
#include "tbiclass_microcontrollerstatuspacket.h"


/*
  At the Moment The Joystick Driver Only Sends one Control State at a Time.
 */

class TBIJoystick : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool TBIJoystickUp READ getJoyUp NOTIFY joystickUpChanged);
    Q_PROPERTY(bool TBIJoystickDown READ getJoyDown NOTIFY joystickDownChanged);
    Q_PROPERTY(bool TBIJoystickLeft READ getJoyLeft NOTIFY joystickLeftChanged);
    Q_PROPERTY(bool TBIJoystickRight READ getJoyRight NOTIFY joystickRightChanged);
    Q_PROPERTY(bool TBIJoystickGreen READ getJoyGreen NOTIFY joystickGreenChanged);
    Q_PROPERTY(bool TBIJoystickRed READ getJoyRed NOTIFY joystickRedChanged);
    Q_PROPERTY(bool TBITJoystickBlack READ getJoyBlack NOTIFY joystickBlackChanged);
    Q_PROPERTY(bool serial_connection_status READ getSerialConnectionStatus NOTIFY serialConnectionStatusChanged)

public:
    explicit TBIJoystick(QObject *parent = nullptr);
    ~TBIJoystick();

    bool getJoyUp(){return m_up_state;}
    bool getJoyDown(){return m_down_state;}
    bool getJoyLeft(){return m_left_state;}
    bool getJoyRight(){return m_right_state;}
    bool getJoyGreen(){return m_green_state;}
    bool getJoyRed(){return m_red_state;}
    bool getJoyBlack(){return m_black_state;}
    bool getSerialConnectionStatus(){return m_serial_connection_status;}

private:
    bool m_up_state;
    bool m_down_state;
    bool m_right_state;
    bool m_left_state;
    bool m_green_state;
    bool m_red_state;
    bool m_black_state;
    bool m_serial_connection_status;

    bool m_last_up_state;
    bool m_last_down_state;
    bool m_last_right_state;
    bool m_last_left_state;
    bool m_last_green_state;
    bool m_last_red_state;
    bool m_last_black_state;

    QTimer *m_connection_timer;

    QTimer *m_up_autorepeat_timer;
    QTimer *m_up_holddown_timer;

private slots:
    void updateTBIJoystickState(MicroControllerStatusPacket &_packet);

signals:
    void serialConnectionStatusChanged();
    void joystickUpChanged();
    void joystickDownChanged();
    void joystickLeftChanged();
    void joystickRightChanged();
    void joystickGreenChanged();
    void joystickRedChanged();
    void joystickBlackChanged();

    void TBIJoystick_UP_Pressed(bool _autorepeat = false);
    void TBIJoystick_Down_Pressed(bool _autorepeat = false);
    void TBIJoystick_Left_Pressed(bool _autorepeat = false);
    void TBIJoystick_Right_Pressed(bool _autorepeat = false);
    void TBIJoystick_Green_Pressed(bool _autorepeat = false);
    void TBIJoystick_Red_Pressed(bool _autorepeat = false);
    void TBIJoystick_Black_Pressed(bool _autorepeat = false);

    void TBIJoystick_Up_Released();
    void TBIJoystick_Down_Released();
    void TBIJoystick_Left_Released();
    void TBIJoystick_Right_Released();
    void TBIJoystick_Green_Released();
    void TBIJoystick_Red_Released();
    void TBIJoystick_Black_Released();
};

#endif // TBIJOYSTICK_H
