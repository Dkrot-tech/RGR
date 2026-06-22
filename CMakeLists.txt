cmake_minimum_required(VERSION 3.10)
project(EncryptionAlgorithmRGR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Основное приложение
add_executable(encryption_rgr
    UI/ConsoleUI.cpp
    Core/CipherManager.cpp
    Core/FileProcessor.cpp
)

target_include_directories(encryption_rgr PRIVATE
    Core
    UI
)

# Плагин XOR
add_library(xor_plugin SHARED
    Plugins/XORCipher/XORCipher.cpp
)

target_include_directories(xor_plugin PRIVATE
    Core
)

# Настройка имени файла плагина (без префикса lib)
set_target_properties(xor_plugin PROPERTIES
    PREFIX ""
    SUFFIX ".so"
)

# Создаём папку plugins и копируем туда плагин после сборки
add_custom_command(TARGET xor_plugin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/plugins
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:xor_plugin> ${CMAKE_BINARY_DIR}/plugins/
)

# Копируем плагин в папку с исполняемым файлом (для удобства)
add_custom_command(TARGET encryption_rgr POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/plugins
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:xor_plugin> ${CMAKE_BINARY_DIR}/plugins/
)
