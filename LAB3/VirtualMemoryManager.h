#ifndef VIRTUAL_MEMORY_MANAGER_H
#define VIRTUAL_MEMORY_MANAGER_H

#include "MainMemory.h"
#include "DiskManager.h"
#include "Logger.h"
#include "Clock.h"

#include <string>
#include <mutex>
#include <unordered_set>

class VirtualMemoryManager {
private:
    MainMemory& memory;
    DiskManager& disk;
    Clock& clock;
    
    std::mutex vmmMutex;

public:
    VirtualMemoryManager(MainMemory& mem, DiskManager& dsk, Clock& clk, Logger& log);
    Logger& logger;
    void store(const std::string& variableId, unsigned int value, int processId);
    void release(const std::string& variableId, int processId);
    int lookup(const std::string& variableId, int processId);
};

#endif
