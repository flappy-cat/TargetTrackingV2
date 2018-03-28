#ifndef SERIALCOMSETTING_H
#define SERIALCOMSETTING_H
#include<QString>


class SerialComSetting
{
public:
    SerialComSetting();
public:
    QString m_str_ComPortName;
    quint32 m_uint_ComBaudRate;
};

#endif // SERIALCOMSETTING_H
