#include "../../Core/ICipher.h"
#include <vector>
#include <random>
#include <stdexcept>
#include <cctype>

class AtbashCipher : public ICipher {
private:
    std::string name = "Atbash";

    // Atbash для латиницы (A↔Z, B↔Y, ...)
    char atbashLatin(char c) {
        if (c >= 'A' && c <= 'Z') {
            return static_cast<char>('Z' - (c - 'A'));
        }
        if (c >= 'a' && c <= 'z') {
            return static_cast<char>('z' - (c - 'a'));
        }
        return c;
    }

    // Atbash для русских букв (А↔Я, Б↔Ю, ...)
    std::string atbashRussian(unsigned char c1, unsigned char c2) {
        // Прописные: А-Я (0xD0 0x90-0xBF)
        if (c1 == 0xD0 && c2 >= 0x90 && c2 <= 0xBF) {
            int idx = c2 - 0x90;  // 0-47
            int newIdx = 47 - idx; // зеркальное отображение
            std::vector<unsigned char> result(2);
            result[0] = 0xD0;
            result[1] = 0x90 + newIdx;
            return std::string(result.begin(), result.end());
        }
        // Строчные: а-я (0xD1 0x80-0x9F)
        else if (c1 == 0xD1 && c2 >= 0x80 && c2 <= 0x9F) {
            int idx = c2 - 0x80;  // 0-31
            int newIdx = 47 - idx; // зеркальное отображение (0-47)
            std::vector<unsigned char> result(2);
            result[0] = 0xD1;
            result[1] = 0x80 + newIdx;
            return std::string(result.begin(), result.end());
        }
        return std::string(1, c1) + std::string(1, c2);
    }

    // Проверка, является ли байт началом русской буквы
    bool isRussianByte(unsigned char c1, unsigned char c2) {
        return (c1 == 0xD0 && c2 >= 0x90 && c2 <= 0xBF) ||
               (c1 == 0xD1 && c2 >= 0x80 && c2 <= 0x9F);
    }

public:
    std::string getName() const override { return name; }

    // Основная функция для текста (работает с буквами)
    std::string processText(const std::string& data, const std::string& key) {
        std::string result = "";
        size_t i = 0;
        
        while (i < data.length()) {
            unsigned char c1 = static_cast<unsigned char>(data[i]);
            
            // Проверяем русскую букву (2 байта)
            if (i + 1 < data.length() && isRussianByte(c1, data[i + 1])) {
                unsigned char c2 = static_cast<unsigned char>(data[i + 1]);
                std::string atbashed = atbashRussian(c1, c2);
                result += atbashed;
                i += 2;
                continue;
            }
            
            // Латинская буква (1 байт)
            if (isalpha(c1)) {
                result += atbashLatin(c1);
                i++;
                continue;
            }
            
            // Остальные символы (цифры, знаки) - оставляем как есть
            result += c1;
            i++;
        }
        
        return result;
    }

    // Для текста (пункт 2)
    std::string encrypt(const std::string& data, const std::string& key) override {
        return processText(data, key);
    }

    std::string decrypt(const std::string& data, const std::string& key) override {
        return processText(data, key);
    }

    // Для файлов (пункт 3) - используем XOR с ключом
    std::vector<unsigned char> encryptBytes(
        const std::vector<unsigned char>& data, 
        const std::string& key) override {
        
        if (key.empty()) throw std::runtime_error("Ключ не может быть пустым!");
        
        std::vector<unsigned char> result = data;
        unsigned char k = static_cast<unsigned char>(key[0]);
        
        for (auto& byte : result) {
            byte = byte ^ k;
        }
        return result;
    }

    std::vector<unsigned char> decryptBytes(
        const std::vector<unsigned char>& data, 
        const std::string& key) override {
        
        return encryptBytes(data, key); // XOR симметричен
    }

    // Генерация ключа
    std::string generateKey() override {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 255);
        return std::string(1, static_cast<char>(dis(gen)));
    }

    bool isValidKey(const std::string& key) override {
        return !key.empty();
    }
};

extern "C" {
    ICipher* createCipher() {
        return new AtbashCipher();
    }

    void destroyCipher(ICipher* cipher) {
        delete cipher;
    }
}
