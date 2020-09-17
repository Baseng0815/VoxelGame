#include "../include/Logger.h"

#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

int Logger::m_mode = LOG_NONE;

void Logger::setLoggingMode(int mode) {
    m_mode = mode;
}

void Logger::logString(const std::string& message) {
    if (m_mode & LOG_NONE) return;

    auto t = std::time(nullptr);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&t), "%d-%m-%Y %H-%M-%S: ");
    std::string logMessage = oss.str() + message;

    if (m_mode & LOG_FILE) {
        std::fstream file("log.txt", std::ios::out | std::ios::app);
        if (file.good())
            file << logMessage << std::endl;
        file.close();
    }

    if (m_mode & LOG_STDOUT)
        std::cout << logMessage << std::endl;
}
