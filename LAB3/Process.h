#ifndef PROCESS_H
#define PROCESS_H

#include "VirtualMemoryManager.h"
#include "Command.h"
#include "Clock.h"
#include <thread>
#include <vector>
#include <atomic>

class Process {
private:
    int processId;
    int startTime;
    int duration;
    std::atomic<bool> running;
    VirtualMemoryManager& vmm;
    Clock& clock;
    std::vector<Command>& commands;
    int commandIndex;

public:
    Process(int id, int start, int dur, VirtualMemoryManager& vm, Clock& clk, std::vector<Command>& cmds);
    void run();
    std::thread spawn();
};

#endif
