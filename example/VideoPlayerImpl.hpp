
#ifndef VIDEO_PLAYER_HPP
#define VIDEO_PLAYER_HPP

#include <iostream>
#include <memory>
#include <IocContainer.hpp>

#include "VideoPlayerInterface.hpp"
#include "DecoderInterface.hpp"

class VideoPlayer : public IVideoPlayer
{
    public:
        VideoPlayer(std::shared_ptr<IDecoder> decoder) : m_decoder(decoder)
        {
            std::cout << "VideoPlayer [" << this << "] created" << std::endl;
        }

        ~VideoPlayer()
        {
            std::cout << "VideoPlayer [" << this << "] destroyed" << std::endl;
        }

        void PresentFrame(const std::vector<uint16_t>& frame) override
        {
            m_decoder->DecodeData(frame);
        }

    private:
        std::shared_ptr<IDecoder> m_decoder;
};

#endif