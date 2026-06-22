#include <iostream>
#include <string>
#include <limits>
#include <cctype>
#include <algorithm>
#include <clocale>

#include "vigenere.h"
#include "file_manager.h"
#include "key_generator.h"
#include "error_handler.h"

using namespace std;

void printMenu() {
    cout << "\n========================================\n";
    cout << "   ШИФР ВИЖЕНЕРА - ГЛАВНОЕ МЕНЮ\n";
    cout << "========================================\n";
    cout << "1. Зашифровать текст\n";
    cout << "2. Расшифровать текст\n";
    cout << "3. Зашифровать файл\n";
    cout << "4. Расшифровать файл\n";
    cout << "5. Сгенерировать ключ\n";
    cout << "6. Установить ключ\n";
    cout << "7. Показать текущий ключ\n";
    cout << "8. Включить/Выключить логирование ошибок\n";
    cout << "0. Выход\n";
    cout << "----------------------------------------\n";
    cout << "Выберите действие: ";
}

void printError(ErrorCode code, const string& context = "") {
    ErrorHandler::handleError(code, context);
}

string getInput(const string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    return input;
}

int getIntInput(const string& prompt) {
    string input;
    int value;
    while (true) {
        cout << prompt;
        getline(cin, input);
        try {
            value = stoi(input);
            if (value >= 0) return value;
            cout << "Введите положительное число.\n";
        } catch (...) {
            cout << "Введите корректное число.\n";
        }
    }
}

