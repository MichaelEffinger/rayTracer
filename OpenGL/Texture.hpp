#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "GLSL.h"
#include "png++/png.hpp"

namespace ES {

    enum class TextureType {
        DIFFUSE,
        SPECULAR,
        NORMAL,
        HEIGHT,
        CUBEMAP,
        NONE
    };

    class Texture {
        GLuint id = 0;

    public:
        int width = 0;
        int height = 0;
        int channels = 4;
        TextureType type;

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        explicit Texture(const std::string& source, TextureType t = TextureType::DIFFUSE) : type(t) {
            try {
                png::image<png::rgba_pixel> texPNGImage;

                // Peek at the color type first so we can convert appropriately
                png::image<png::rgb_pixel> testLoad;
                bool isRGBA = false;

                // Try loading as RGBA first, fall back to RGB and convert
                try {
                    png::image<png::rgba_pixel> rgbaImg(source);
                    texPNGImage = std::move(rgbaImg);
                    isRGBA = true;
                } catch (...) {
                    // Not RGBA - load as RGB and manually add alpha=255
                    png::image<png::rgb_pixel> rgbImg(source);
                    width  = rgbImg.get_width();
                    height = rgbImg.get_height();
                    texPNGImage.resize(width, height);
                    for (std::size_t y = 0; y < (std::size_t)height; y++) {
                        for (std::size_t x = 0; x < (std::size_t)width; x++) {
                            png::rgb_pixel src = rgbImg[y][x];
                            texPNGImage[y][x] = png::rgba_pixel(src.red, src.green, src.blue, 255);
                        }
                    }
                }

                width  = texPNGImage.get_width();
                height = texPNGImage.get_height();

                std::vector<unsigned char> data;
                data.reserve(width * height * 4);

                for (std::size_t y = 0; y < (std::size_t)height; y++) {
                    for (std::size_t x = 0; x < (std::size_t)width; x++) {
                        png::rgba_pixel pixel = texPNGImage[height - y - 1][x];
                        data.push_back(pixel.red);
                        data.push_back(pixel.green);
                        data.push_back(pixel.blue);
                        data.push_back(pixel.alpha);
                    }
                }

                glGenTextures(1, &id);
                glBindTexture(GL_TEXTURE_2D, id);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
                glGenerateMipmap(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, 0);

            } catch (const std::exception& e) {
                std::cerr << "Texture Load Error: " << source << " -> " << e.what() << std::endl;
                id = 0;
            }
        }
        
        Texture(Texture&& other) noexcept : id(other.id), width(other.width), height(other.height), channels(other.channels),type(other.type) {
            other.id = 0;
        }

        Texture& operator=(Texture&& other) noexcept {
            if (this != &other) {
                release();
                id = other.id;
                width = other.width;
                height = other.height;
                channels = other.channels;
                type = other.type;
                other.id = 0;
                other.width = 0;
                other.height = 0;
                other.channels = 0;
            }
            return *this;
        }

        void release() {
            if (id != 0) {
                glDeleteTextures(1, &id);
                id = 0;
            }
        }

        void bind(unsigned int unit = 0) const {
            if (id == 0) return; // Safety check
            glActiveTexture(GL_TEXTURE0 + unit);
            glBindTexture(GL_TEXTURE_2D, id);
        }

        void unbind() const {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        GLuint getID() const { return id; }

        ~Texture() {
            release();
        }


    Texture(const std::vector<std::string>& faces) : type(TextureType::CUBEMAP) {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);

        for (unsigned int i = 0; i < faces.size(); i++) {
            try {
                png::image<png::rgba_pixel> image(faces[i]);
                int w = image.get_width();
                int h = image.get_height();

                std::vector<unsigned char> data;
                data.reserve(w * h * 4);

                for (std::size_t y = 0; y < (std::size_t)h; y++) {
                    for (std::size_t x = 0; x < (std::size_t)w; x++) {
                        png::rgba_pixel pixel = image[y][x]; 
                        data.push_back(pixel.red);
                        data.push_back(pixel.green);
                        data.push_back(pixel.blue);
                        data.push_back(pixel.alpha);
                    }
                }

                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, 
                            w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
            } catch (const std::exception& e) {
                std::cerr << "CubeMap Load Error at face " << i << ": " << e.what() << std::endl;
            }
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }



    };
}