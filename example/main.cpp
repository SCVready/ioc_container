
#include <IocContainer.hpp>
#include "VideoPlayerInterface.hpp"
#include "VideoPlayerImpl.hpp"
#include "DecoderInterface.hpp"
#include "DecoderHardwareImpl.hpp"
#include "LoggerInterface.hpp"
#include "LoggerFileImpl.hpp"
#include "LoggerBadImpl.hpp"

int main()
{
    /* 1. Example */
    {
        IocContainer::GetSingleton().RegisterService<ILogger, FileLogger>();
        IocContainer::GetSingleton().RegisterService<IDecoder, HardwareDecoder, ILogger>();
        IocContainer::GetSingleton().RegisterService<IVideoPlayer, VideoPlayer, IDecoder>();
        auto video_player = IocContainer::GetSingleton().GetService<IVideoPlayer>();

        std::vector<uint16_t> frame_data = {0x10, 0x11, 0x12, 0x13};
        video_player->PresentFrame(frame_data);
    }

    /* Restore the IocContainer to the default values */
    IocContainer::GetSingleton().RemoveService<ILogger>();
    IocContainer::GetSingleton().RemoveService<IDecoder>();
    IocContainer::GetSingleton().RemoveService<IVideoPlayer>();

    std::cout << std::endl;

    /* 2. Circular dependency: Logger depends on VideoPlayer */
    {
        IocContainer::GetSingleton().RegisterService<ILogger, LoggerBadImpl, IVideoPlayer>();
        IocContainer::GetSingleton().RegisterService<IDecoder, HardwareDecoder, ILogger>();
        IocContainer::GetSingleton().RegisterService<IVideoPlayer, VideoPlayer, IDecoder>();

        try
        {
            auto video_player = IocContainer::GetSingleton().GetService<IVideoPlayer>();
        }
        catch(const CircularDependency& exception)
        {
            std::cout << exception.what() << std::endl;
        }
    }
    return 0;
}
