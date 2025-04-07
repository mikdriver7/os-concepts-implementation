#include "DiskManager.h"
#include <fstream>
#include <sstream>

DiskManager::DiskManager(const std::string& file) : filename(file) {
    std::ofstream ofs(filename, std::ios::app); // Create file if not exists
}

void DiskManager::writeVariable(const std::string& id, unsigned int value) {
    std::lock_guard<std::mutex> lock(diskMutex);
    std::ifstream inFile(filename);
    std::ofstream tempFile("vm_temp.txt");

    std::string line;
    bool updated = false;

    while (getline(inFile, line)) {
        std::istringstream iss(line);
        std::string vid;
        unsigned int val;
        iss >> vid >> val;
        if (vid == id) {
            tempFile << id << " " << value << "\n";
            updated = true;
        }
        else {
            tempFile << line << "\n";
        }
    }

    if (!updated)
        tempFile << id << " " << value << "\n";

    inFile.close();
    tempFile.close();
    std::remove(filename.c_str());
    std::rename("vm_temp.txt", filename.c_str());
}

bool DiskManager::readVariable(const std::string& id, unsigned int& value) {
    std::lock_guard<std::mutex> lock(diskMutex);
    std::ifstream inFile(filename);
    std::string vid;
    unsigned int val;

    while (inFile >> vid >> val) {
        if (vid == id) {
            value = val;
            inFile.close();
            return true;
        }
    }

    return false;
}

void DiskManager::removeVariable(const std::string& id) {
    std::lock_guard<std::mutex> lock(diskMutex);
    std::ifstream inFile(filename);
    std::ofstream tempFile("vm_temp.txt");

    std::string vid;
    unsigned int val;

    while (inFile >> vid >> val) {
        if (vid != id) {
            tempFile << vid << " " << val << "\n";
        }
    }

    inFile.close();
    tempFile.close();
    std::remove(filename.c_str());
    std::rename("vm_temp.txt", filename.c_str());
}
