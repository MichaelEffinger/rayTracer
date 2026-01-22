#pragma once

#include "ColorN.hpp"

#include <iostream>

namespace ES{
    
    inline void write_color(std::ostream& out, const ES::RGB& pixel_color) {
        auto r = pixel_color.R();
        auto g = pixel_color.G();
        auto b = pixel_color.B();

        // Translate the [0,1] component values to the byte range [0,255].
        int rbyte = int(255.999 * r);
        int gbyte = int(255.999 * g);
        int bbyte = int(255.999 * b);

        // Write out the pixel color components.
        out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
    }
}
