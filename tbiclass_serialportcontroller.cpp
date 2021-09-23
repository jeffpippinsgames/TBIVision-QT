#include "tbiclass_serialportcontroller.h"

SerialPortController::SerialPortController (QObject *parent) : QObject(parent)
{
    m_microcontroller_heartbeat_timer = new QTimer(this);
    m_microcontroller_heartbeat_timer->setInterval(200);
    m_microcontroller_heartbeat_timer->setSingleShot(true);
    QObject::connect(m_microcontroller_heartbeat_timer, SIGNAL(timeout()), this, SLOT(onSendMicroControllerHeartbeat()));

    m_port_reconnection_timer = new QTimer(this);
    m_port_reconnection_timer->setInterval(200);
    QObject::connect(m_port_reconnection_timer, SIGNAL(timeout()), this, SLOT(onReconnectTimer()));

    m_isconnected = false;
    this->openMicroControllerPort();
}

SerialPortController::~SerialPortController()
{
    if(m_serial_port)
    {
        if(m_serial_port->isOpen())
        {
            m_serial_port->close();
            if(m_showdebug) qDebug() << "SerialPortController::~SerialPortController Serial Port Closed.";
        }
    }
    if(m_showdebug) qDebug() << "SerialPortController::~SerialPortController() Object Destroyed.";
}

SerialPortControllerReturnType::SerialControllerReturnType_t SerialPortController::sendSerialCommand(QByteArray &_data)
{
    if(m_serial_port == nullptr)
    {
        if(m_showdebug) qDebug() << "TBIClass_SerialPortController::sendSerialCommand() Error in sendSerialCommand: m_serial_port is null";
        activateReConnectionTimerIfNeeded();
        return SerialPortControllerReturnType::TBI_SERIAL_SERIAL_PORT_IS_NULL_POINTER;
    }
    if(_data.size() > TBIConstants::TBI_COMMAND_BUFFER_SIZE)
    {
        if(m_showdebug) qDebug() << "TBIClass_SerialPortController::sendSerialCommand() Error. _data is larger than allowed size.";
        return SerialPortControllerReturnType::TBI_SERIAL_FAILED_SEND_BYTEARRAY_TO_LARGE;
    }
    if(m_serial_port->isOpen())
    {
        //Make Sure the Data Array is TBI_COMMAND_BUFFER_SIZE in size.
        //The Microcontroller an incomming packet to be this size.
        while(_data.size() < TBIConstants::TBI_COMMAND_BUFFER_SIZE)
        {
            _data.append(uint(255));
        }
        m_serial_port->write(_data);
        if(_data[0].operator!=(uint(GaryCommands::TBI_CMD_KEEP_ALIVE)))
        {
            if(m_showdebug) qDebug() << "TBIClass_SerialPortController::sendSerialCommand() Sending Command to Controller: " << _data.toHex(',');
        }

    }
    else
    {
        activateReConnectionTimerIfNeeded();
        if(m_showdebug) qDebug() << "TBIClass_SerialPortController::sendSerialCommand() Error in sendSerialCommand: m_serial_port is not open";
        return SerialPortControllerReturnType::TBI_SERIAL_SERIAL_PORT_IS_NOT_OPENED;
    }
    return SerialPortControllerReturnType::TBI_SERIAL_OK;
}

void SerialPortController::setStatus(QString _msg)
{
  m_status = _msg;
  emit statusChanged();
}

void SerialPortController::activateReConnectionTimerIfNeeded()
{
    if(!m_port_reconnection_timer->isActive()) m_port_reconnection_timer->start();
}

SerialPortControllerReturnType::SerialControllerReturnType_t SerialPortController::openMicroControllerPort(quint16 _vendorid, quint16 _productid)
{
    m_serial_port = nullptr;
    m_serial_info = new QSerialPortInfo();

    foreach(QSerialPortInfo _serialinfo, m_serial_info->availablePorts())
    {
        if(_serialinfo.hasProductIdentifier() && _serialinfo.hasVendorIdentifier() )
        {
            if(_serialinfo.vendorIdentifier() == _vendorid)
            {
                if(_serialinfo.productIdentifier() == _productid)
                {
                    m_serial_port = new QSerialPort(_serialinfo, this);
                    m_serial_port->setBaudRate(2000000);
                    m_serial_port->setStopBits(QSerialPort::OneStop);
                    m_serial_port->setParity(QSerialPort::EvenParity);
                    m_serial_port->setDataBits(QSerialPort::Data8);
                    m_serial_port->setFlowControl(QSerialPort::NoFlowControl);
                    QObject::connect(m_serial_port, SIGNAL(readyRead()), this, SLOT(readSerial()));
                    QObject::connect(m_serial_port, SIGNAL(errorOccurred(QSerialPort::SerialPortError)), this, SLOT(onSerialPortError(QSerialPort::SerialPortError)));
                    if(m_serial_port->open(QIODevice::ReadWrite))
                    {
                        m_isconnected = true;
                        emit connectionChanged();
                        m_microcontroller_heartbeat_timer->start();
                        acknowledgeStatusPacket();
                        if(m_showdebug) qDebug() << "SerialPortControllerReturnType::openMicroControllerPort() Serial Port Opened.";
                        setStatus("MicroController Serial Port Opened.");
                        return SerialPortControllerReturnType::TBI_SERIAL_OK;
                    }
                    else
                    {
                        if(m_showdebug) qDebug() << "SerialPortControllerReturnType::openMicroControllerPort() Error Opening Serial Port";
                        setStatus("Failed to Open Microcontroller Serial Port.");
                        activateReConnectionTimerIfNeeded();
                        return SerialPortControllerReturnType::TBI_SERIAL_FAILED_TO_OPEN_SERIAL_PORT;
                    }
                }
            }
        }
    }
    if(m_showdebug) qDebug() << "SerialPortControllerReturnType::openMicroControllerPort() Could Not Find Specified Vendor and Product ID.";
    setStatus("Specified MicroController Not Found.");
    activateReConnectionTimerIfNeeded();
    return SerialPortControllerReturnType::TBI_SERIAL_FAILED_TO_FIND_DEVICE_WITH_VEDNOR_AND_PRODUCT_ID;

}

