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

    // Getter for the process ID
    int getId() const {
        return processId;
    }

    // Getter for the duration of the process
    int getDuration() const {
        return duration;
    }

    // Getter for the start time of the process
    int getStartTime() const {
        return startTime;
    }

    // Check if the process is currently running
    bool isRunning() const {
        return running.load();
    }
    
};

#endif
