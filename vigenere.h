
#ifndef VIGENERE_H
#define VIGENERE_H

#include <string>
#include <vector>
#include <locale>
#include <codecvt>
#include "error_codes.h"

class VigenereCipher {
private:
    std::wstring key;
    std::locale loc;
    
    bool isKeyValid(const std::wstring& key) const;
    std::vector<int> getKeyOffsets(const std::wstring& key) const;
    wchar_t shiftChar(wchar_t c, int offset, bool encrypt) const;
    bool isCyrillic(wchar_t c) const;
    bool isLatin(wchar_t c) const;
    bool isAlpha(wchar_t c) const;
    wchar_t toUpper(wchar_t c) const;
    
    std::string wstringToString(const std::wstring& wstr) const;
    std::wstring stringToWstring(const std::string& str) const;

public:
    VigenereCipher();
    explicit VigenereCipher(const std::string& key);
    ErrorCode setKey(const std::string& newKey);
    std::string getKey() const;
    ErrorCode encrypt(const std::string& plaintext, std::string& ciphertext) const;
    ErrorCode decrypt(const std::string& ciphertext, std::string& plaintext) const;
    bool isKeySet() const;
};

#endif // VIGENERE_H