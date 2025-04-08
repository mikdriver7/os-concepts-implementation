#include "Scheduler.h"


Scheduler::Scheduler(int cores, Logger& logger, Clock& clock, VirtualMemoryManager& vmm)
    : vmm(vmm), maxCores(cores), logger(logger), clock(clock), activeCores(0) {}


void Scheduler::addProcess(Process* process) {
    std::lock_guard<std::mutex> lock(queueMutex);
    processQueue.push_back(process);
    cv.notify_one();
}
   

void Scheduler::run() {
    // Loop until all processes have been processed or we stop manually
    while (!done || !processQueue.empty()) {
        // Lock the mutex to ensure thread safety when accessing the queue
        std::unique_lock<std::mutex> lock(queueMutex);

        // Find the first ready process
        auto it = std::find_if(processQueue.begin(), processQueue.end(), [&](Process* p) {
            return clock.getTime() >= p->getStartTime() * 1000;
        });


        // If a process is ready to start and there are free cores
        if (it != processQueue.end() && activeCores.load() < maxCores) {
            Process* proc = *it;
            processQueue.erase(it);

            // Log before launching thread
            {
                std::lock_guard<std::mutex> logLock(logMutex);
                logger.log("\nClock: " + std::to_string(clock.getTime()) +
                           ", Scheduler: Launching Process " + std::to_string(proc->getId()) +
                           " (Start time: " + std::to_string(proc->getStartTime()) + ")");
                std::cout << "[DEBUG] Launching Process " << proc->getId()
                          << " | Active Cores before: " << activeCores.load() << std::endl;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Sleep for 10ms to simulate clock tick


            // Launch thread for the process
            runningThreads.emplace_back([this, proc]() {
                {
                    std::lock_guard<std::mutex> lock(queueMutex);
                    activeCores.fetch_add(1);  // Increment core count after thread starts
                }

                while (true) {
                    // Find the first command that has not been executed
                    Command* cmd = nullptr;
                    for (auto& c : proc->getCommands()) {
                        if (!c.isExecuted()) {
                            cmd = &c;
                            break;
                        }
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(400)); // Sleep to simulate execution

                    if (cmd != nullptr) {
                        // Execute the command
                        switch (cmd->type) {
                            case STORE:
                                vmm.store(cmd->variableId, cmd->value, proc->getId());
                                break;
                            case RELEASE:
                                vmm.release(cmd->variableId, proc->getId());
                                break;
                            case LOOKUP:
                                vmm.lookup(cmd->variableId, proc->getId());
                                break;
                            default:
                                break;
                        }
        
                        cmd->markExecuted();  // Mark the command as executed

                    }

                    // If all commands are executed, break the loop
                    bool allExecuted = std::all_of(proc->getCommands().begin(), proc->getCommands().end(), [](Command& c) {
                        return c.isExecuted();
                    });
                    if (allExecuted) break;

                    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Sleep before re-checking
                }

                // After process execution, decrement active cores
                {
                    std::lock_guard<std::mutex> lock(queueMutex);
                    activeCores.fetch_sub(1);
                }

                cv.notify_one();  // Notify scheduler that a core is free


                // Log completion
                logger.log("Clock: " + std::to_string(clock.getTime()) +
                           ", Process " + std::to_string(proc->getId()) + ": Finished.");

                std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Sleep for 10ms to simulate clock tick

            });

            continue;
        }

        // If no process is ready and there are no free cores, wait for a change
        cv.wait(lock, [&]() {
            return (activeCores.load() < maxCores || !processQueue.empty()) || done;
        });
    }

    // After all processes have been scheduled, wait for all running threads to finish
    for (auto& t : runningThreads) {
        if (t.joinable()) {
            t.join();  // Ensure that the thread finishes execution
        }
    }

    std::cout << "Simulation completed." << std::endl;  // Print when simulation is done
}


bool Scheduler::hasReadyProcess() {
    for (auto* proc : processQueue) {
        if (clock.getTime() >= proc->getStartTime() * 1000) {
            return true;
        }
    }
    return false;
}

void Scheduler::stop() {
    done = true;
    cv.notify_all();
}