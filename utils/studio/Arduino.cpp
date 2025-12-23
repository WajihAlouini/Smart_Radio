#include "Arduino.h"
#include <QDebug>

Arduino::Arduino()
{
    buffer = "";
    portName = "";
    isAvailable = false;
    serial = new QSerialPort;
}

int Arduino::connectArduino()
{
    for(const QSerialPortInfo& info : QSerialPortInfo::availablePorts())
    {
        if(info.hasVendorIdentifier() && info.hasProductIdentifier())
        {
            if(info.vendorIdentifier() == VENDOR_ID && info.productIdentifier() == PRODUCT_ID)
            {
                isAvailable = true;
                portName = info.portName();
            }
        }
    }

    if(isAvailable) {
        serial->setPortName(portName);
        if(serial->open(QSerialPort::ReadWrite)) {
            serial->setBaudRate(QSerialPort::Baud9600);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            return 0;
        }
        return 1;
    }
    return -1;
}

int Arduino::closeArduino()
{
    if(serial->isOpen()) {
        serial->close();
        return 0;
    }
    return 1;
}

QByteArray Arduino::readFromArduino()
{
    if(serial->isReadable()) {
        buffer = serial->readAll();
        return buffer;
    }
    return QByteArray();
}

int Arduino::writeToArduino(QByteArray d)
{
    if(serial->isWritable()) {
        serial->write(d);
        return 0;
    }
    return -1;
}
