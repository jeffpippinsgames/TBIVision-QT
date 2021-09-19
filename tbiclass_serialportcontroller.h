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

private slots:
    void readSerial();
    void onSerialPortError(QSerialPort::SerialPortError _error);
    void onReconnectTimer();
    void onSendMicroControllerHeartbeat();

private:
    static const bool m_showdebug = false;

    static const quint16 m_teensy32_vendorID = 0x16C0;
    static const quint16 m_teensy32_productID = 0x0483;
    static const quint16 m_arduino_uno_vendorID = 0x2341;
    static const quint16 m_arduino_uno_productID =0x0043;
    static const quint16 m_ftdi_FT232_UART_vendorID = 0x0403;
    static const quint16 m_ftdi_FT232_UART_productID = 0x6001;

    QSerialPortInfo *m_serial_info;
    QSerialPort *m_serial_port;

    QTimer *m_port_reconnection_timer;
    QTimer *m_microcontroller_heartbeat_timer;

    QString m_status;

    MicroControllerStatusPacket m_microcontroller_status_packet;

signals:
    void statusChanged();
    void microControllerPacketReady(MicroControllerStatusPacket &_packet);
    void joystickFlagsReady(quint8 _flags);

};

#endif // TBICLASS_SERIALPORTCONTROLLER_H