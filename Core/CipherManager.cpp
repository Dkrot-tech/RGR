#include "CipherManager.h"
#include <iostream>
#include <filesystem>
#include <dlfcn.h>

CipherManager::~CipherManager() {
    // Просто очищаем, без вызова dlclose
    ciphers.clear();
    libraryHandles.clear();
}

bool CipherManager::loadPlugin(const std::string& pluginPath) {
    if (!std::filesystem::exists(pluginPath)) {
        std::cerr << "❌ Плагин не найден: " << pluginPath << std::endl;
        return false;
    }

    void* handle = dlopen(pluginPath.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "❌ Не удалось загрузить библиотеку: " << dlerror() << std::endl;
        return false;
    }

    using CreateCipherFunc = ICipher*(*)();
    using DestroyCipherFunc = void(*)(ICipher*);

    auto createFunc = (CreateCipherFunc)dlsym(handle, "createCipher");
    auto destroyFunc = (DestroyCipherFunc)dlsym(handle, "destroyCipher");

    if (!createFunc || !destroyFunc) {
        std::cerr << "❌ Библиотека не соответствует интерфейсу ICipher" << std::endl;
        dlclose(handle);
        return false;
    }

    ICipher* cipher = createFunc();
    std::string name = cipher->getName();

    ciphers[name] = std::shared_ptr<ICipher>(cipher, [destroyFunc](ICipher* c) {
        destroyFunc(c);
    });
    libraryHandles[name] = handle;

    std::cout << "✅ Плагин загружен: " << name << std::endl;
    return true;
}

std::vector<std::string> CipherManager::getAvailableCiphers() const {
    std::vector<std::string> names;
    for (const auto& [name, _] : ciphers) {
        names.push_back(name);
    }
    return names;
}

std::shared_ptr<ICipher> CipherManager::getCipher(const std::string& name) const {
    auto it = ciphers.find(name);
    if (it != ciphers.end()) {
        return it->second;
    }
    return nullptr;
}

void CipherManager::unloadAll() {
    // Пустая функция - ничего не делаем
    ciphers.clear();
    libraryHandles.clear();
}
