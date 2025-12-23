#ifndef ARDUINO_H
#define ARDUINO_H

#include <QByteArray>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class Arduino
{
public:
    Arduino();
    int connectArduino();
    int closeArduino();
    int writeToArduino(QByteArray d);
    QByteArray readFromArduino();
    QSerialPort* getSerialPort() { return serial; }
    QString getPortName() { return portName; }

private:
    QSerialPort *serial;
    static const quint16 VENDOR_ID = 9025;
    static const quint16 PRODUCT_ID = 67;
    QString portName;
    bool isAvailable;
    QByteArray buffer;
};

#endif // ARDUINO_H
