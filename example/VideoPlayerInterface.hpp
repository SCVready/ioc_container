
#ifndef VIDEO_PLAYER_INTERFACE_HPP
#define VIDEO_PLAYER_INTERFACE_HPP

#include <IocContainer.hpp>

class IVideoPlayer : public ServiceBase
{
    public:
        virtual ~IVideoPlayer() {};

        virtual void PresentFrame(const std::vector<uint16_t>& frame) = 0;
};

#endif