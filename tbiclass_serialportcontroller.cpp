#include "tbiclass_serialportcontroller.h"

SerialPortController::SerialPortController (QObject *parent) : QObject(parent)
{
}

SerialPortController::~SerialPortController()
{

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
                    //Best Practical Baud Rate with the Serial Lib. See https://arduino.stackexchange.com/questions/296/how-high-of-a-baud-rate-can-i-go-without-errors
                    if(m_serial_port->open(QIODevice::ReadWrite))
                    {
                        m_serial_port->setBaudRate(2000000);
                        //m_serial_port->setBaudRate(1000000);
                        //m_serial_port->setBaudRate(500000);
                        //m_serial_port->setBaudRate(250000);
                        //m_serial_port->setBaudRate(230400);
                        //m_serial_port->setBaudRate(115200);
                        m_serial_port->setStopBits(QSerialPort::OneStop);
                        m_serial_port->setParity(QSerialPort::EvenParity);
                        m_serial_port->setDataBits(QSerialPort::Data8);
                        m_serial_port->setFlowControl(QSerialPort::NoFlowControl);
                        QObject::connect(m_serial_port, SIGNAL(readyRead()), this, SLOT(readSerial()));
                        QObject::connect(m_serial_port, SIGNAL(errorOccurred(QSerialPort::SerialPortError)), this, SLOT(onSerialPortError(QSerialPort::SerialPortError)));
                        m_serial_port->flush();
                        m_serial_port->clear();
                        QThread::msleep(500);
                        m_write_timer->start();
                        m_microcontroller_heartbeat_timer->start();
                        m_isconnected = true;
                        emit serialPortConnected();
                        if(m_showdebug) qDebug() << "SerialPortControllerReturnType::openMicroControllerPort() Serial Port Opened.";
                        return SerialPortControllerReturnType::TBI_SERIAL_OK;
                    }
                    else
                    {
                        if(m_showdebug) qDebug() << "SerialPortControllerReturnType::openMicroControllerPort() Error Opening Serial Port";
                        m_isconnected = false;
                        emit serialPortDisconnected();
                        activateReConnectionTimerIfNeeded();
                        return SerialPortControllerReturnType::TBI_SERIAL_FAILED_TO_OPEN_SERIAL_PORT;
                    }
                }
            }
        }
    }
    if(m_showdebug) qDebug() << "SerialPortControllerReturnType::openMicroControllerPort() Could Not Find Specified Vendor and Product ID.";
    activateReConnectionTimerIfNeeded();
    return SerialPortControllerReturnType::TBI_SERIAL_FAILED_TO_FIND_DEVICE_WITH_VEDNOR_AND_PRODUCT_ID;

}

SerialPortControllerReturnType::SerialControllerReturnType_t SerialPortController::acknowledgeStatusPacket()
{
    QByteArray _cmd;
    _cmd.append(uint(GaryCommands::TBI_CMD_ACKNOWLEDGE_STATUS_PACKET));
    if(m_showdebug) qDebug() << "TBIClass_SerialPortController::acknowledgeStatusPacket(): Sending MicroController Acknowledge Status Packet Command";
    sendSerialCommand(_cmd);
    return SerialPortControllerReturnType::TBI_SERIAL_OK;
}

void SerialPortController::initialize()
{

    m_microcontroller_heartbeat_timer = new QTimer(this);
    m_microcontroller_heartbeat_timer->setInterval(100);
    m_microcontroller_heartbeat_timer->setSingleShot(true);
    QObject::connect(m_microcontroller_heartbeat_timer, SIGNAL(timeout()), this, SLOT(onSendMicroControllerHeartbeat()));

    m_port_reconnection_timer = new QTimer(this);
    m_port_reconnection_timer->setInterval(500);
    QObject::connect(m_port_reconnection_timer, SIGNAL(timeout()), this, SLOT(onReconnectTimer()));

    m_write_buffer = new QByteArray();
    m_write_timer = new QTimer(this);
    m_write_timer->setInterval(10);
    m_write_timer->setSingleShot(true);
    QObject::connect(m_write_timer, &QTimer::timeout, this, &SerialPortController::onWriteSerialTimer);

    this->openMicroControllerPort();
}

void SerialPortController::startThread()
{
    this->initialize();
    if(m_showdebug) qDebug() << "Staring Serial Thread";

}

void SerialPortController::quitThread()
{
    m_microcontroller_heartbeat_timer->stop();
    if(m_port_reconnection_timer->isActive()) m_port_reconnection_timer->stop();
    delete m_port_reconnection_timer;
    delete m_microcontroller_heartbeat_timer;
    delete m_write_buffer;
    delete m_write_timer;

    if(m_serial_port)
    {
        while(m_serial_port->isOpen())
        {
            m_serial_port->close();
            if(m_showdebug) qDebug() << "SerialPortController::~SerialPortController Serial Port Closed.";
        }
    }
    delete m_serial_port;
    delete m_serial_info;
    if(m_showdebug) qDebug() << "SerialPortController::~SerialPortController() Object Destroyed.";
}

