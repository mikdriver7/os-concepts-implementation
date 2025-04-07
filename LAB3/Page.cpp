#include "Page.h"

Page::Page()
    : variableId(""), value(0), lastAccessTime(-1), isOccupied(false) {}

void Page::assign(const std::string& id, unsigned int val, int currentTime) {
    variableId = id;
    value = val;
    lastAccessTime = currentTime;
    isOccupied = true;
}

void Page::release() {
    variableId.clear();
    value = 0;
    lastAccessTime = -1;
    isOccupied = false;
}

bool Page::isEmpty() const {
    return !isOccupied;
}
