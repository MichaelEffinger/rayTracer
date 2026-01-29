#include "FrameBuffer.hpp"    
    
namespace ES{

    [[nodiscard]] auto FrameBuffer::begin() noexcept { return data_.begin(); }

    [[nodiscard]] auto FrameBuffer::begin() const noexcept { return data_.begin(); }

    [[nodiscard]] auto FrameBuffer::cbegin() const noexcept { return data_.cbegin(); }

    [[nodiscard]] auto FrameBuffer::end() noexcept { return data_.end(); }

    [[nodiscard]] auto FrameBuffer::end() const noexcept { return data_.end(); }
    
    [[nodiscard]] auto FrameBuffer::cend() const noexcept { return data_.cend(); }


    [[nodiscard]] std::size_t FrameBuffer::size() const noexcept{
        return width_*height_;
    }

    [[nodiscard]] auto&& FrameBuffer::operator()(this auto&& self, std::size_t row, std::size_t col) noexcept{
        return std::forward_like<decltype(self)>(self[(row*self.width() + col)]);
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