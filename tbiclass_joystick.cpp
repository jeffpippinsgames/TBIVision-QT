#include "tbiclass_joystick.h"

TBIJoystick::TBIJoystick(QObject *parent) : QObject(parent)
{
    //Instantiate Timers
    m_connection_timer = new QTimer();
    m_up_holddown_timer = new QTimer();
    m_down_holddown_timer = new QTimer();
    m_left_holddown_timer = new QTimer();
    m_right_holddown_timer = new QTimer();
    m_green_holddown_timer = new QTimer();
    m_red_holddown_timer = new QTimer();
    m_black_holddown_timer = new QTimer();
    //Set Intervals
    m_connection_timer->setInterval(m_connection_timeout);
    m_up_holddown_timer->setInterval(m_holddown_interval_timeout);
    m_down_holddown_timer->setInterval(m_holddown_interval_timeout);
    m_left_holddown_timer->setInterval(m_holddown_interval_timeout);
    m_right_holddown_timer->setInterval(m_holddown_interval_timeout);
    m_green_holddown_timer->setInterval(m_holddown_interval_timeout);
    m_red_holddown_timer->setInterval(m_holddown_interval_timeout);
    m_black_holddown_timer->setInterval(m_holddown_interval_timeout);
    //Set Single Shots
    m_connection_timer->setSingleShot(true);
    m_up_holddown_timer->setSingleShot(true);
    m_down_holddown_timer->setSingleShot(true);
    m_left_holddown_timer->setSingleShot(true);
    m_right_holddown_timer->setSingleShot(true);
    m_green_holddown_timer->setSingleShot(true);
    m_red_holddown_timer->setSingleShot(true);
    m_black_holddown_timer->setSingleShot(true);
    //Make Timer Signal Slot Connections
    QObject::connect( m_connection_timer, SIGNAL(timeout()), this, SLOT(onFailedConnectionTimer()));
    QObject::connect( m_up_holddown_timer, SIGNAL(timeout()), this, SLOT(onUpHoldDownTimer()));
    QObject::connect( m_down_holddown_timer, SIGNAL(timeout()), this, SLOT(onDownHoldDownTimer()));
    QObject::connect( m_left_holddown_timer, SIGNAL(timeout()), this, SLOT(onLeftHoldDownTimer()));
    QObject::connect( m_right_holddown_timer, SIGNAL(timeout()), this, SLOT(onRightHoldDownTimer()));
    QObject::connect( m_green_holddown_timer, SIGNAL(timeout()), this, SLOT(onGreenHoldDownTimer()));
    QObject::connect( m_red_holddown_timer, SIGNAL(timeout()), this, SLOT(onRedHoldDownTimer()));
    QObject::connect( m_black_holddown_timer, SIGNAL(timeout()), this, SLOT(onBlackHoldDownTimer()));
    //Set Internal Members
    m_updating_flags = false;
    m_up_state = false;
    m_last_up_state = false;
    m_down_state = false;
    m_last_down_state = false;
    m_left_state = false;
    m_last_left_state = false;
    m_right_state = false;
    m_last_right_state = false;
    m_green_state = false;
    m_last_green_state = false;
    m_red_state = false;
    m_last_red_state = false;
    m_black_state = false;
    m_last_black_state = false;
    m_serial_connection_state = false;

    m_auto_repeat = true;
}

TBIJoystick::~TBIJoystick()
{

}

void TBIJoystick::extractFlagstoStates()
{
    m_up_state = m_controller_flags & 0b00000001;
    m_down_state = m_controller_flags & 0b00000010;
    m_left_state = m_controller_flags & 0b00000100;
    m_right_state = m_controller_flags & 0b00001000;
    m_green_state = m_controller_flags & 0b00010000;
    m_red_state = m_controller_flags & 0b00100000;
    m_black_state = m_controller_flags & 0b01000000;
}

