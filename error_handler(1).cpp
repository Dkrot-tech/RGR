#include "error_handler.h"
#include <map>

bool ErrorHandler::logErrors = true;
std::function<void(const std::string&)> ErrorHandler::errorCallback = nullptr;

void ErrorHandler::setLogging(bool enable) {
    logErrors = enable;
}

void ErrorHandler::setErrorCallback(std::function<void(const std::string&)> callback) {
    errorCallback = callback;
}

void ErrorHandler::clearCallback() {
    errorCallback = nullptr;
}

std::string ErrorHandler::getErrorMessage(ErrorCode code) {
    switch (code) {
        case ErrorCode::SUCCESS: return "Операция выполнена успешно";
        case ErrorCode::FILE_NOT_FOUND: return "Файл не найден по указанному пути";
        case ErrorCode::FILE_READ_ERROR: return "Ошибка при чтении файла";
        case ErrorCode::FILE_WRITE_ERROR: return "Ошибка при записи в файл";
        case ErrorCode::INVALID_KEY: return "Ключ не соответствует требованиям алгоритма";
        case ErrorCode::EMPTY_TEXT: return "Входной текст пуст";
        case ErrorCode::INVALID_ALGORITHM: return "Выбранный алгоритм не поддерживается";
        case ErrorCode::KEY_GENERATION_FAILED: return "Не удалось сгенерировать ключ";
        case ErrorCode::MEMORY_ALLOCATION_ERROR: return "Ошибка выделения памяти";
        case ErrorCode::INVALID_FILE_PATH: return "Путь к файлу содержит недопустимые символы";
        case ErrorCode::DIRECTORY_CREATION_FAILED: return "Не удалось создать директорию";
        case ErrorCode::UNKNOWN_ERROR: return "Произошла неизвестная ошибка";
        default: return "Неизвестный код ошибки";
    }
}

void ErrorHandler::handleError(ErrorCode code, const std::string& context) {
    if (code == ErrorCode::SUCCESS) return;
    
    std::string message = getErrorMessage(code);
    if (!context.empty()) {
        message += " (контекст: " + context + ")";
    }
    
    if (logErrors) {
        std::cerr << "[ОШИБКА] " << message << std::endl;
    }
    
    if (errorCallback) {
        errorCallback(message);
    }
}

bool ErrorHandler::isSuccess(ErrorCode code) {
    return code == ErrorCode::SUCCESS;
}
