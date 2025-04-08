#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Process.h"
#include "Logger.h"
#include "Clock.h"

#include <vector>
#include <queue>
#include <deque>
#include <thread>
#include <mutex>
#include <sstream>
#include <condition_variable>
#include <iostream>
#include <atomic>


class Scheduler {
private:
    int maxCores;
    std::atomic<int> activeCores;
    bool done;

    std::deque<Process*> processQueue;
    std::vector<std::thread> runningThreads;
    std::mutex queueMutex;
    std::mutex logMutex;      //mutex for thread-safe logging
    std::condition_variable cv;

    Logger& logger;  // Reference to the logger
    Clock& clock;    // Reference to the clock

    

public:
    Scheduler(int cores, Logger& logger, Clock& clock);  // Pass logger and clock
    void addProcess(Process* process);
    void run();
    void stop();
    
    bool hasReadyProcess();
};

#endif
