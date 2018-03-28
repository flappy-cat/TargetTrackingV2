#ifndef LOG4CAT_H
#define LOG4CAT_H


#include "log4cplus/logger.h"
#include "log4cplus/configurator.h"
#include "log4cplus/layout.h"
#include "log4cplus/loggingmacros.h"
#include "log4cplus/helpers/stringhelper.h"

using namespace log4cplus;
using namespace log4cplus::helpers;


typedef enum __log_level
{
    Enum_LogLevel_Infomation,
    Enum_LogLevel_Debug,
    Enum_LogLevel_Warning,
    Enum_LogLevel_Error,
    Enum_LogLevel_Fatal
}Enum_LogLevel;

#define EASYLOG(X) Log4Cat::getInstance()->Log(Enum_LogLevel_Debug , X);

class Log4Cat
{

private:
    Log4Cat();
    ~Log4Cat();
    static Log4Cat* pInstance;
    Logger m_logger;
public:
    static Log4Cat* getInstance();
    //Load property file to initialize the STYLE of logdata list.
    void InitLog(const std::string& strPathProfile);

    void Log(Enum_LogLevel loglevel , const std::string& strLog);

};

#endif // LOG4CAT_H
