#include "VirtualMemoryManager.h"
#include <sstream>

VirtualMemoryManager::VirtualMemoryManager(MainMemory& mem, DiskManager& dsk, Clock& clk, Logger& log)
    : memory(mem), disk(dsk), clock(clk), logger(log) {}

void VirtualMemoryManager::store(const std::string& variableId, unsigned int value, int processId) {
    std::lock_guard<std::mutex> lock(vmmMutex);
    int time = clock.getTime();

    if (!memory.storeVariable(variableId, value, time)) {
        disk.writeVariable(variableId, value);
    }

    std::ostringstream oss;
    oss << "Clock: " << time << ", Process " << processId << ", Store: Variable " << variableId << ", Value: " << value;
    logger.log(oss.str());
}

void VirtualMemoryManager::release(const std::string& variableId, int processId) {
    std::lock_guard<std::mutex> lock(vmmMutex);
    int time = clock.getTime();

    if (!memory.releaseVariable(variableId)) {
        disk.removeVariable(variableId);
    }

    std::ostringstream oss;
    oss << "Clock: " << time << ", Process " << processId << ", Release: Variable " << variableId;
    logger.log(oss.str());
}

int VirtualMemoryManager::lookup(const std::string& variableId, int processId) {
    std::lock_guard<std::mutex> lock(vmmMutex);
    int time = clock.getTime();
    unsigned int value;

    std::ostringstream oss;

    if (memory.lookupVariable(variableId, value, time)) {
        oss << "Clock: " << time << ", Process " << processId << ", Lookup: Variable " << variableId << ", Value: " << value;
        logger.log(oss.str());
        return value;
    }

    if (disk.readVariable(variableId, value)) {
        std::string outId;
        unsigned int outVal;
        memory.swapIn(variableId, value, time, outId, outVal);
        disk.removeVariable(variableId);
        disk.writeVariable(outId, outVal);

        logger.log("Clock: " + std::to_string(time) + ", Memory Manager, SWAP: Variable " + variableId + " with Variable " + outId);
        oss << "Clock: " << time << ", Process " << processId << ", Lookup: Variable " << variableId << ", Value: " << value;
        logger.log(oss.str());
        return value;
    }

    oss << "Clock: " << time << ", Process " << processId << ", Lookup: Variable " << variableId << ", Value: -1";
    logger.log(oss.str());
    return -1;
}
