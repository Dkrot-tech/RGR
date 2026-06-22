#include "../../Core/ICipher.h"
#include <vector>
#include <random>
#include <chrono>
#include <stdexcept>  // ← ЭТО ДОБАВЛЯЕМ!

class XORCipher : public ICipher {
private:
    std::string name = "XOR";

public:
    std::string getName() const override { return name; }

    std::string encrypt(const std::string& data, const std::string& key) override {
        if (key.empty()) throw std::runtime_error("Ключ не может быть пустым!");
        
        std::string result = data;
        unsigned char k = static_cast<unsigned char>(key[0]);
        
        for (size_t i = 0; i < result.size(); ++i) {
            result[i] = static_cast<char>(static_cast<unsigned char>(result[i]) ^ k);
        }
        return result;
    }

    std::string decrypt(const std::string& data, const std::string& key) override {
        return encrypt(data, key);
    }

    std::vector<unsigned char> encryptBytes(
        const std::vector<unsigned char>& data, 
        const std::string& key) override {
        
        if (key.empty()) throw std::runtime_error("Ключ не может быть пустым!");
        
        std::vector<unsigned char> result = data;
        unsigned char k = static_cast<unsigned char>(key[0]);
        
        for (auto& byte : result) {
            byte ^= k;
        }
        return result;
    }

    std::vector<unsigned char> decryptBytes(
        const std::vector<unsigned char>& data, 
        const std::string& key) override {
        return encryptBytes(data, key);
    }

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
        return new XORCipher();
    }

    void destroyCipher(ICipher* cipher) {
        delete cipher;
    }
}
