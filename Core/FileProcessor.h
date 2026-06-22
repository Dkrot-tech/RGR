#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

#include <string>
#include <vector>
#include <memory>
#include "ICipher.h"

class FileProcessor {
public:
    static bool processFile(
        const std::string& inputPath,
        const std::string& outputPath,
        std::shared_ptr<ICipher> cipher,
        const std::string& key,
        bool encrypt
    );

    static bool fileExists(const std::string& path);
    static bool createDirectories(const std::string& path);
};

#endif