SerialPortControllerReturnType::SerialControllerReturnType_t SerialPortController::acknowledgeStatusPacket()
{
    QByteArray _cmd;
    _cmd.append(uint(GaryCommands::TBI_CMD_ACKNOWLEDGE_STATUS_PACKET));
    if(m_showdebug) qDebug() << "TBIClass_SerialPortController::acknowledgeStatusPacket(): Sending MicroController Acknowledge Status Packet Command";
    return sendSerialCommand(_cmd);
}

void SerialPortController::readSerial()
{

    if(m_showdebug)
    {
        qDebug() << "TBIClass_SerialPortController::readSerial() Available Bytes Ready To Be Read = " << m_serial_port->bytesAvailable();
    }
    if(m_serial_port->bytesAvailable() == TBIConstants::TBI_CONTROL_STATUS_BUFFER_SIZE)
    {
        QByteArray _data = m_serial_port->readAll();
        m_microcontroller_status_packet.fillDataFromSerialRead(_data);
        emit microControllerPacketReady(m_microcontroller_status_packet);
        emit joystickFlagsReady(m_microcontroller_status_packet.getJoystickFlags());
        acknowledgeStatusPacket();
    }
}

void SerialPortController::onSerialPortError(QSerialPort::SerialPortError _error)
{
    switch(_error)
    {
        case QSerialPort::NoError:
            if(m_showserialerrors) qDebug() << "SerialPortController::onSerialPortError(): Serial Port No Error: " << _error;
        break;
        case QSerialPort::DeviceNotFoundError:
            if(m_showserialerrors) qDebug() << "SerialPortController::onSerialPortError(): Serial Port Device Not Found Error: " << _error;
        break;
        case QSerialPort::PermissionError:
            if(m_showserialerrors) qDebug() << "SerialPortController::onSerialPortError(): Serial Port Permission Error: " << _error;
        break;
        case QSerialPort::OpenError:
            if(m_showserialerrors) qDebug() << "SerialPortController::onSerialPortError(): Serial Port Open Error: " << _error;
        break;
        case QSerialPort::NotOpenError:
            if(m_showserialerrors) qDebug() << "SerialPortController::onSerialPortError(): Serial Port Not Open Error: " << _error;
        break;
        case QSerialPort::ParityError:
            if(m_showserialerrors) qDebug() << "SerialPortController::onSerialPortError(): Serial Port Parity Error: " << _error;
        break;
        case QSerialPort::FramingError:
            if(m_showserialerrors) qDebug() << "SerialPortController::onSerialPortError(): Serial Port Framing Error: " << _error;
        break;
        case QSerialPort::BreakConditionError:
            if(m_showserialerrors) qDebug() << "SerialPortController::onSerialPortError(): Serial Port Break Condition Error: " << _error;
        break;
        case QSerialPort::WriteError:
            if(m_showserialerrors) qDebug() << "SerialPortController::onSerialPortError(): Serial Port Write Error: " << _error;
        break;
        case QSerialPort::ReadError:
            if(m_showserialerrors) qDebug() << "SerialPortController::onSerialPortError(): Serial Port Read Error: " << _error;
        break;
        case QSerialPort::ResourceError:
            if(m_serial_port)
            {
                if(m_serial_port->isOpen())
                {
                    m_serial_port->close();

                }
            }
            m_isconnected = false;
            emit connectionChanged();
            activateReConnectionTimerIfNeeded();
            if(m_showserialerrors) qDebug() << "SerialPortController::onSerialPortError(): Serial Port Resource Error: " << _error;
            setStatus("Serial Port Resource Error.");
        break;
        case QSerialPort::UnsupportedOperationError:
            if(m_showserialerrors) qDebug() << "SerialPortController::onSerialPortError(): Serial Port Unsupported Operation Error: " << _error;
        break;
        case QSerialPort::TimeoutError:
            if(m_showserialerrors) qDebug() << "SerialPortController::onSerialPortError(): Serial Port Timeout Error: " << _error;
        break;
        case QSerialPort::UnknownError:
            if(m_showserialerrors) qDebug() << "SerialPortController::onSerialPortError(): Serial Port Unknown Error: " << _error;
        break;
    }


}

void SerialPortController::onReconnectTimer()
{
    if(openMicroControllerPort() == SerialPortControllerReturnType::TBI_SERIAL_OK) m_port_reconnection_timer->stop();
}

void SerialPortController::onSendMicroControllerHeartbeat()
{
    QByteArray _cmd;
    _cmd.append(char(GaryCommands::TBI_CMD_KEEP_ALIVE));
    sendSerialCommand(_cmd);
    m_microcontroller_heartbeat_timer->start();
}