void displayResult(const string& result, const string& title) {
    cout << "\n----------------------------------------\n";
    cout << title << ":\n";
    cout << "----------------------------------------\n";
    cout << result << "\n";
    cout << "----------------------------------------\n";
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    cout << "========================================\n";
    cout << "   ШИФР ВИЖЕНЕРА\n";
    cout << "========================================\n";
    cout << "Поддерживается кириллица и латиница\n";
    cout << "========================================\n";

    VigenereCipher cipher;
    FileManager fileManager;
    bool running = true;
    bool loggingEnabled = true;

    while (running) {
        try {
            printMenu();
            string choiceStr;
            getline(cin, choiceStr);
            int choice = stoi(choiceStr);

            switch (choice) {
                case 0: {
                    cout << "Выход из программы. До свидания!\n";
                    running = false;
                    break;
                }

                case 1: {
                    if (!cipher.isKeySet()) {
                        cout << "Ключ не установлен. Сначала установите ключ.\n";
                        break;
                    }

                    string text = getInput("Введите текст для шифрования: ");
                    if (text.empty()) {
                        printError(ErrorCode::EMPTY_TEXT);
                        break;
                    }

                    string encrypted;
                    ErrorCode code = cipher.encrypt(text, encrypted);
                    if (code == ErrorCode::SUCCESS) {
                        displayResult(encrypted, "Зашифрованный текст");
                    } else {
                        printError(code);
                    }
                    break;
                }

                case 2: {
                    if (!cipher.isKeySet()) {
                        cout << "Ключ не установлен. Сначала установите ключ.\n";
                        break;
                    }

                    string text = getInput("Введите текст для расшифрования: ");
                    if (text.empty()) {
                        printError(ErrorCode::EMPTY_TEXT);
                        break;
                    }

                    string decrypted;
                    ErrorCode code = cipher.decrypt(text, decrypted);
                    if (code == ErrorCode::SUCCESS) {
                        displayResult(decrypted, "Расшифрованный текст");
                    } else {
                        printError(code);
                    }
                    break;
                }

                case 3: {
                    if (!cipher.isKeySet()) {
                        cout << "Ключ не установлен. Сначала установите ключ.\n";
                        break;
                    }

                    string inputPath = getInput("Введите путь к файлу для шифрования: ");
                    if (inputPath.empty()) {
                        printError(ErrorCode::INVALID_FILE_PATH);
                        break;
                    }

                    string content;
                    ErrorCode readCode = fileManager.readFile(inputPath, content);
                    if (readCode != ErrorCode::SUCCESS) {
                        printError(readCode, inputPath);
                        break;
                    }

                    string encrypted;
                    ErrorCode encryptCode = cipher.encrypt(content, encrypted);
                    if (encryptCode != ErrorCode::SUCCESS) {
                        printError(encryptCode);
                        break;
                    }

                    string outputPath = getInput("Введите путь для сохранения результата: ");
                    if (outputPath.empty()) {
                        printError(ErrorCode::INVALID_FILE_PATH);
                        break;
                    }

                    ErrorCode writeCode = fileManager.writeFile(outputPath, encrypted);
                    if (writeCode == ErrorCode::SUCCESS) {
                        cout << "Файл успешно зашифрован и сохранён по пути: " << outputPath << "\n";
                    } else {
                        printError(writeCode, outputPath);
                    }
                    break;
                }

                case 4: {
                    if (!cipher.isKeySet()) {
                        cout << "Ключ не установлен. Сначала установите ключ.\n";
                        break;
                    }

                    string inputPath = getInput("Введите путь к файлу для расшифрования: ");
                    if (inputPath.empty()) {
                        printError(ErrorCode::INVALID_FILE_PATH);
                        break;
                    }

                    string content;
                    ErrorCode readCode = fileManager.readFile(inputPath, content);
                    if (readCode != ErrorCode::SUCCESS) {
                        printError(readCode, inputPath);
                        break;
                    }

                    string decrypted;
                    ErrorCode decryptCode = cipher.decrypt(content, decrypted);
                    if (decryptCode != ErrorCode::SUCCESS) {
                        printError(decryptCode);
                        break;
                    }

                    string outputPath = getInput("Введите путь для сохранения результата: ");
                    if (outputPath.empty()) {
                        printError(ErrorCode::INVALID_FILE_PATH);
                        break;
                    }

                    ErrorCode writeCode = fileManager.writeFile(outputPath, decrypted);
                    if (writeCode == ErrorCode::SUCCESS) {
                        cout << "Файл успешно расшифрован и сохранён по пути: " << outputPath << "\n";
                    } else {
                        printError(writeCode, outputPath);
                    }
                    break;
                }

                case 5: {
                    int keyLength = getIntInput("Введите длину ключа: ");
                    if (keyLength <= 0) {
                        printError(ErrorCode::KEY_GENERATION_FAILED);
                        break;
                    }

                    string generatedKey;
                    ErrorCode code = KeyGenerator::generateVigenereKey(keyLength, generatedKey);
                    if (code == ErrorCode::SUCCESS) {
                        cout << "Сгенерированный ключ: " << generatedKey << "\n";
                        ErrorCode setCode = cipher.setKey(generatedKey);
                        if (setCode != ErrorCode::SUCCESS) {
                            printError(setCode);
                        } else {
                            cout << "Ключ успешно установлен.\n";
                        }
                    } else {
                        printError(code);
                    }
                    break;
                }

                case 6: {
                    string newKey = getInput("Введите новый ключ (буквы латиницы или кириллицы): ");
                    ErrorCode code = cipher.setKey(newKey);
                    if (code == ErrorCode::SUCCESS) {
                        cout << "Ключ успешно установлен.\n";
                    } else {
                        printError(code);
                    }
                    break;
                }

                case 7: {
                    if (cipher.isKeySet()) {
                        cout << "Текущий ключ: " << cipher.getKey() << "\n";
                    } else {
                        cout << "Ключ не установлен.\n";
                    }
                    break;
                }

                case 8: {
                    loggingEnabled = !loggingEnabled;
                    ErrorHandler::setLogging(loggingEnabled);
                    cout << "Логирование ошибок " << (loggingEnabled ? "включено" : "выключено") << "\n";
                    break;
                }

                default: {
                    cout << "Неверный выбор. Попробуйте снова.\n";
                    break;
                }
            }
        } catch (const exception& e) {
            ErrorHandler::handleError(ErrorCode::UNKNOWN_ERROR, e.what());
            cout << "Произошла ошибка. Попробуйте снова.\n";
        } catch (...) {
            ErrorHandler::handleError(ErrorCode::UNKNOWN_ERROR, "неизвестное исключение");
            cout << "Произошла неизвестная ошибка. Попробуйте снова.\n";
        }
    }

    return 0;
}
