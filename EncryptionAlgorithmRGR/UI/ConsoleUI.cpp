#include "../Core/CipherManager.h"
#include "../Core/FileProcessor.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <limits>

class ConsoleUI {
private:
    CipherManager manager;
    std::string lastEncrypted;

    // Функция для безопасного ввода числа
    int safeInput() {
        int value;
        while (true) {
            std::cin >> value;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "[ОШИБКА] Введите число: ";
            } else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
        }
    }

    void showMenu() {
        std::cout << "\n=========================================\n";
        std::cout << "    Шифратор/Дешифратор Encryption RGR\n";
        std::cout << "=========================================\n";
        std::cout << "1. Загрузить плагин\n";
        std::cout << "2. Шифровать/дешифровать текст\n";
        std::cout << "3. Шифровать/дешифровать файл\n";
        std::cout << "4. Сгенерировать ключ\n";
        std::cout << "5. Показать доступные алгоритмы\n";
        std::cout << "6. Выход\n";
        std::cout << "-----------------------------------------\n";
        std::cout << "Выберите действие: ";
    }

    // Функция для получения ключа (вручную или генерация)
    std::string getKey(std::shared_ptr<ICipher> cipher) {
        std::cout << "1 - ввести ключ вручную, 2 - сгенерировать случайный ключ: ";
        int keyChoice = safeInput();
        
        std::string key;
        if (keyChoice == 2) {
            key = cipher->generateKey();
            std::cout << "[СГЕНЕРИРОВАН] Ключ: " << key << std::endl;
        } else {
            std::cout << "Введите ключ: ";
            std::getline(std::cin, key);
            if (key.empty()) {
                key = "K";
                std::cout << "[ПРЕДУПРЕЖДЕНИЕ] Ключ не введён, использую 'K'\n";
            }
        }
        return key;
    }

    void handleText() {
        auto ciphers = manager.getAvailableCiphers();
        if (ciphers.empty()) {
            std::cout << "[ОШИБКА] Нет загруженных алгоритмов!\n";
            return;
        }

        std::cout << "\nДоступные алгоритмы:\n";
        for (size_t i = 0; i < ciphers.size(); ++i) {
            std::cout << "  " << i + 1 << ". " << ciphers[i] << std::endl;
        }

        std::cout << "Выберите номер: ";
        int choice = safeInput();

        if (choice < 1 || choice > static_cast<int>(ciphers.size())) {
            std::cout << "[ОШИБКА] Неверный выбор!\n";
            return;
        }

        auto cipher = manager.getCipher(ciphers[choice - 1]);
        if (!cipher) {
            std::cout << "[ОШИБКА] Не удалось загрузить алгоритм!\n";
            return;
        }

        std::string key = getKey(cipher);

        if (!cipher->isValidKey(key)) {
            std::cout << "[ОШИБКА] Неверный ключ!\n";
            return;
        }

        std::cout << "Введите текст: ";
        std::string text;
        std::getline(std::cin, text);

        std::cout << "1 - шифровать, 2 - дешифровать: ";
        int action = safeInput();

        try {
            if (action == 1) {
                std::string encrypted = cipher->encrypt(text, key);
                lastEncrypted = encrypted;
                
                std::cout << "\n[ЗАШИФРОВАНО]\n";
                std::cout << "----------------------------------------\n";
                std::cout << encrypted << "\n";
                std::cout << "----------------------------------------\n";
                
                std::ofstream f("encrypted.txt");
                f << encrypted;
                f.close();
                std::cout << "[СОХРАНЕНО] В файл encrypted.txt\n";
                
                std::cout << "\nРасшифровать этот же текст? (y/n): ";
                char answer;
                std::cin >> answer;
                std::cin.ignore();
                
                if (answer == 'y' || answer == 'Y') {
                    std::string decrypted = cipher->decrypt(lastEncrypted, key);
                    
                    std::cout << "\n[РАСШИФРОВАНО]\n";
                    std::cout << "----------------------------------------\n";
                    std::cout << decrypted << "\n";
                    std::cout << "----------------------------------------\n";
                }
                
            } else if (action == 2) {
                std::string decrypted = cipher->decrypt(text, key);
                
                std::cout << "\n[РАСШИФРОВАНО]\n";
                std::cout << "----------------------------------------\n";
                std::cout << decrypted << "\n";
                std::cout << "----------------------------------------\n";
                
            } else {
                std::cout << "[ОШИБКА] Неверное действие!\n";
            }
        } catch (const std::exception& e) {
            std::cout << "[ОШИБКА] " << e.what() << std::endl;
        }
    }

    void handleFile() {
        auto ciphers = manager.getAvailableCiphers();
        if (ciphers.empty()) {
            std::cout << "[ОШИБКА] Нет загруженных алгоритмов!\n";
            return;
        }

        std::cout << "\nДоступные алгоритмы:\n";
        for (size_t i = 0; i < ciphers.size(); ++i) {
            std::cout << "  " << i + 1 << ". " << ciphers[i] << std::endl;
        }

        std::cout << "Выберите номер: ";
        int choice = safeInput();

        if (choice < 1 || choice > static_cast<int>(ciphers.size())) {
            std::cout << "[ОШИБКА] Неверный выбор!\n";
            return;
        }

        auto cipher = manager.getCipher(ciphers[choice - 1]);
        if (!cipher) {
            std::cout << "[ОШИБКА] Не удалось загрузить алгоритм!\n";
            return;
        }

        std::string key = getKey(cipher);

        if (!cipher->isValidKey(key)) {
            std::cout << "[ОШИБКА] Неверный ключ!\n";
            return;
        }

        std::cout << "Входной файл: ";
        std::string inputPath;
        std::getline(std::cin, inputPath);

        std::cout << "Выходной файл: ";
        std::string outputPath;
        std::getline(std::cin, outputPath);

        if (!FileProcessor::fileExists(inputPath)) {
            std::cout << "[ОШИБКА] Файл не найден! Создать новый? (y/n): ";
            char answer;
            std::cin >> answer;
            std::cin.ignore();
            if (answer == 'y' || answer == 'Y') {
                std::ofstream f(inputPath);
                f << "Тестовый текст для шифрования";
                f.close();
                std::cout << "[СОЗДАН] Файл: " << inputPath << std::endl;
            } else {
                return;
            }
        }

        std::cout << "1 - шифровать, 2 - дешифровать: ";
        int action = safeInput();

        try {
            bool result = FileProcessor::processFile(inputPath, outputPath, cipher, key, action == 1);
            if (result) {
                std::cout << "[ГОТОВО] Файл обработан успешно.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "[ОШИБКА] " << e.what() << std::endl;
        }
    }

public:
    void run() {
        std::cout << "[ЗАПУСК] Добро пожаловать в Encryption Algorithm RGR!\n";
        std::cout << "[ЗАГРУЗКА] Загружаю плагины...\n";
        
        std::filesystem::create_directories("plugins");
        for (const auto& entry : std::filesystem::directory_iterator("plugins")) {
            if (entry.is_regular_file()) {
                manager.loadPlugin(entry.path().string());
            }
        }

        while (true) {
            showMenu();
            int choice = safeInput();

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
                        std::cout << "[ОШИБКА] Нет алгоритмов!\n";
                        break;
                    }
                    std::cout << "Выберите алгоритм для генерации ключа:\n";
                    for (size_t i = 0; i < ciphers.size(); ++i) {
                        std::cout << "  " << i + 1 << ". " << ciphers[i] << std::endl;
                    }
                    int ch = safeInput();
                    if (ch >= 1 && ch <= static_cast<int>(ciphers.size())) {
                        auto cipher = manager.getCipher(ciphers[ch - 1]);
                        std::string key = cipher->generateKey();
                        std::cout << "[СГЕНЕРИРОВАН] Ключ: " << key << std::endl;
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
                    std::cout << "[ВЫХОД] До свидания!\n";
                    return;
                default:
                    std::cout << "[ОШИБКА] Неверный выбор!\n";
            }
        }
    }
};

int main() {
    try {
        ConsoleUI ui;
        ui.run();
    } catch (const std::exception& e) {
        std::cerr << "[КРИТИЧЕСКАЯ ОШИБКА] " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
