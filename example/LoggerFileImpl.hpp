
#ifndef LOGGER_FILE_IMPL_HPP
#define LOGGER_FILE_IMPL_HPP

#include <iostream>

#include "LoggerInterface.hpp"

class FileLogger : public ILogger
{
    public:
        FileLogger()
        {
            std::cout << "Logger [" << this << "] created" << std::endl;
        };

        ~FileLogger()
        {
            std::cout << "Logger [" << this << "] destroyed" << std::endl;
        };

        void Log(const std::string& message) override
        {
            std::cout << "Logger: " << message << std::endl;
        }
};

#endif