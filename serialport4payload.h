#ifndef SERIALPORT4PAYLOAD_H
#define SERIALPORT4PAYLOAD_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
class SerialPort4Payload : public QObject
{
    Q_OBJECT
public:
    explicit SerialPort4Payload(QObject *parent = 0);
private:
    QSerialPort *m_pSerialPortPayload;
    bool ParseDataDown(unsigned char* bufData , int dataLen);
signals:

public slots:
    bool OpenSerialPort();
    bool CloseSerialPort();
    void SendData();
private slots:
    void ReceiveData();
};

#endif // SERIALPORT4PAYLOAD_H
