#pragma once

#include "ColorN.hpp"

#include <iostream>
#include "Ray.hpp"
#include "FrameBuffer.hpp"
#include "../src/handleGraphicsArgs.h"
#include "../src/png++/png.hpp"

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

    inline ES::RGB ray_to_color(const ES::Ray<float>& r) {
        ES::Vector3<float> dir = r.direction.normalize();
        return ES::RGB{0.5f * (dir.x() + 1.0f), 0.5f * (dir.y() + 1.0f), 0.5f * (dir.z() + 1.0f)};
    }

inline void export_as_PNG(const ES::FrameBuffer& fb, const std::string& filename) {
    int w = fb.width();
    int h = fb.height();

    png::image<png::rgb_pixel> imData(w, h);

    for (size_t y = 0; y < h; ++y) {
        for (size_t x = 0; x < w; ++x) {
            RGB8 col(fb(y, x));

            // RGB_Int col = RGB_Int::from_linear(fb(y,x).R(),fb(y,x).G(), fb(y,x).B());
            imData[h - 1 - y][x] = png::rgb_pixel(col.R(), col.G(), col.B());
        }
    }

    imData.write(filename);
}



}
