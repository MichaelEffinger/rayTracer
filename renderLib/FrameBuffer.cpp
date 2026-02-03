#include "FrameBuffer.hpp"    
    
namespace ES{

    [[nodiscard]] std::size_t FrameBuffer::size() const noexcept{
        return width_*height_;
    }
    constexpr FrameBuffer& FrameBuffer::fill(RGB color) noexcept{
        std::fill(begin(),end(),color);
        return *this;
     }

    [[nodiscard]] std::size_t FrameBuffer::width() const noexcept{
        return width_;
    }

    [[nodiscard]] std::size_t FrameBuffer::height() const noexcept{
        return height_;
    }


}