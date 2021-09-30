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
#include <QThread>

class SerialPortController : public QObject
{
    Q_OBJECT

public:

    explicit SerialPortController (QObject *parent = nullptr);
    ~SerialPortController();

private:
    void initialize();
    void activateReConnectionTimerIfNeeded();
    SerialPortControllerReturnType::SerialControllerReturnType_t openMicroControllerPort(quint16 _vendorid = m_arduino_uno_vendorID, quint16 _productid = m_arduino_uno_productID);
    SerialPortControllerReturnType::SerialControllerReturnType_t acknowledgeStatusPacket();

public slots:
    void startThread();
    void quitThread();
    void sendSerialCommand(QByteArray _data);

private slots:
    void readSerial();
    void onSerialPortError(QSerialPort::SerialPortError _error);
    void onReconnectTimer();
    void onSendMicroControllerHeartbeat();
    void closeAndDestroySerialPort();
    void onWriteSerialTimer();

private:
    static const bool m_showdebug = false;
    static const bool m_showserialerrors = true;
    static const bool m_showstatuspacketspersec = false;

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

    QTimer *m_write_timer;
    QByteArray *m_write_buffer;

    bool m_isconnected;

signals:
    void serialPortDisconnected();
    void serialPortConnected();
    void statusChanged();
    void microControllerPacketReady(QByteArray _packet);
    void joystickFlagsReady(quint8 _flags);


};

#endif // TBICLASS_SERIALPORTCONTROLLER_H
