#include "../../Core/ICipher.h"
#include <vector>
#include <random>
#include <stdexcept>
#include <cctype>

class VigenereCipher : public ICipher {
private:
    std::string name = "Vigenere";

    // Получить числовой сдвиг из ключа (по позиции)
    int getKeyShift(const std::string& key, size_t pos) {
        if (key.empty()) return 0;
        unsigned char ch = key[pos % key.length()];
        return static_cast<int>(ch) % 256;
    }

public:
    std::string getName() const override { return name; }

    // Универсальная функция для шифрования/дешифрования байтов
    std::string processData(const std::string& data, const std::string& key, bool encrypt) {
        if (key.empty()) throw std::runtime_error("Ключ не может быть пустым!");
        
        std::string result = data;
        size_t keyPos = 0;
        
        for (size_t i = 0; i < result.size(); ++i) {
            unsigned char byte = static_cast<unsigned char>(result[i]);
            int shift = getKeyShift(key, keyPos);
            
            if (encrypt) {
                result[i] = static_cast<char>(byte ^ shift);
            } else {
                result[i] = static_cast<char>(byte ^ shift);
            }
            
            // XOR симметричен, поэтому шифрование и дешифрование одинаковы
            // Но для Виженера мы используем XOR для совместимости с файлами
            keyPos++;
        }
        
        return result;
    }

    // Для текста
    std::string encrypt(const std::string& data, const std::string& key) override {
        return processData(data, key, true);
    }

    std::string decrypt(const std::string& data, const std::string& key) override {
        return processData(data, key, false);
    }

    // Для файлов
    std::vector<unsigned char> encryptBytes(
        const std::vector<unsigned char>& data, 
        const std::string& key) override {
        
        std::string input(data.begin(), data.end());
        std::string result = processData(input, key, true);
        return std::vector<unsigned char>(result.begin(), result.end());
    }

    std::vector<unsigned char> decryptBytes(
        const std::vector<unsigned char>& data, 
        const std::string& key) override {
        
        std::string input(data.begin(), data.end());
        std::string result = processData(input, key, false);
        return std::vector<unsigned char>(result.begin(), result.end());
    }

    // Генерация случайного ключа (строка из букв)
    std::string generateKey() override {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 25);
        
        // Ключ длиной от 5 до 10 символов
        int len = 5 + (gen() % 6);
        std::string key = "";
        for (int i = 0; i < len; ++i) {
            key += static_cast<char>('A' + dis(gen));
        }
        return key;
    }

    bool isValidKey(const std::string& key) override {
        return !key.empty();
    }
};

extern "C" {
    ICipher* createCipher() {
        return new VigenereCipher();
    }

    void destroyCipher(ICipher* cipher) {
        delete cipher;
    }
}
