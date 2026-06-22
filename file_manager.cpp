#include "file_manager.h"

#include <fstream>
#include <locale>
#include <stdexcept>

std::wstring readFile(const std::string& filename)
{
    std::wifstream file(filename);

    if (!file.is_open())
        throw std::runtime_error("Cannot open file");

    file.imbue(std::locale(""));

    std::wstring text;
    std::wstring line;

    while (std::getline(file, line))
    {
        text += line;
        text += L'\n';
    }

    return text;
}

void writeFile(const std::string& filename,
               const std::wstring& content)
{
    std::wofstream file(filename);

    if (!file.is_open())
        throw std::runtime_error("Cannot create file");

    file.imbue(std::locale(""));

    file << content;
}