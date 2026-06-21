
#ifndef KEY_GENERATOR_H
#define KEY_GENERATOR_H

#include <string>
#include "error_codes.h"

class KeyGenerator {
public:
    static ErrorCode generateRandomKey(int length, std::string& key);
    static ErrorCode generateVigenereKey(int length, std::string& key);
    static ErrorCode generateKeyFromSeed(const std::string& seed, int length, std::string& key);
    static bool isValidKey(const std::string& key);

private:
    static char getRandomLetter();
    static std::string getRandomCyrillicLetter();
    static char getRandomUpperLetter();
    static int getRandomNumber(int min, int max);
};

#endif 