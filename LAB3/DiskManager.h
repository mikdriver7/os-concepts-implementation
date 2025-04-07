#ifndef DISK_MANAGER_H
#define DISK_MANAGER_H

#include <string>
#include <fstream>
#include <mutex>
#include <vector>

struct DiskEntry {
    std::string variableId;
    unsigned int value;
};

class DiskManager {
private:
    std::string filename;
    std::mutex diskMutex;

public:
    DiskManager(const std::string& file);
    void writeVariable(const std::string& id, unsigned int value);
    bool readVariable(const std::string& id, unsigned int& value);
    void removeVariable(const std::string& id);
};

#endif
