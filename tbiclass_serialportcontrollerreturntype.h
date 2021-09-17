#ifndef TBICLASS_SERIALPORTCONTROLLERRETURNTYPE_H
#define TBICLASS_SERIALPORTCONTROLLERRETURNTYPE_H

#include <QObject>
#include <QQuickItem>

class SerialPortControllerReturnType : public QObject
{
    Q_OBJECT
public:
    explicit SerialPortControllerReturnType(QObject *parent = nullptr);

    enum SerialControllerReturnType_t {TBI_SERIAL_OK = 0x01,
                                       TBI_SERIAL_GENERAL_ERROR = 0x02,
                                       TBI_SERIAL_FAILED_TO_OPEN_SERIAL_PORT = 0x03,
                                       TBI_SERIAL_FAILED_TO_FIND_DEVICE_WITH_VEDNOR_AND_PRODUCT_ID = 0x04,
                                       TBI_SERIAL_SERIAL_PORT_IS_NULL_POINTER = 0x05,
                                       TBI_SERIAL_FAILED_SEND_BYTEARRAY_TO_LARGE = 0x06,
                                       TBI_SERIAL_SERIAL_PORT_IS_NOT_OPENED = 0x07};

    Q_ENUMS(SerialControllerReturnType_t)
    static void declareQML()
    {
        qmlRegisterType<SerialPortControllerReturnType>("tbi.vision.components", 1, 0, "SerialPortControllerReturnType");
    }
signals:

};

#endif // TBICLASS_SERIALPORTCONTROLLERRETURNTYPE_H
