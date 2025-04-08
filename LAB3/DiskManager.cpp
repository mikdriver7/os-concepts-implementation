#include "DiskManager.h"
#include <fstream>
#include <sstream>

// Constructor initializes the DiskManager with a file for storage. If the file does not exist, it creates one.
DiskManager::DiskManager(const std::string& file) : filename(file) {
    std::ofstream ofs(filename, std::ios::app); // Open file in append mode to create it if it doesn't exist
}

// Method to write a variable (id, value) to the disk file
void DiskManager::writeVariable(const std::string& id, unsigned int value) {
    std::lock_guard<std::mutex> lock(diskMutex);
    std::ifstream inFile(filename);
    std::ofstream tempFile("vm_temp.txt"); // Temporary file to store updated contents

    std::string line;
    bool updated = false; // Flag to track if the variable has been updated

    // Read through the file line by line
    while (getline(inFile, line)) {
        std::istringstream iss(line); // Split the line into id and value
        std::string vid;
        unsigned int val;
        iss >> vid >> val;

        // If the id matches, update the value
        if (vid == id) {
            tempFile << id << " " << value << "\n"; // Write the updated value
            updated = true;
        }
        else {
            tempFile << line << "\n"; // Otherwise, just copy the line to the temporary file
        }
    }

    // If the variable was not found, append it to the temporary file
    if (!updated) tempFile << id << " " << value << "\n";

    // Close both input and output files
    inFile.close();
    tempFile.close();

    // Replace the original file with the updated one
    std::remove(filename.c_str());
    std::rename("vm_temp.txt", filename.c_str());
}

// Method to read a variable from the disk by its id
bool DiskManager::readVariable(const std::string& id, unsigned int& value) {
    std::lock_guard<std::mutex> lock(diskMutex);
    std::ifstream inFile(filename);
    std::string vid;
    unsigned int val;

    // Search for the variable by id in the file
    while (inFile >> vid >> val) {
        if (vid == id) {
            value = val; // Set the value if the id is found
            inFile.close();
            return true; // Return true if found
        }
    }

    inFile.close();
    return false;  // Return false if the id was not found
}

// Method to remove a variable from the disk by its id
void DiskManager::removeVariable(const std::string& id) {
    std::lock_guard<std::mutex> lock(diskMutex);
    std::ifstream inFile(filename);
    std::ofstream tempFile("vm_temp.txt");

    std::string vid;
    unsigned int val;

    // Read through the file and copy all entries except the one to be removed
    while (inFile >> vid >> val) {
        if (vid != id) {
            tempFile << vid << " " << val << "\n"; // Write all except the removed variable
        }
    }

    inFile.close();
    tempFile.close();
        
    // Replace the original file with the updated one
    std::remove(filename.c_str());
    std::rename("vm_temp.txt", filename.c_str());
}
