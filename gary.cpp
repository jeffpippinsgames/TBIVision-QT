#include "gary.h"
#include <QDebug>

Gary::Gary(QObject *parent) : QObject(parent)
{
    m_jogStop = true;
    m_jogUp = false;
    qDebug() << "Gary Object Instantiated.";
    this->findOpenTeensy();
}

Gary::~Gary()
{
    qDebug() << "Gary Object Destroyed.";
    m_serial_port->close();
    if(m_serial_port)
    {
        if(m_serial_port->isOpen())
        {
            m_serial_port->close();
            qDebug() << "Serial Port Closed.";
        }
    }
}


QString Gary::name() const
{
    return m_name;
}

bool Gary::jogStop() const
{
    return m_jogStop;
}

bool Gary::jogUp() const
{
    return m_jogUp;
}

void Gary::declareQML()
{
        qmlRegisterType<Gary>("tbi.vision.components", 1, 0, "Gary");
}

bool Gary::findOpenTeensy()
{
    m_serial_info = new QSerialPortInfo();
    //Poll Each Serial Port Look For The Teensy 3.2
    foreach(QSerialPortInfo _serialinfo, m_serial_info->availablePorts())
    {
       if(_serialinfo.hasProductIdentifier() && _serialinfo.hasVendorIdentifier() )
       {
           if(_serialinfo.vendorIdentifier() == m_teensy_vendorID)
           {
               if(_serialinfo.productIdentifier() == m_teensy_productID)
               {
                   m_serial_port = new QSerialPort(_serialinfo, this);
                   m_serial_port->setBaudRate(QSerialPort::Baud115200);
                   m_serial_port->setStopBits(QSerialPort::OneStop);
                   m_serial_port->setParity(QSerialPort::EvenParity);
                   m_serial_port->setDataBits(QSerialPort::Data8);
                   m_serial_port->setFlowControl(QSerialPort::NoFlowControl);
                   if(m_serial_port->open(QIODevice::ReadWrite))
                   {
                       qDebug() << "Serial Port Opened.";
                       return true;
                   }
                   else
                   {
                       qDebug() << "Error Opening Serial Port";
                       return false;
                   }
               }
           }
       }
    }
    qDebug() << "Could Not Find Teensy 3.2 Serial Device.";
    return false;
}

void Gary::setName(const QString &_name)
{
    if(_name == m_name) return;
    m_name = _name;
    emit nameChanged(); //Fire Signal

}

void Gary::setJogStop(const bool _jogstop)
{
    m_jogStop = _jogstop;
    if(_jogstop)
    {
        QByteArray _jogstopcmd;
        _jogstopcmd.append(char(GaryCommands::TBI_CMD_STOP_MOVEMENT));
        sendSerialCommand(_jogstopcmd);
    }
}

void Gary::setJogUp(const bool _jogup)
{
   m_jogUp = _jogup;
   if(_jogup)
   {
       QByteArray _jogupcmd;
       _jogupcmd.append(char(GaryCommands::TBI_CMD_JOG_UP));
       sendSerialCommand(_jogupcmd);
   }
}

void Gary::sendSerialCommand(QByteArray &_data)
{
    if(!m_serial_port) return;
    if(m_serial_port->isOpen())
    {
        m_serial_port->write(_data);
        qDebug() << "Sending Command to Controller: " << _data.toHex(',');
    }
}







