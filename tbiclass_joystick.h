#ifndef TBIJOYSTICK_H
#define TBIJOYSTICK_H

#include <QObject>
#include <QTimer>
#include "tbiclass_microcontrollerstatuspacket.h"

class TBIJoystick : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool up_state READ getJoyUpState NOTIFY joystickUpStateChanged);
    Q_PROPERTY(bool down_state READ getJoyDownState NOTIFY joystickDownStateChanged);
    Q_PROPERTY(bool left_state READ getJoyLeftState NOTIFY joystickLeftStateChanged);
    Q_PROPERTY(bool right_state READ getJoyRightState NOTIFY joystickRightStateChanged);
    Q_PROPERTY(bool green_state READ getJoyGreenState NOTIFY joystickGreenStateChanged);
    Q_PROPERTY(bool red_state READ getJoyRedState NOTIFY joystickRedStateChanged);
    Q_PROPERTY(bool black_state READ getJoyBlackState NOTIFY joystickBlackStateChanged);
    Q_PROPERTY(bool serial_connection_state READ getSerialConnectionState NOTIFY serialConnectionStateChanged)

public:
    explicit TBIJoystick(QObject *parent = nullptr);
    ~TBIJoystick();

    bool getJoyUpState(){return m_up_state;}
    bool getJoyDownState(){return m_down_state;}
    bool getJoyLeftState(){return m_left_state;}
    bool getJoyRightState(){return m_right_state;}
    bool getJoyGreenState(){return m_green_state;}
    bool getJoyRedState(){return m_red_state;}
    bool getJoyBlackState(){return m_black_state;}
    bool getSerialConnectionState(){return m_serial_connection_state;}

private:
    void extractFlagstoStates();
    void processStates();
    void updateLastStates();

private:
    static const bool m_showdebug = true;

    const int m_holddown_interval_timeout = 500;
    const int m_autorepeat_timeout = 50;
    const int m_connection_timeout = 500;

    bool m_updating_flags;

    quint8 m_controller_flags;
    bool m_up_state;
    bool m_down_state;
    bool m_right_state;
    bool m_left_state;
    bool m_green_state;
    bool m_red_state;
    bool m_black_state;
    bool m_serial_connection_state;

    bool m_last_up_state;
    bool m_last_down_state;
    bool m_last_right_state;
    bool m_last_left_state;
    bool m_last_green_state;
    bool m_last_red_state;
    bool m_last_black_state;

    QTimer *m_connection_timer;

    QTimer *m_up_holddown_timer;
    QTimer *m_down_holddown_timer;
    QTimer *m_left_holddown_timer;
    QTimer *m_right_holddown_timer;
    QTimer *m_green_holddown_timer;
    QTimer *m_red_holddown_timer;
    QTimer *m_black_holddown_timer;

public slots:
    void updateTBIJoystickState(quint8 _joystick_flags);

private slots:
    void onFailedConnectionTimer();

    void onUpHoldDownTimer();

    void onDownHoldDownTimer();

    void onLeftHoldDownTimer();

    void onRightHoldDownTimer();

    void onGreenHoldDownTimer();

    void onRedHoldDownTimer();

    void onBlackHoldDownTimer();

signals:
    void serialConnectionStateChanged();
    void joystickUpStateChanged();
    void joystickDownStateChanged();
    void joystickLeftStateChanged();
    void joystickRightStateChanged();
    void joystickGreenStateChanged();
    void joystickRedStateChanged();
    void joystickBlackStateChanged();

    void TBIJoystick_Up_Pressed(QVariant _autorepeat = false);
    void TBIJoystick_Down_Pressed(QVariant _autorepeat = false);
    void TBIJoystick_Left_Pressed(QVariant _autorepeat = false);
    void TBIJoystick_Right_Pressed(QVariant _autorepeat = false);
    void TBIJoystick_Green_Pressed(QVariant _autorepeat = false);
    void TBIJoystick_Red_Pressed(QVariant _autorepeat = false);
    void TBIJoystick_Black_Pressed(QVariant _autorepeat = false);

    void TBIJoystick_Up_Released();
    void TBIJoystick_Down_Released();
    void TBIJoystick_Left_Released();
    void TBIJoystick_Right_Released();
    void TBIJoystick_Green_Released();
    void TBIJoystick_Red_Released();
    void TBIJoystick_Black_Released();
};

#endif // TBIJOYSTICK_H
