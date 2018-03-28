

#include "log4cat.h"
Log4Cat* Log4Cat::pInstance = NULL;

Log4Cat::Log4Cat()
{
    pInstance = NULL;
}
Log4Cat::~Log4Cat ()
{
    if(pInstance != NULL)
    {
        delete pInstance;
    }
    pInstance = NULL;

}

Log4Cat* Log4Cat::getInstance ()
{
    if(NULL == pInstance)
    {
        pInstance = new Log4Cat;
    }
    return pInstance;
}

void Log4Cat::InitLog(const std::string &strPathProfile)
{
    log4cplus::initialize ();
    PropertyConfigurator::doConfigure (LOG4CPLUS_TEXT(strPathProfile));
    m_logger = Logger::getRoot ();
}

void Log4Cat::Log(Enum_LogLevel loglevel, const std::__cxx11::string &strLog)
{
    switch (loglevel) {
    case Enum_LogLevel_Debug:
        LOG4CPLUS_DEBUG(m_logger , strLog);
        break;
    default:
        break;
    }
}


