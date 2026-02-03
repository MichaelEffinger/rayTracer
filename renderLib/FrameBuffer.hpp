#pragma once


#include "ColorN.hpp"
#include <algorithm>
#include <utility>
#include <vector>
#include <string>


namespace ES{

    // row major probably I guess
    class FrameBuffer {
        
        std::vector<RGB> data_;
        std::size_t width_;
        std::size_t height_;  
        public:


        [[nodiscard]] auto begin() noexcept { return data_.begin(); }

        [[nodiscard]] auto begin() const noexcept { return data_.begin(); }

        [[nodiscard]] auto cbegin() const noexcept { return data_.cbegin(); }

        [[nodiscard]] auto end() noexcept { return data_.end(); }

        [[nodiscard]] auto end() const noexcept { return data_.end(); }
        
        [[nodiscard]] auto cend() const noexcept { return data_.cend(); }


        FrameBuffer() noexcept: width_(100),height_(100),data_(100*100){}

        FrameBuffer(std::size_t w, std::size_t h) noexcept : width_(w),height_(h),data_(w*h){}

        [[nodiscard]] auto&& operator[](this auto&& self, std::size_t index){
            return std::forward_like<decltype(self)>(self.data_[index]);
        }

        [[nodiscard]] auto&& operator()(this auto&& self, std::size_t row, std::size_t col) noexcept{
            return std::forward_like<decltype(self)>(self[(row*self.width() + col)]);
        }   

        constexpr FrameBuffer& fill(RGB color = RGB(0,0,0)) noexcept;


        [[nodiscard]] std::size_t width() const noexcept;

        FrameBuffer& set_width(std::size_t new_width) noexcept{
            width_ = new_width;
            data_.resize(width_*height_);
            return *this;
        }

        FrameBuffer& set_height(std::size_t new_height)noexcept{
            height_ = new_height;
            data_.resize(height_*width_);
            return *this;
        }

        FrameBuffer& set_height_width(std::size_t new_height, std::size_t new_width){
            width_ = new_width;
            height_ = new_height;
            data_.resize(height_*width_);
            return *this;
        }
    
        FrameBuffer& gradiant_up(RGB bottom, RGB top){
            const float inv_h = 1.0f / float(height_ - 1);
            std::size_t i = 0;
            std::transform(begin(), end(), begin(), [&](const RGB&) {std::size_t y = i / width_; float t = y * inv_h; ++i; return bottom.lerp(top, t); });
            return *this;
        }


        FrameBuffer& gradiant_right(RGB left, RGB right){
            const float inv_w = 1.0f/float(width_ - 1);
            std::size_t i = 0;
            std::transform(begin(), end(), begin(), [&](const RGB&) {std::size_t x = i % width_; float t = x * inv_w; ++i; return left.lerp(right, t); });
            return *this;
        }

        [[nodiscard]] std::size_t height() const noexcept;

        [[nodiscard]] std::size_t size() const noexcept;
    
    };







}