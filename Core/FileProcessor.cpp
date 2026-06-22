#include "FileProcessor.h"
#include <fstream>
#include <iostream>
#include <filesystem>

bool FileProcessor::processFile(
    const std::string& inputPath,
    const std::string& outputPath,
    std::shared_ptr<ICipher> cipher,
    const std::string& key,
    bool encrypt) {
    
    try {
        if (!fileExists(inputPath)) {
            std::cerr << "❌ Входной файл не существует: " << inputPath << std::endl;
            return false;
        }

        std::filesystem::path outPath(outputPath);
        if (outPath.has_parent_path() && !std::filesystem::exists(outPath.parent_path())) {
            std::cout << "📁 Создаю директории: " << outPath.parent_path() << std::endl;
            std::filesystem::create_directories(outPath.parent_path());
        }

        std::ifstream inFile(inputPath, std::ios::binary);
        if (!inFile.is_open()) {
            std::cerr << "❌ Не удалось открыть входной файл: " << inputPath << std::endl;
            return false;
        }

        std::ofstream outFile(outputPath, std::ios::binary);
        if (!outFile.is_open()) {
            std::cerr << "❌ Не удалось создать выходной файл: " << outputPath << std::endl;
            inFile.close();
            return false;
        }

        std::vector<unsigned char> buffer(
            (std::istreambuf_iterator<char>(inFile)),
            std::istreambuf_iterator<char>()
        );
        inFile.close();

        std::vector<unsigned char> result;
        if (encrypt) {
            result = cipher->encryptBytes(buffer, key);
        } else {
            result = cipher->decryptBytes(buffer, key);
        }

        outFile.write(reinterpret_cast<const char*>(result.data()), result.size());
        outFile.close();

        std::cout << "✅ Файл обработан. Результат: " << outputPath << std::endl;
        std::cout << "   Размер: " << result.size() << " байт" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "❌ Ошибка при обработке файла: " << e.what() << std::endl;
        return false;
    }
}

bool FileProcessor::fileExists(const std::string& path) {
    return std::filesystem::exists(path);
}

bool FileProcessor::createDirectories(const std::string& path) {
    try {
        std::filesystem::path p(path);
        if (p.has_parent_path() && !std::filesystem::exists(p.parent_path())) {
            return std::filesystem::create_directories(p.parent_path());
        }
        return true;
    } catch (...) {
        return false;
    }
}
