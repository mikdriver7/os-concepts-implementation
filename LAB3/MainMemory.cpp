#include "MainMemory.h"

MainMemory::MainMemory(int size) : pages(size) {}

bool MainMemory::storeVariable(const std::string& id, unsigned int value, int currentTime) {
    std::lock_guard<std::mutex> lock(memoryMutex);
    for (auto& page : pages) {
        if (page.isEmpty()) {
            page.assign(id, value, currentTime);
            return true;
        }
    }
    return false;
}

bool MainMemory::releaseVariable(const std::string& id) {
    std::lock_guard<std::mutex> lock(memoryMutex);
    for (auto& page : pages) {
        if (page.isOccupied && page.variableId == id) {
            page.release();
            return true;
        }
    }
    return false;
}

bool MainMemory::lookupVariable(const std::string& id, unsigned int& value, int currentTime) {
    std::lock_guard<std::mutex> lock(memoryMutex);
    for (auto& page : pages) {
        if (page.isOccupied && page.variableId == id) {
            value = page.value;
            page.lastAccessTime = currentTime;
            return true;
        }
    }
    return false;
}

bool MainMemory::hasVariable(const std::string& id) const {
    for (const auto& page : pages) {
        if (page.isOccupied && page.variableId == id) {
            return true;
        }
    }
    return false;
}

bool MainMemory::isFull() const {
    for (const auto& page : pages) {
        if (!page.isOccupied)
            return false;
    }
    return true;
}

int MainMemory::findLRUIndex() const {
    int lruIndex = -1;
    int oldestTime = INT32_MAX;
    for (int i = 0; i < pages.size(); ++i) {
        if (pages[i].isOccupied && pages[i].lastAccessTime < oldestTime) {
            oldestTime = pages[i].lastAccessTime;
            lruIndex = i;
        }
    }
    return lruIndex;
}

void MainMemory::swapIn(const std::string& id, unsigned int value, int currentTime, std::string& swappedOutId, unsigned int& swappedOutValue) {
    std::lock_guard<std::mutex> lock(memoryMutex);
    int index = findLRUIndex();
    if (index >= 0) {
        swappedOutId = pages[index].variableId;
        swappedOutValue = pages[index].value;
        pages[index].assign(id, value, currentTime);
    }
}
