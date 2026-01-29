#include "ColorN.hpp"
#include <vector>
#include <string>


namespace ES{

    // row major probably I guess
    class FrameBuffer {
        
        std::vector<RGB> data_;
        std::size_t width_;
        std::size_t height_;  
        public:


        /** @brief Returns iterator to first element. See `cbegin` for const iterators. */
        [[nodiscard]] auto begin() noexcept;
        /** @overload */
        [[nodiscard]] auto begin() const noexcept;
        /** @overload */
        [[nodiscard]] auto cbegin() const noexcept;
        /** @brief Returns iterator past the last element. See `cend` for const iterators. */
        [[nodiscard]] auto end() noexcept;
        /** @overload */
        [[nodiscard]] auto end() const noexcept;
        /** @overload */
        [[nodiscard]] auto cend() const noexcept;
        /** @} */

        FrameBuffer() noexcept: width_(100),height_(100),data_(100*100){}

        FrameBuffer(std::size_t w, std::size_t h) noexcept : width_(w),height_(h),data_(w*h){}

        
        [[nodiscard]] auto&& operator()(this auto&& self, std::size_t row, std::size_t col) noexcept;

        constexpr FrameBuffer& fill(RGB color = RGB(0,0,0)) noexcept;


        [[nodiscard]] std::size_t width() const noexcept;

        [[nodiscard]] std::size_t height() const noexcept;

        [[nodiscard]] std::size_t size() const noexcept;

        FrameBuffer& export_as_PNG(std::string filename);
        
   
    };







}