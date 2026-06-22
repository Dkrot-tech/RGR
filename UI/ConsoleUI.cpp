#include "../Core/CipherManager.h"
#include "../Core/FileProcessor.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

class ConsoleUI {
private:
    CipherManager manager;
    std::string lastEncrypted;  // Запоминаем последний зашифрованный текст

    void showMenu() {
        std::cout << "\n========================================\n";
        std::cout << "   Шифратор/Дешифратор Encryption RGR\n";
        std::cout << "========================================\n";
        std::cout << "1. Загрузить плагин\n";
        std::cout << "2. Шифровать/дешифровать текст\n";
        std::cout << "3. Шифровать/дешифровать файл\n";
        std::cout << "4. Сгенерировать ключ\n";
        std::cout << "5. Показать доступные алгоритмы\n";
        std::cout << "6. Выход\n";
        std::cout << "----------------------------------------\n";
        std::cout << "Выберите действие: ";
    }

    void handleText() {
        auto ciphers = manager.getAvailableCiphers();
        if (ciphers.empty()) {
            std::cout << "❌ Нет загруженных алгоритмов!\n";
            return;
        }

        std::cout << "\nДоступные алгоритмы:\n";
        for (size_t i = 0; i < ciphers.size(); ++i) {
            std::cout << "  " << i + 1 << ". " << ciphers[i] << std::endl;
        }

        std::cout << "Выберите номер: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice < 1 || choice > static_cast<int>(ciphers.size())) {
            std::cout << "❌ Неверный выбор!\n";
            return;
        }

        auto cipher = manager.getCipher(ciphers[choice - 1]);
        if (!cipher) {
            std::cout << "❌ Ошибка!\n";
            return;
        }

        std::cout << "Введите ключ (один символ): ";
        std::string key;
        std::getline(std::cin, key);
        
        if (key.empty()) {
            key = "K";
            std::cout << "Ключ не введён, использую 'K'\n";
        }

        std::cout << "Введите текст: ";
        std::string text;
        std::getline(std::cin, text);

        std::cout << "1 - шифровать, 2 - дешифровать: ";
        int action;
        std::cin >> action;
        std::cin.ignore();

        try {
            if (action == 1) {
                // Шифруем
                std::string encrypted = text;
                char k = key[0];
                for (size_t i = 0; i < encrypted.size(); ++i) {
                    encrypted[i] = encrypted[i] ^ k;
                }
                
                lastEncrypted = encrypted;  // ЗАПОМИНАЕМ
                
                std::cout << "\n✅ ЗАШИФРОВАНО:\n";
                std::cout << "----------------------------------------\n";
                std::cout << "Текст: " << encrypted << "\n";
                std::cout << "----------------------------------------\n";
                
                // Сохраняем в файл
                std::ofstream f("encrypted.txt");
                f << encrypted;
                f.close();
                std::cout << "📁 Сохранено в encrypted.txt\n";
                
                // Спрашиваем, расшифровать ли
                std::cout << "\n❓ Расшифровать этот же текст? (y/n): ";
                char answer;
                std::cin >> answer;
                std::cin.ignore();
                
                if (answer == 'y' || answer == 'Y') {
                    // Расшифровываем
                    std::string decrypted = lastEncrypted;
                    char k2 = key[0];
                    for (size_t i = 0; i < decrypted.size(); ++i) {
                        decrypted[i] = decrypted[i] ^ k2;
                    }
                    
                    std::cout << "\n✅ РАСШИФРОВАНО:\n";
                    std::cout << "----------------------------------------\n";
                    std::cout << decrypted << "\n";
                    std::cout << "----------------------------------------\n";
                }
                
            } else if (action == 2) {
                // Расшифровываем
                std::string decrypted = text;
                char k = key[0];
                for (size_t i = 0; i < decrypted.size(); ++i) {
                    decrypted[i] = decrypted[i] ^ k;
                }
                
                std::cout << "\n✅ РАСШИФРОВАНО:\n";
                std::cout << "----------------------------------------\n";
                std::cout << decrypted << "\n";
                std::cout << "----------------------------------------\n";
                
            } else {
                std::cout << "❌ Неверное действие!\n";
            }
        } catch (const std::exception& e) {
            std::cout << "❌ Ошибка: " << e.what() << std::endl;
        }
    }

    void handleFile() {
        auto ciphers = manager.getAvailableCiphers();
        if (ciphers.empty()) {
            std::cout << "❌ Нет загруженных алгоритмов!\n";
            return;
        }

        std::cout << "\nДоступные алгоритмы:\n";
        for (size_t i = 0; i < ciphers.size(); ++i) {
            std::cout << "  " << i + 1 << ". " << ciphers[i] << std::endl;
        }

        std::cout << "Выберите номер: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice < 1 || choice > static_cast<int>(ciphers.size())) {
            std::cout << "❌ Неверный выбор!\n";
            return;
        }

        auto cipher = manager.getCipher(ciphers[choice - 1]);
        if (!cipher) {
            std::cout << "❌ Ошибка!\n";
            return;
        }

        std::cout << "Введите ключ: ";
        std::string key;
        std::getline(std::cin, key);
        
        if (key.empty()) {
            key = "K";
            std::cout << "Ключ не введён, использую 'K'\n";
        }

        std::cout << "Входной файл: ";
        std::string inputPath;
        std::getline(std::cin, inputPath);

        std::cout << "Выходной файл: ";
        std::string outputPath;
        std::getline(std::cin, outputPath);

        if (!FileProcessor::fileExists(inputPath)) {
            std::cout << "❌ Файл не найден! Создать новый? (y/n): ";
            char answer;
            std::cin >> answer;
            std::cin.ignore();
            if (answer == 'y' || answer == 'Y') {
                std::ofstream f(inputPath);
                f << "Тестовый текст для шифрования";
                f.close();
                std::cout << "✅ Создан файл: " << inputPath << std::endl;
            } else {
                return;
            }
        }

        std::cout << "1 - шифровать, 2 - дешифровать: ";
        int action;
        std::cin >> action;
        std::cin.ignore();

        FileProcessor::processFile(inputPath, outputPath, cipher, key, action == 1);
    }

