#include "Logger.h"

Logger::Logger(const std::string& filename) {
    logFile.open(filename, std::ios::out);
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    logFile << message << std::endl;
    logFile.flush();
}
