#include "MainMemory.h"

// Constructor to initialize memory with a specified size
MainMemory::MainMemory(int size) : pages(size) {}

// Store a variable in the memory

bool MainMemory::storeVariable(const std::string& id, unsigned int value, int currentTime) {
    std::lock_guard<std::recursive_mutex> lock(memoryMutex); // Ensure thread-safety

    // Check if variable already exists
    if (hasVariable(id)) {
        std::cout << "Variable " << id << " already exists in memory" << std::endl;
        return false;
    }

    // Check each page to find the first empty one to store the variable
    // Try to find empty page
    for (auto& page : pages) {
        if (page.isEmpty()) {
            page.assign(id, value, currentTime);
            return true;
        }
    }
    
    // If no empty page is found, memory is full. Attempt to swap in the new variable
    // by evicting the Least Recently Used (LRU) page.
    std::string swappedOutId;
    unsigned int swappedOutValue;
    std::cout << "Clock: " << currentTime << ", Memory is full, attempting to swap in variable " << id << std::endl;
    swapIn(id, value, currentTime, swappedOutId, swappedOutValue);

    // After swapping, return true because we now have space for the new variable
    return true;
}

// Release a variable from memory by clearing its page
bool MainMemory::releaseVariable(const std::string& id) {
    std::lock_guard<std::recursive_mutex> lock(memoryMutex); // Ensure thread-safety

    // Iterate over pages to find the variable with the given id
    for (auto& page : pages) {
        if (!page.isEmpty() && page.variableId == id) {
            // If the variable is found, release it (clear the page)
            page.release();
            return true;
        }
    }

    // If variable is not found, return false
    return false;
}

// Look up a variable in memory and return its value
bool MainMemory::lookupVariable(const std::string& id, unsigned int& value, int currentTime) {
    std::lock_guard<std::recursive_mutex> lock(memoryMutex); // Ensure thread-safety

    // Search through the pages to find the variable with the given id
    for (auto& page : pages) {
        if (!page.isEmpty() && page.variableId == id) {
            // If found, set the value and update the last access time
            value = page.value;
            page.lastAccessTime = currentTime;
            return true;
        }
    }

    // Return false if the variable was not found in memory
    return false;
}

// Check if a variable exists in memory
bool MainMemory::hasVariable(const std::string& id) const {
    // Iterate through the pages to check if the variable exists
    for (const auto& page : pages) {
        if (!page.isEmpty() && page.variableId == id) {
            return true;  // If found, return true
        }
    }

    return false; // Return false if not found
}

// Check if memory is full (no empty pages)
bool MainMemory::isFull() const {
    // Iterate through pages and check if any page is empty
    for (const auto& page : pages) {
        if (page.isEmpty())
            return false;  // If an empty page is found, memory is not full
    }

    // Return true if no empty pages are found (memory is full)
    return true; 
}

// Find the index of the Least Recently Used (LRU) page
int MainMemory::findLRUIndex() const {
    int lruIndex = -1; // Initialize the LRU index as -1 (invalid index)
    int oldestTime = INT32_MAX; // Set the oldest time to the maximum possible value

    std::cout << "Finding LRU page:" << std::endl;  // Debugging output

    // Iterate through the pages to find the LRU page
    for (int i = 0; i < pages.size(); ++i) {
        std::cout << "Page " << i << ": " << (pages[i].isEmpty() ? "Empty" : "Occupied") << std::endl;

        if (!pages[i].isEmpty() && pages[i].lastAccessTime < oldestTime) {
            oldestTime = pages[i].lastAccessTime;
            lruIndex = i;
        }
    }

    std::cout << "LRU index: " << lruIndex << ", Oldest Time: " << oldestTime << std::endl;  // Debugging output
    return lruIndex;    // Return the index of the LRU page
}

// Swap in a new variable by evicting the Least Recently Used (LRU) page
void MainMemory::swapIn(const std::string& id, unsigned int value, int currentTime, std::string& swappedOutId, unsigned int& swappedOutValue) {
    std::lock_guard<std::recursive_mutex> lock(memoryMutex); // Ensure thread-safety
    
    std::cout << "swapIn called for variable: " << id << ", with value: " << value << std::endl;

    // Find the index of the Least Recently Used (LRU) page
    int index = findLRUIndex();
    std::cout << "LRU Index: " << index << std::endl;  // Log the LRU index for debugging

    if (index >= 0) {  // If a valid LRU page is found
        // Store the details of the evicted (swapped-out) page
        swappedOutId = pages[index].variableId;
        swappedOutValue = pages[index].value;


        // Check if the variable is already in memory and if the value matches
        if (pages[index].variableId == id) {
            if (pages[index].value == value) {
                std::cout << "Variable " << id << " with value " << value << " is already in memory, skipping swap." << std::endl;
                return;  // Skip swapping if the same variable with the same value is already in memory
            } else {
                std::cout << "Variable " << id << " found in memory but with different value. Evicting and swapping." << std::endl;
            }
        }

        // Evict the page (release it) and assign the new variable to the LRU page
        std::cout << "Evicting Variable " << swappedOutId << " with Value " << swappedOutValue << std::endl;
        pages[index].release();  // Release the old page
        pages[index].assign(id, value, currentTime); // Assign the new variable

        std::cout << "Swapped Variable " << swappedOutId << " with Variable " << id << std::endl;
    } else {
        std::cout << "Clock: " << currentTime << ", Memory Manager, ERROR: No valid LRU page found for eviction!" << std::endl;
    }
}
