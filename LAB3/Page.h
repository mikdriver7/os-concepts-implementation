#ifndef PAGE_H
#define PAGE_H

#include <string>

class Page {
public:
    std::string variableId;
    unsigned int value;
    int lastAccessTime;
    bool isOccupied;

    Page();
    void assign(const std::string& id, unsigned int val, int currentTime);
    void release();
    bool isEmpty() const;
};

#endif
