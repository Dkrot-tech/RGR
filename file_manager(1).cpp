#include "file_manager.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

std::string readFile(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        throw std::runtime_error("Error");
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

void writeFile(const std::string& filename, const std::string& content)
{
    std::ofstream file(filename);

    if (!file.is_open())
    {
        throw std::runtime_error("Error");
    }

    file << content;
}
