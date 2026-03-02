#include <iostream>
#include <fstream>
#include <vector>

inline void readFloatsFromFile(const std::string& filename, std::vector<float> &allFloats)
{
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file\n";
        return;
    }

    float value;
    char c;

    while (file >> value) {
        allFloats.push_back(value);

        // Consume comma if there
        if (file.peek() == ',') file.get(c);
    }
}