public:
    void run() {
        std::cout << "🔐 Добро пожаловать в Encryption Algorithm RGR!\n";
        std::cout << "📂 Загружаю плагины...\n";
        
        std::filesystem::create_directories("plugins");
        for (const auto& entry : std::filesystem::directory_iterator("plugins")) {
            if (entry.is_regular_file()) {
                manager.loadPlugin(entry.path().string());
            }
        }

        while (true) {
            showMenu();
            int choice;
            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
                case 1: {
                    std::cout << "Путь к плагину: ";
                    std::string path;
                    std::getline(std::cin, path);
                    manager.loadPlugin(path);
                    break;
                }
                case 2:
                    handleText();
                    break;
                case 3:
                    handleFile();
                    break;
                case 4: {
                    auto ciphers = manager.getAvailableCiphers();
                    if (ciphers.empty()) {
                        std::cout << "❌ Нет алгоритмов!\n";
                        break;
                    }
                    std::cout << "Выберите алгоритм:\n";
                    for (size_t i = 0; i < ciphers.size(); ++i) {
                        std::cout << "  " << i + 1 << ". " << ciphers[i] << std::endl;
                    }
                    int ch;
                    std::cin >> ch;
                    std::cin.ignore();
                    if (ch >= 1 && ch <= static_cast<int>(ciphers.size())) {
                        auto cipher = manager.getCipher(ciphers[ch - 1]);
                        std::string key = cipher->generateKey();
                        std::cout << "✅ Сгенерирован ключ: " << key << std::endl;
                    }
                    break;
                }
                case 5: {
                    auto ciphers = manager.getAvailableCiphers();
                    std::cout << "Загруженные алгоритмы:\n";
                    if (ciphers.empty()) {
                        std::cout << "  (нет)\n";
                    } else {
                        for (const auto& name : ciphers) {
                            std::cout << "  - " << name << std::endl;
                        }
                    }
                    break;
                }
                case 6:
                    std::cout << "До свидания!\n";
                    return;
                default:
                    std::cout << "❌ Неверный выбор!\n";
            }
        }
    }
};

int main() {
    try {
        ConsoleUI ui;
        ui.run();
    } catch (const std::exception& e) {
        std::cerr << "❌ Ошибка: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
