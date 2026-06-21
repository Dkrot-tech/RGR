#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include "error_codes.h"
#include <string>
#include <iostream>
#include <functional>

class ErrorHandler {
private:
    static bool logErrors;
    static std::function<void(const std::string&)> errorCallback;

public:
    static void setLogging(bool enable);
    static void setErrorCallback(std::function<void(const std::string&)> callback);
    static void handleError(ErrorCode code, const std::string& context = "");
    static bool isSuccess(ErrorCode code);
    static std::string getErrorMessage(ErrorCode code);
    static void clearCallback();
};

#endif 