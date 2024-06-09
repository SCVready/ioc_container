
#ifndef LOGGER_INTERFACE_HPP
#define LOGGER_INTERFACE_HPP

#include <IocContainer.hpp>
#include <string>

class ILogger : public ServiceBase
{
    public:
        virtual ~ILogger() {};

        virtual void Log(const std::string& message) = 0;
};

#endif
