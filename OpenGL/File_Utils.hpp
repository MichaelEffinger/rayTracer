// File_utils.hpp
#pragma once
#include <string>
#include <fstream>
#include <sstream>

namespace ES {
    inline std::string loadFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) return "";
        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }
}