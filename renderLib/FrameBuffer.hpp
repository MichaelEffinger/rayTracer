

#include "ContainerN.hpp"
#include "ColorN.hpp"


namespace ES{

    // row major probably I guess
    template<std::size_t N, std::size_t M>
    class FrameBuffer: public ContainerN<FrameBuffer<N,M>,RGB,N*M>{
        
        using ContainerN<FrameBuffer<N,M>,RGB,N*M>::ContainerN;
        using ContainerN<FrameBuffer<N,M>,RGB,N>::begin;
        using ContainerN<FrameBuffer<N,M>,RGB,N>::end;
        using ContainerN<FrameBuffer<N,M>,RGB,N>::cend;
        using ContainerN<FrameBuffer<N,M>,RGB,N>::data;
        using ContainerN<FrameBuffer<N,M>,RGB,N>::cbegin;
   

        constexpr RGB operator()()const noexcept{}   
        constexpr FrameBuffer& clear() noexcept{
            std::fill(begin(),end(),RGB(0,0,0));
            return *this;
        }

   
    };







}