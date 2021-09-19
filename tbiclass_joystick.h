#ifndef TBIJOYSTICK_H
#define TBIJOYSTICK_H

#include <QObject>
#include <QTimer>
#include <QVariant>
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
    Q_PROPERTY(bool enable_auto_repeat READ getEnableAutoRepeat WRITE setEnableAutoRepeat NOTIFY enableAutoRepeatChanged)

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
    bool getEnableAutoRepeat(){return m_auto_repeat;}

    void setEnableAutoRepeat(bool _state){m_auto_repeat = _state; emit getEnableAutoRepeat();}


private:
    void extractFlagstoStates();
    void processStates();
    void updateLastStates();

private:
    static const bool m_showdebug = false;

    const int m_holddown_interval_timeout = 500;
    const int m_autorepeat_timeout = 50;
    const int m_connection_timeout = 500;

    bool m_auto_repeat;
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
    void enableAutoRepeatChanged();
    void serialConnectionStateChanged();
    void joystickUpStateChanged();
    void joystickDownStateChanged();
    void joystickLeftStateChanged();
    void joystickRightStateChanged();
    void joystickGreenStateChanged();
    void joystickRedStateChanged();
    void joystickBlackStateChanged();

    void tBIJoystick_Up_Pressed(bool _autorepeat = false);
    void tBIJoystick_Down_Pressed(bool _autorepeat = false);
    void tBIJoystick_Left_Pressed(bool _autorepeat = false);
    void tBIJoystick_Right_Pressed(bool _autorepeat = false);
    void tBIJoystick_Green_Pressed(bool _autorepeat = false);
    void tBIJoystick_Red_Pressed(bool _autorepeat = false);
    void tBIJoystick_Black_Pressed(bool _autorepeat = false);

    void tBIJoystick_Up_Released();
    void tBIJoystick_Down_Released();
    void tBIJoystick_Left_Released();
    void tBIJoystick_Right_Released();
    void tBIJoystick_Green_Released();
    void tBIJoystick_Red_Released();
    void tBIJoystick_Black_Released();
};

#endif // TBIJOYSTICK_H
