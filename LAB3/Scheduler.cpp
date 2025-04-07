#include "Scheduler.h"

Scheduler::Scheduler(int cores) : maxCores(cores), done(false) {}

void Scheduler::addProcess(Process* process) {
    std::lock_guard<std::mutex> lock(queueMutex);
    processQueue.push(process);
    cv.notify_one();
}

void Scheduler::run() {
    while (!done || !processQueue.empty()) {
        std::unique_lock<std::mutex> lock(queueMutex);
        cv.wait(lock, [&]() {
            return !processQueue.empty() && runningThreads.size() < maxCores;
            });

        if (!processQueue.empty()) {
            Process* proc = processQueue.front();
            processQueue.pop();
            runningThreads.emplace_back(proc->spawn());
        }
    }

    // Wait for all threads to finish
    for (auto& t : runningThreads) {
        if (t.joinable()) t.join();
    }
}

void Scheduler::stop() {
    done = true;
    cv.notify_all();
}