void TBIJoystick::processStates()
{

    /*----------------------------------------------------------------------
      State | Last State
           0|0  - Nothing Happens No Button Pressed Or Released
           0|1  - Release Button Condition
           1|0  - Initial Press Condition
           1|1  - Hold Down Button Position - Timers Control This Do Nothing
      ----------------------------------------------------------------------*/



    //Up Button Handling---------------------------------------------
    if(!m_up_state && m_last_up_state) //Release Condition
    {
        //Fire Release Signal and Kill Hold Down Timer. Reset the Hold Down Timer Interval
        m_up_holddown_timer->stop();
        emit tBIJoystick_Up_Released();
        if(m_showdebug) qDebug() << "TBIJoystick::processStates(): Joystick Up Released";
    }
    else if(m_up_state && !m_last_up_state) //Initial Press Condition
    {
        //Fire the Press Signal and Start Hold Down Timer
        emit tBIJoystick_Up_Pressed(false);
        m_up_holddown_timer->start(m_holddown_interval_timeout);
        if(m_showdebug) qDebug() << "TBIJoystick::processStates(): Joystick Up Pressed";
    }
    //----------------------------------------------------------------

    //Down Button Handling---------------------------------------------
    if(!m_down_state && m_last_down_state) //Release Condition
    {
        //Fire Release Signal and Kill Hold Down Timer. Reset the Hold Down Timer Interval
        m_down_holddown_timer->stop();
        emit tBIJoystick_Down_Released();
        if(m_showdebug) qDebug() << "TBIJoystick::processStates(): Joystick Down Released";
    }
    else if(m_down_state && !m_last_down_state) //Initial Press Condition
    {
        //Fire the Press Signal and Start Hold Down Timer
        emit tBIJoystick_Down_Pressed(false);
        m_down_holddown_timer->start(m_holddown_interval_timeout);
        if(m_showdebug) qDebug() << "TBIJoystick::processStates(): Joystick Down Pressed";
    }
    //----------------------------------------------------------------

    //Left Button Handling---------------------------------------------
    if(!m_left_state && m_last_left_state) //Release Condition
    {
        //Fire Release Signal and Kill Hold Down Timer. Reset the Hold Down Timer Interval
        m_left_holddown_timer->stop();
        emit tBIJoystick_Left_Released();
        if(m_showdebug) qDebug() << "TBIJoystick::processStates(): Joystick Left Released";
    }
    else if(m_left_state && !m_last_left_state) //Initial Press Condition
    {
        //Fire the Press Signal and Start Hold Down Timer
        emit tBIJoystick_Left_Pressed(false);
        m_left_holddown_timer->start(m_holddown_interval_timeout);
        if(m_showdebug) qDebug() << "TBIJoystick::processStates(): Joystick Left Pressed";
    }
    //----------------------------------------------------------------

    //Right Button Handling---------------------------------------------
    if(!m_right_state && m_last_right_state) //Release Condition
    {
        //Fire Release Signal and Kill Hold Down Timer. Reset the Hold Down Timer Interval
        m_right_holddown_timer->stop();
        emit tBIJoystick_Right_Released();
        if(m_showdebug) qDebug() << "TBIJoystick::processStates(): Joystick Right Released";
    }
    else if(m_right_state && !m_last_right_state) //Initial Press Condition
    {
        //Fire the Press Signal and Start Hold Down Timer
        emit tBIJoystick_Right_Pressed(false);
        m_right_holddown_timer->start(m_holddown_interval_timeout);
        if(m_showdebug) qDebug() << "TBIJoystick::processStates(): Joystick Right Pressed";
    }
    //----------------------------------------------------------------

    //Green Button Handling---------------------------------------------
    if(!m_green_state && m_last_green_state) //Release Condition
    {
        //Fire Release Signal and Kill Hold Down Timer. Reset the Hold Down Timer Interval
        m_green_holddown_timer->stop();
        emit tBIJoystick_Green_Released();
        if(m_showdebug) qDebug() << "TBIJoystick::processStates(): Green Button Released";
    }
    else if(m_green_state && !m_last_green_state) //Initial Press Condition
    {
        //Fire the Press Signal and Start Hold Down Timer
        emit tBIJoystick_Green_Pressed(false);
        m_green_holddown_timer->start(m_holddown_interval_timeout);
        if(m_showdebug) qDebug() << "TBIJoystick::processStates(): Green Button Pressed";
    }
    //----------------------------------------------------------------

    //Red Button Handling---------------------------------------------
    if(!m_red_state && m_last_red_state) //Release Condition
    {
        //Fire Release Signal and Kill Hold Down Timer. Reset the Hold Down Timer Interval
        m_red_holddown_timer->stop();
        emit tBIJoystick_Red_Released();
        if(m_showdebug) qDebug() << "TBIJoystick::processStates(): Red Button Released";
    }
    else if(m_red_state && !m_last_red_state) //Initial Press Condition
    {
        //Fire the Press Signal and Start Hold Down Timer
        emit tBIJoystick_Red_Pressed(false);
        m_red_holddown_timer->start(m_holddown_interval_timeout);
        if(m_showdebug) qDebug() << "TBIJoystick::processStates(): Red Button Pressed";
    }
    //----------------------------------------------------------------

    //Black Button Handling---------------------------------------------
    if(!m_black_state && m_last_black_state) //Release Condition
    {
        //Fire Release Signal and Kill Hold Down Timer. Reset the Hold Down Timer Interval
        m_black_holddown_timer->stop();
        emit tBIJoystick_Black_Released();
        if(m_showdebug) qDebug() << "TBIJoystick::processStates(): Black Button Released";
    }
    else if(m_black_state && !m_last_black_state) //Initial Press Condition
    {
        //Fire the Press Signal and Start Hold Down Timer
        emit tBIJoystick_Black_Pressed(false);
        m_black_holddown_timer->start(m_holddown_interval_timeout);
        if(m_showdebug) qDebug() << "TBIJoystick::processStates(): Black Button Pressed";
    }
    //----------------------------------------------------------------
}

