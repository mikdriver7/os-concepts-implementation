#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <mutex>
#include <string>

class Logger {
private:
    std::ofstream logFile;
    std::mutex logMutex;

public:
    Logger(const std::string& filename);
    ~Logger();
    void log(const std::string& message);
};

#endif
