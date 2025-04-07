#ifndef MAIN_MEMORY_H
#define MAIN_MEMORY_H

#include "Page.h"
#include <vector>
#include <string>
#include <mutex>

class MainMemory {
private:
    std::vector<Page> pages;
    std::mutex memoryMutex;

public:
    MainMemory(int size);
    bool storeVariable(const std::string& id, unsigned int value, int currentTime);
    bool releaseVariable(const std::string& id);
    bool lookupVariable(const std::string& id, unsigned int& value, int currentTime);
    bool hasVariable(const std::string& id) const;
    int findLRUIndex() const;
    bool isFull() const;
    void swapIn(const std::string& id, unsigned int value, int currentTime, std::string& swappedOutId, unsigned int& swappedOutValue);
};

#endif
