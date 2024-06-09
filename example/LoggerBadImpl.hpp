
#ifndef LOGGER_BAD_IMPL_HPP
#define LOGGER_BAD_IMPL_HPP

#include <memory>
#include <iostream>

#include "LoggerInterface.hpp"
#include "VideoPlayerInterface.hpp"

class LoggerBadImpl : public ILogger
{
    public:
        LoggerBadImpl(std::shared_ptr<IVideoPlayer> video_player) : m_video_player(video_player)
        {
            std::cout << "LoggerBadImpl [" << this << "] created" << std::endl;
        };

        ~LoggerBadImpl()
        {
            std::cout << "LoggerBadImpl [" << this << "] destroyed" << std::endl;
        };

        void Log(const std::string& message) override
        {
            std::cout << "LoggerBadImpl: " << message << std::endl;
        }
    private:
        std::shared_ptr<IVideoPlayer> m_video_player;
};

#endif