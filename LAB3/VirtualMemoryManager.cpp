#include "VirtualMemoryManager.h"
#include <sstream>

VirtualMemoryManager::VirtualMemoryManager(MainMemory& mem, DiskManager& dsk, Clock& clk, Logger& log)
    : memory(mem), disk(dsk), clock(clk), logger(log) {

}

// This instruction stores the given variable id and its value in the 
//first unassigned spot in the memory. 
void VirtualMemoryManager::store(const std::string& variableId, unsigned int value, int processId) {
    std::lock_guard<std::mutex> lock(vmmMutex); // Mutex lock to ensure thread-safety
    
    int time = clock.getTime(); // Get the current time

    // Debug message to terminal
    std::cout << "Process " << processId << " attempting to store Variable " << variableId << std::endl;

    // Check if the variable is already in memory
    if (memory.hasVariable(variableId)) {
        // If the variable is already in memory, skip storing it
        std::cout << "Variable " << variableId << " already in memory, skipping store." << std::endl;

        // Log the operation
        std::ostringstream oss;
        // oss << "Clock: " << time << ", Process " << processId << ", Store: Variable " << variableId << ", Value: " << value << " (Already in memory)";
        // logger.log(oss.str());

        return;
    }

    // Try to store the variable in memory
        if (!memory.storeVariable(variableId, value, time)) {
        // If storing in memory fails, write to disk
        disk.writeVariable(variableId, value);

        // Log the operation
        std::ostringstream oss;
        oss << "Clock: " << time << ", Process " << processId << ", Store: Variable " << variableId << ", Value: " << value << " (Stored on disk)";
        logger.log(oss.str());
    } else {
        // Successfully stored in memory
        std::ostringstream oss;
        oss << "Clock: " << time << ", Process " << processId << ", Store: Variable " << variableId << ", Value: " << value;
        logger.log(oss.str());
    }
}

//This instruction removes the variable id and its value from the memory so the 
// page which was holding this variable becomes available for further storage.
void VirtualMemoryManager::release(const std::string& variableId, int processId) {
    std::lock_guard<std::mutex> lock(vmmMutex);
    int time = clock.getTime();

    std::cout << "Process " << processId << " attempting to release Variable " << variableId << std::endl;

    // Try to release the variable from memory
    if (!memory.releaseVariable(variableId)) {
        // If release from memory fails, remove from disk
        disk.removeVariable(variableId);
    }

    // Log the operation
    std::ostringstream oss;
    oss << "Clock: " << time << ", Process " << processId << ", Release: Variable " << variableId;
    logger.log(oss.str());
}


// This instruction checks if the given variableId is stored in the
// memory and returns its value or -1 if it does not exist. If the Id exists in the main memory
// it returns its value. If the Id is not in the main memory but exists in disk space (i.e. page
// fault occurs), then it should move this variable into the memory and release the assigned
// page in the virtual memory. Notice that if no spot is available in the memory, program
// needs to swap this variable with the least recently accessed variable, i.e. the variable with
// smallest Last Access time, in the main memory.
int VirtualMemoryManager::lookup(const std::string& variableId, int processId) {
    std::lock_guard<std::mutex> lock(vmmMutex);
    int time = clock.getTime();
    unsigned int value;

    std::cout << "Lookup called for Variable: " << variableId << std::endl;


    std::ostringstream oss;

    // Look up variable in memory
    if (memory.lookupVariable(variableId, value, time)) {
        // If found in memory, log the result and return the value
        oss << "Clock: " << time << ", Process " << processId << ", Lookup: Variable " << variableId << ", Value: " << value;
        logger.log(oss.str());
        return value;
    }

    // If not found in memory, try reading from disk and perform swap
    if (disk.readVariable(variableId, value)) {
        std::string outId;
        unsigned int outVal;
        
        // Perform the swap
        memory.swapIn(variableId, value, time, outId, outVal);
        
        // Log the swap operation
        oss << "Clock: " << time << ", Memory Manager, SWAP: Variable " << variableId << " with Variable " << outId;
        logger.log(oss.str()); 
        return value;
    }

    // If variable not found in either memory or disk, log as missing
    oss << "Clock: " << time << ", Process " << processId << ", Lookup: Variable " << variableId << ", Value: -1";
    logger.log(oss.str());
    
    return -1; // Indicate that the variable was not found
}
