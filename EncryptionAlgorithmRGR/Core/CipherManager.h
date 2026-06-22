#ifndef CIPHERMANAGER_H
#define CIPHERMANAGER_H

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "ICipher.h"

class CipherManager {
private:
    std::map<std::string, std::shared_ptr<ICipher>> ciphers;
    std::map<std::string, void*> libraryHandles;

public:
    CipherManager() = default;
    ~CipherManager();

    bool loadPlugin(const std::string& pluginPath);
    std::vector<std::string> getAvailableCiphers() const;
    std::shared_ptr<ICipher> getCipher(const std::string& name) const;
    void unloadAll();
};

#endif
