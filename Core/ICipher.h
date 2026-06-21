#ifndef ICIPHER_H
#define ICIPHER_H

#include <string>
#include <vector>

class ICipher {
public:
    virtual ~ICipher() = default;

    virtual std::string getName() const = 0;
    virtual std::string encrypt(const std::string& data, const std::string& key) = 0;
    virtual std::string decrypt(const std::string& data, const std::string& key) = 0;
    
    virtual std::vector<unsigned char> encryptBytes(
        const std::vector<unsigned char>& data, 
        const std::string& key) = 0;
    
    virtual std::vector<unsigned char> decryptBytes(
        const std::vector<unsigned char>& data, 
        const std::string& key) = 0;

    virtual std::string generateKey() = 0;
    virtual bool isValidKey(const std::string& key) = 0;
};

extern "C" {
    ICipher* createCipher();
    void destroyCipher(ICipher* cipher);
}

#endif
