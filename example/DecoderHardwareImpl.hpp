
#ifndef DECODER_HARDWARE_IMPL_HPP
#define DECODER_HARDWARE_IMPL_HPP

#include <memory>
#include <iostream>

#include "DecoderInterface.hpp"
#include "LoggerInterface.hpp"

class HardwareDecoder : public IDecoder
{
    public:
        HardwareDecoder(std::shared_ptr<ILogger> logger) : m_logger(logger)
        {
            std::cout << "Decoder [" << this << "] created" << std::endl;
        }

        ~HardwareDecoder()
        {
            std::cout << "Decoder [" << this << "] destroyed" << std::endl;
        }

        void DecodeData(const std::vector<uint16_t>& data) override
        {
            m_logger->Log("Hardware decoding... frame data[0] = " + std::to_string(data[0]));
        }
    private:
        std::shared_ptr<ILogger> m_logger;
};

#endif
