#include <iostream>
#include <string>
#include <locale>

#include "atbash.h"
#include "file_manager.h"
#include "key_generator.h"

int main()
{
    std::locale::global(std::locale(""));

    std::cout << "=== Atbash Cipher Program ===\n";

    std::string inputFile;
    std::string outputFile;

    std::cout << "Enter input file path: ";
    std::getline(std::cin, inputFile);

    std::cout << "Enter output file path: ";
    std::getline(std::cin, outputFile);

    try
    {
        std::wstring text = readFile(inputFile);

        std::string key = generateKey();

        std::cout << "Generated key: " << key << std::endl;

        std::wstring result = encryptAtbash(text);

        writeFile(outputFile, result);

        std::cout << "Done successfully!" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}