void SerialPortController::sendSerialCommand(QByteArray _data)
{
    if(m_serial_port == nullptr)
    {
        if(m_showdebug) qDebug() << "TBIClass_SerialPortController::sendSerialCommand() Error in sendSerialCommand: m_serial_port is null";
        activateReConnectionTimerIfNeeded();
        return;
    }
    if(_data.size() > TBIConstants::TBI_COMMAND_BUFFER_SIZE)
    {
        if(true) qDebug() << "TBIClass_SerialPortController::sendSerialCommand() Error. _data is larger than allowed size.";
        return;
    }
    if(m_serial_port->isOpen())
    {
        //Make Sure the Data Array is TBI_COMMAND_BUFFER_SIZE in size.
        //The Microcontroller an incomming packet to be this size.
        while(_data.size() < TBIConstants::TBI_COMMAND_BUFFER_SIZE)
        {
            _data.append(uint(255));
        }
        if(m_showdebug && _data[0].operator!=((char)GaryCommands::TBI_CMD_KEEP_ALIVE)) qDebug() << "TBIClass_SerialPortController::sendSerialCommand() " << _data.toHex(',') << " added to write buffer";
        m_write_buffer->push_back(_data);
        //m_serial_port->write(_data);
        //m_serial_port->waitForBytesWritten(1000);
    }
    else
    {
        activateReConnectionTimerIfNeeded();
        if(m_showdebug) qDebug() << "TBIClass_SerialPortController::sendSerialCommand() Error in sendSerialCommand: m_serial_port is not open";

    }

}

void SerialPortController::readSerial()
{

    //qDebug() << "Reading Serial";
    if(m_showdebug)
    {
        qDebug() << "TBIClass_SerialPortController::readSerial() Available Bytes Ready To Be Read = " << m_serial_port->bytesAvailable();
    }
    while(m_serial_port->bytesAvailable() >= TBIConstants::TBI_CONTROL_STATUS_BUFFER_SIZE)
    {

        QByteArray _data = m_serial_port->read(TBIConstants::TBI_CONTROL_STATUS_BUFFER_SIZE);
        if(m_showdebug)
        {
            qDebug() << "QByteArray _data = " << _data.toHex(',');
        }
        if(m_showdebug) qDebug() << "_data size = " << _data.size();
        emit microControllerPacketReady(_data);
        quint8 _joystick_flags = ((quint8)_data[22]);
        emit joystickFlagsReady(_joystick_flags);
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
        if(m_serial_port->isOpen())
        {
            m_serial_port->flush();
            m_serial_port->clear();
            this->acknowledgeStatusPacket();
        }
        else
        {
            this->closeAndDestroySerialPort();
            m_isconnected = false;
            emit serialPortDisconnected();
            activateReConnectionTimerIfNeeded();
        }
        break;
    case QSerialPort::ResourceError:
        this->closeAndDestroySerialPort();
        m_isconnected = false;
        emit serialPortDisconnected();
        activateReConnectionTimerIfNeeded();
        if(m_showserialerrors) qDebug() << "SerialPortController::onSerialPortError(): Serial Port Resource Error: " << _error;
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

void SerialPortController::closeAndDestroySerialPort()
{
    if(m_serial_port)
    {
        m_serial_port->flush();
        while(m_serial_port->isOpen())
        {
            m_serial_port->close();
        }
        //delete m_serial_port;
    }
}

void SerialPortController::onWriteSerialTimer()
{
    if(m_showdebug) qDebug() << "SerialPortController::onWriteSerialTimer(): Write Buffer Size = " << m_write_buffer->size();
    //Check If there are commands in the buffer
    if(m_write_buffer->size() >= TBIConstants::TBI_COMMAND_BUFFER_SIZE)
    {
        if(m_showdebug)
        {
            QByteArray _data = m_write_buffer->left(TBIConstants::TBI_COMMAND_BUFFER_SIZE);
            qDebug() << "SerialPortController::onWriteSerialTimer(): Writing " << _data.toHex(',') << " To Serial Port.";
        }
        m_serial_port->write(m_write_buffer->left(TBIConstants::TBI_COMMAND_BUFFER_SIZE));
        if(m_serial_port->waitForBytesWritten())
        {
            m_write_buffer->remove(0, TBIConstants::TBI_COMMAND_BUFFER_SIZE);
        }
    }
    m_write_timer->start();
}


