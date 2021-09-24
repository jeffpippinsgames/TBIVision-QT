#ifndef TBICLASS_SERIALPORTCONTROLLER_H
#define TBICLASS_SERIALPORTCONTROLLER_H

#include <QObject>
#include <QString>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QByteArray>
#include "tbiclass_garybasetypes.h"
#include "tbiclass_serialportcontrollerreturntype.h"
#include "tbiclass_microcontrollerstatuspacket.h"

class SerialPortController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString status READ getStatus  NOTIFY statusChanged)
    Q_PROPERTY(bool isconnected READ getIsConnected NOTIFY connectionChanged)

public:
    explicit SerialPortController (QObject *parent = nullptr);
    ~SerialPortController();

    QString getStatus(){return m_status;}

    SerialPortControllerReturnType::SerialControllerReturnType_t sendSerialCommand(QByteArray &_data);
    bool isSerialPortOK();

private:
    void setStatus(QString _msg);
    void activateReConnectionTimerIfNeeded();
    SerialPortControllerReturnType::SerialControllerReturnType_t openMicroControllerPort(quint16 _vendorid = m_arduino_uno_vendorID, quint16 _productid = m_arduino_uno_productID);
    SerialPortControllerReturnType::SerialControllerReturnType_t acknowledgeStatusPacket();
    bool getIsConnected(){return m_isconnected;}

private slots:
    void readSerial();
    void onSerialPortError(QSerialPort::SerialPortError _error);
    void onReconnectTimer();
    void onSendMicroControllerHeartbeat();
    void onPacketsPerSecondDisplayTime();


private:
    static const bool m_showdebug = false;
    static const bool m_showserialerrors = true;
    static const bool m_showstatuspacketspersec = true;

    static const quint16 m_teensy32_vendorID = 0x16C0;
    static const quint16 m_teensy32_productID = 0x0483;
    static const quint16 m_arduino_uno_vendorID = 0x2341;
    static const quint16 m_arduino_uno_productID =0x0043;
    static const quint16 m_ftdi_FT232_UART_vendorID = 0x0403;
    static const quint16 m_ftdi_FT232_UART_productID = 0x6001;


    QByteArray m_write_buffer;

    QSerialPortInfo *m_serial_info;
    QSerialPort *m_serial_port;

    QTimer *m_port_reconnection_timer;
    QTimer *m_microcontroller_heartbeat_timer;

    QTimer *m_packets_per_second_display_timer;
    quint32 m_current_guid;
    quint32 m_last_guid;
    quint32 m_guid_diff;

    QString m_status;
    bool m_isconnected;

    MicroControllerStatusPacket m_microcontroller_status_packet;

signals:
    void connectionChanged();
    void statusChanged();
    void microControllerPacketReady(MicroControllerStatusPacket &_packet);
    void joystickFlagsReady(quint8 _flags);

};

#endif // TBICLASS_SERIALPORTCONTROLLER_H
