#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Process.h"
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

class Scheduler {
private:
    int maxCores;
    std::queue<Process*> processQueue;
    std::vector<std::thread> runningThreads;
    std::mutex queueMutex;
    std::condition_variable cv;
    bool done;

public:
    Scheduler(int cores);
    void addProcess(Process* process);
    void run();
    void stop();
};

#endif