void TBIJoystick::updateLastStates()
{
    m_last_up_state = m_up_state;
    m_last_down_state = m_down_state;
    m_last_left_state = m_left_state;
    m_last_right_state = m_right_state;
    m_last_green_state = m_green_state;
    m_last_red_state = m_red_state;
    m_last_black_state = m_black_state;
}

void TBIJoystick::updateTBIJoystickState(quint8 _joystick_flags)
{
    //if(m_showdebug) qDebug() << "TBIJoystick::updateTBIJoystickState(): Joystick State =" << QString::number(_joystick_flags, 2);
    if(m_updating_flags) return; //if Already in the loop exit

    m_updating_flags = true;

    //Do Connection Timer Stuff
    m_connection_timer->start(m_connection_timeout);
    if(!m_serial_connection_state)
    {
        m_serial_connection_state = true;
        emit serialConnectionStateChanged();
        if(m_showdebug) qDebug() << "TBIJoystick::updateTBIJoystickState((): Connection to Joystick Has Been Made.";
    }

    //Process The Joystick Signals
    m_controller_flags = _joystick_flags; //Set Flags
    extractFlagstoStates(); //Sets The States
    processStates(); //Processes the states and fires signals
    updateLastStates(); //Updates Last States

    m_updating_flags = false;
}

void TBIJoystick::onFailedConnectionTimer()
{
    m_serial_connection_state = false;
    m_up_holddown_timer->stop();
    m_down_holddown_timer->stop();
    m_left_holddown_timer->stop();
    m_right_holddown_timer->stop();
    m_green_holddown_timer->stop();
    m_red_holddown_timer->stop();
    m_black_holddown_timer->stop();
    emit serialConnectionStateChanged();
    if(m_showdebug) qDebug() << "TBIJoystick::onFailedConnectionTimer(): Connection to Joystick Has Been Lost.";
}

void TBIJoystick::onUpHoldDownTimer()
{

    if(m_auto_repeat) emit tBIJoystick_Up_Pressed(true);
    m_up_holddown_timer->start(m_autorepeat_timeout);
    if(m_showdebug) qDebug() << "TBIJoystick::processStates(): Joystick Up Pressed";
}

void TBIJoystick::onDownHoldDownTimer()
{
    if(m_auto_repeat) emit tBIJoystick_Down_Pressed(true);
    m_down_holddown_timer->start(m_autorepeat_timeout);
    if(m_showdebug) qDebug() << "TBIJoystick::processStates(): Joystick Down Pressed";
}

void TBIJoystick::onLeftHoldDownTimer()
{
    if(m_auto_repeat) emit tBIJoystick_Left_Pressed(true);
    m_left_holddown_timer->start(m_autorepeat_timeout);
    if(m_showdebug) qDebug() << "TBIJoystick::processStates(): Joystick Left Pressed";
}

void TBIJoystick::onRightHoldDownTimer()
{
    if(m_auto_repeat) emit tBIJoystick_Right_Pressed(true);
    m_right_holddown_timer->start(m_autorepeat_timeout);
    if(m_showdebug) qDebug() << "TBIJoystick::processStates(): Joystick Right Pressed";
}

void TBIJoystick::onGreenHoldDownTimer()
{
    if(m_auto_repeat) emit tBIJoystick_Green_Pressed(true);
    m_green_holddown_timer->start(m_autorepeat_timeout);
    if(m_showdebug) qDebug() << "TBIJoystick::processStates(): Green Button Pressed";
}

void TBIJoystick::onRedHoldDownTimer()
{
    if(m_auto_repeat) emit tBIJoystick_Red_Pressed(true);
    m_red_holddown_timer->start(m_autorepeat_timeout);
    if(m_showdebug) qDebug() << "TBIJoystick::processStates(): Red Button Pressed";
}

void TBIJoystick::onBlackHoldDownTimer()
{
    if(m_auto_repeat) emit tBIJoystick_Black_Pressed(true);
    m_black_holddown_timer->start(m_autorepeat_timeout);
    if(m_showdebug) qDebug() << "TBIJoystick::processStates(): Black Button Pressed";
}
