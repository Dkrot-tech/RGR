#include "atbash.h"

char atbashChar(char c)
{
    if (c >= 'A' && c <= 'Z')
        return 'Z' - (c - 'A');

    if (c >= 'a' && c <= 'z')
        return 'z' - (c - 'a');

    return c;
}

std::string encryptAtbash(const std::string& text)
{
    std::string result = text;

    for (char& c : result)
    {
        c = atbashChar(c);
    }

    return result;
}

std::string decryptAtbash(const std::string& text)
{
    return encryptAtbash(text);
}