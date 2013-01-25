#include "Logger_log4cxx.h"

#include "../../thirdpartylib/log4cxx/log4cxx.h"
#include "../../thirdpartylib/log4cxx/propertyconfigurator.h"

namespace IMUST
{

class LoggerPtrWrapper
{
public:
    log4cxx::LoggerPtr &operator ->()
    {
        return loggerPtr_;
    }
    log4cxx::LoggerPtr &operator *()
    {
        return loggerPtr_;
    }
    log4cxx::LoggerPtr &operator =(const log4cxx::LoggerPtr loggerPtr)
    {
        loggerPtr_ = loggerPtr;
        return loggerPtr_;
    }

private:
    log4cxx::LoggerPtr  loggerPtr_;
};

Log4CxxLoggerImpl::Log4CxxLoggerImpl(const OJString &configFileName,
    const OJString &logTag) :
    logger_(new LoggerPtrWrapper)
{
    assert(!configFileName.empty() && "Config filename can not be empty");

    log4cxx::PropertyConfigurator::configure(configFileName);
    *logger_ = log4cxx::Logger::getLogger(logTag);
}

Log4CxxLoggerImpl::~Log4CxxLoggerImpl()
{

}

void Log4CxxLoggerImpl::logFatal(const OJString &msg)
{
    (*logger_)->fatal(msg);
}

void Log4CxxLoggerImpl::logError(const OJString &msg)
{
    (*logger_)->error(msg);
}

void Log4CxxLoggerImpl::logWarn(const OJString &msg)
{
    (*logger_)->warn(msg);
}

void Log4CxxLoggerImpl::logInfo(const OJString &msg)
{
    (*logger_)->info(msg);
}

void Log4CxxLoggerImpl::logDebug(const OJString &msg)
{
    (*logger_)->debug(msg);
}

void Log4CxxLoggerImpl::logTrace(const OJString &msg)
{
    (*logger_)->trace(msg);
}

}