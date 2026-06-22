
#include "vigenere.h"
#include <algorithm>
#include <codecvt>
#include <iostream>
#include <vector>

VigenereCipher::VigenereCipher() : loc(std::locale("")) {}

VigenereCipher::VigenereCipher(const std::string& key) : loc(std::locale("")) {
    setKey(key);
}

std::wstring VigenereCipher::stringToWstring(const std::string& str) const {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    try {
        return converter.from_bytes(str);
    } catch (...) {
        // Если конвертация не удалась, возвращаем пустую строку
        return L"";
    }
}

std::string VigenereCipher::wstringToString(const std::wstring& wstr) const {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    try {
        return converter.to_bytes(wstr);
    } catch (...) {
        // Если конвертация не удалась, возвращаем пустую строку
        return "";
    }
}

bool VigenereCipher::isCyrillic(wchar_t c) const {
    return (c >= 0x0410 && c <= 0x044F) || c == 0x0401 || c == 0x0451;
}

bool VigenereCipher::isLatin(wchar_t c) const {
    return (c >= L'A' && c <= L'Z') || (c >= L'a' && c <= L'z');
}

bool VigenereCipher::isAlpha(wchar_t c) const {
    return isLatin(c) || isCyrillic(c);
}

wchar_t VigenereCipher::toUpper(wchar_t c) const {
    if (isLatin(c)) {
        if (c >= L'a' && c <= L'z') {
            return c - (L'a' - L'A');
        }
        return c;
    }
    if (isCyrillic(c)) {
        if (c >= 0x0430 && c <= 0x044F) {
            return c - 0x20;
        }
        if (c == 0x0451) return 0x0401;
        return c;
    }
    return c;
}

bool VigenereCipher::isKeyValid(const std::wstring& key) const {
    if (key.empty()) return false;
    for (wchar_t c : key) {
        if (!isAlpha(c)) {
            return false;
        }
    }
    return true;
}

std::vector<int> VigenereCipher::getKeyOffsets(const std::wstring& key) const {
    std::vector<int> offsets;
    for (wchar_t c : key) {
        wchar_t upper = toUpper(c);
        if (isLatin(upper)) {
            offsets.push_back(upper - L'A');
        } else if (isCyrillic(upper)) {
            offsets.push_back(upper - 0x0410);
        } else {
            offsets.push_back(0);
        }
    }
    return offsets;
}

ErrorCode VigenereCipher::setKey(const std::string& newKey) {
    std::wstring wKey = stringToWstring(newKey);
    if (wKey.empty() || !isKeyValid(wKey)) {
        return ErrorCode::INVALID_KEY;
    }
    key = wKey;
    return ErrorCode::SUCCESS;
}

std::string VigenereCipher::getKey() const {
    return wstringToString(key);
}

bool VigenereCipher::isKeySet() const {
    return !key.empty();
}

wchar_t VigenereCipher::shiftChar(wchar_t c, int offset, bool encrypt) const {
    if (!isAlpha(c)) {
        return c;
    }

    wchar_t base;
    int alphabetSize;
    wchar_t upper = toUpper(c);
    
    if (isLatin(upper)) {
        base = L'A';
        alphabetSize = 26;
    } else if (isCyrillic(upper)) {
        base = 0x0410;
        alphabetSize = 32;
    } else {
        return c;
    }

    int shift = encrypt ? offset : (alphabetSize - offset);
    wchar_t result = static_cast<wchar_t>((upper - base + shift) % alphabetSize + base);
    
    // Восстанавливаем регистр
    if (isLatin(c)) {
        if (c >= L'a' && c <= L'z') {
            result = result - L'A' + L'a';
        }
    } else if (isCyrillic(c)) {
        if (c >= 0x0430 && c <= 0x044F) {
            result = result - 0x0410 + 0x0430;
        }
        if (c == 0x0451) {
            if (result == 0x0401) return 0x0451;
        }
    }
    
    return result;
}

ErrorCode VigenereCipher::encrypt(const std::string& plaintext, std::string& ciphertext) const {
    if (plaintext.empty()) {
        return ErrorCode::EMPTY_TEXT;
    }
    if (!isKeySet()) {
        return ErrorCode::INVALID_KEY;
    }

    std::wstring wPlaintext = stringToWstring(plaintext);
    if (wPlaintext.empty()) {
        return ErrorCode::EMPTY_TEXT;
    }
    
    std::vector<int> offsets = getKeyOffsets(key);
    std::wstring wCiphertext;
    wCiphertext.reserve(wPlaintext.length());

    size_t keyIndex = 0;
    for (wchar_t c : wPlaintext) {
        if (isAlpha(c)) {
            wchar_t processed = shiftChar(c, offsets[keyIndex % offsets.size()], true);
            wCiphertext.push_back(processed);
            keyIndex++;
        } else {
            wCiphertext.push_back(c);
        }
    }

    ciphertext = wstringToString(wCiphertext);
    return ErrorCode::SUCCESS;
}

ErrorCode VigenereCipher::decrypt(const std::string& ciphertext, std::string& plaintext) const {
    if (ciphertext.empty()) {
        return ErrorCode::EMPTY_TEXT;
    }
    if (!isKeySet()) {
        return ErrorCode::INVALID_KEY;
    }

    std::wstring wCiphertext = stringToWstring(ciphertext);
    if (wCiphertext.empty()) {
        return ErrorCode::EMPTY_TEXT;
    }
    
    std::vector<int> offsets = getKeyOffsets(key);
    std::wstring wPlaintext;
    wPlaintext.reserve(wCiphertext.length());

    size_t keyIndex = 0;
    for (wchar_t c : wCiphertext) {
        if (isAlpha(c)) {
            wchar_t processed = shiftChar(c, offsets[keyIndex % offsets.size()], false);
            wPlaintext.push_back(processed);
            keyIndex++;
        } else {
            wPlaintext.push_back(c);
        }
    }

    plaintext = wstringToString(wPlaintext);
    return ErrorCode::SUCCESS;
}