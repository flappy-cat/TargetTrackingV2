/************************************************************
 * Author:  yanhua
 * Date:    2017-9
 * Version: V1.2
 * Desc:    Add some valvues used for initializition.
 * Version:
 * Desc:
*************************************************************/
#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#define FLAPPYDEBUG 1

#define PERIOD_CTRL 40
#include <QSettings>
#include <QString>
#include <stdint.h>


class GlobalSettings
{
private:
    GlobalSettings();
    ~GlobalSettings();
private:
     static GlobalSettings* instance;
public:
     static GlobalSettings* GetInstance();
          void initSettings();
public:
     QString m_str_PayloadComm;
     quint32 m_uint_PayloadPortBaud;
  
     QString m_str_FireCtrlComm;
     quint32 m_uint_FireCtrlPortBaud;

     QString m_str_LocalAddr_IP;
     quint16 m_uint_LocalAddr_Port;
     QString m_str_RemoteAddr_IP;
     quint16 m_uint_RemoteAddr_Port;
};

#endif // GLOBALSETTINGS_H
