
#include "key_generator.h"
#include <random>
#include <chrono>
#include <cctype>
#include <algorithm>
#include <clocale>
#include <vector>

char KeyGenerator::getRandomLetter() {
    static std::mt19937 rng(static_cast<unsigned>(
        std::chrono::steady_clock::now().time_since_epoch().count()
    ));
    static std::uniform_int_distribution<int> dist(0, 25);
    
    return static_cast<char>('a' + dist(rng));
}

std::string KeyGenerator::getRandomCyrillicLetter() {
    static std::mt19937 rng(static_cast<unsigned>(
        std::chrono::steady_clock::now().time_since_epoch().count()
    ));
    static std::uniform_int_distribution<int> dist(0, 31);
    
    // Русские буквы в UTF-8 (заглавные)
    // А-Я (без Ё)
    std::vector<std::string> cyrillic = {
        "А", "Б", "В", "Г", "Д", "Е", "Ж", "З", "И", "Й",
        "К", "Л", "М", "Н", "О", "П", "Р", "С", "Т", "У",
        "Ф", "Х", "Ц", "Ч", "Ш", "Щ", "Ъ", "Ы", "Ь", "Э", "Ю", "Я"
    };
    
    return cyrillic[dist(rng)];
}

char KeyGenerator::getRandomUpperLetter() {
    static std::mt19937 rng(static_cast<unsigned>(
        std::chrono::steady_clock::now().time_since_epoch().count()
    ));
    static std::uniform_int_distribution<int> dist(0, 25);
    
    return static_cast<char>('A' + dist(rng));
}

int KeyGenerator::getRandomNumber(int min, int max) {
    static std::mt19937 rng(static_cast<unsigned>(
        std::chrono::steady_clock::now().time_since_epoch().count()
    ));
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

ErrorCode KeyGenerator::generateRandomKey(int length, std::string& key) {
    if (length <= 0) {
        return ErrorCode::KEY_GENERATION_FAILED;
    }

    key.clear();
    key.reserve(length);
    
    for (int i = 0; i < length; ++i) {
        key.push_back(getRandomLetter());
    }

    return ErrorCode::SUCCESS;
}

ErrorCode KeyGenerator::generateVigenereKey(int length, std::string& key) {
    if (length <= 0) {
        return ErrorCode::KEY_GENERATION_FAILED;
    }

    key.clear();
    
    // Генерируем ключ из латиницы и кириллицы
    for (int i = 0; i < length; ++i) {
        if (getRandomNumber(0, 1) == 0) {
            // Латиница (1 символ)
            key.push_back(getRandomUpperLetter());
        } else {
            // Кириллица (2 символа в UTF-8)
            key += getRandomCyrillicLetter();
        }
    }

    return ErrorCode::SUCCESS;
}

ErrorCode KeyGenerator::generateKeyFromSeed(const std::string& seed, int length, std::string& key) {
    if (length <= 0) {
        return ErrorCode::KEY_GENERATION_FAILED;
    }

    key.clear();
    key.reserve(length);

    std::mt19937 rng(static_cast<unsigned>(
        std::hash<std::string>{}(seed)
    ));
    std::uniform_int_distribution<int> dist(0, 25);

    for (int i = 0; i < length; ++i) {
        key.push_back(static_cast<char>('A' + dist(rng)));
    }

    return ErrorCode::SUCCESS;
}

bool KeyGenerator::isValidKey(const std::string& key) {
    if (key.empty()) return false;
    
    // Проверяем, что все символы - буквы (латиница или кириллица)
    for (size_t i = 0; i < key.length(); ++i) {
        unsigned char c = static_cast<unsigned char>(key[i]);
        
        // Проверка на латиницу
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            continue;
        }
        
        // Проверка на кириллицу (UTF-8)
        // Русские буквы в UTF-8: первый байт 0xD0 или 0xD1
        if (c == 0xD0 || c == 0xD1) {
            if (i + 1 < key.length()) {
                unsigned char c2 = static_cast<unsigned char>(key[i + 1]);
                // Диапазон для русских букв
                if ((c2 >= 0x80 && c2 <= 0xBF)) {
                    i++; // пропускаем второй байт
                    continue;
                }
            }
        }
        
        return false;
    }
    return true;
}