#include "atbash.h"

wchar_t atbashChar(wchar_t c)
{
    if (c >= L'A' && c <= L'Z')
        return L'Z' - (c - L'A');

    if (c >= L'a' && c <= L'z')
        return L'z' - (c - L'a');

    if (c >= L'А' && c <= L'Я')
        return L'Я' - (c - L'А');

    if (c >= L'а' && c <= L'я')
        return L'я' - (c - L'а');

    return c;
}

std::wstring encryptAtbash(const std::wstring& text)
{
    std::wstring result = text;

    for (wchar_t& c : result)
        c = atbashChar(c);

    return result;
}

std::wstring decryptAtbash(const std::wstring& text)
{
    return encryptAtbash(text);
}