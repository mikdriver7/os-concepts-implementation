#include "Scheduler.h"


Scheduler::Scheduler(int cores, Logger& logger, Clock& clock) 
    : maxCores(cores), activeCores(0), done(false), logger(logger), clock(clock) {
    // Constructor initializes maxCores, done, and sets references to logger and clock
}

void Scheduler::addProcess(Process* process) {
    std::lock_guard<std::mutex> lock(queueMutex);
    processQueue.push_back(process);
    cv.notify_one();
}

// void Scheduler::run() {
//     // Loop until all processes have been processed or we stop manually
//     while (!done) {
//         // Lock the mutex to ensure thread safety when accessing the queue
//         std::unique_lock<std::mutex> lock(queueMutex);

//         // Debug output (thread-safe)
//         {
//             std::lock_guard<std::mutex> logLock(logMutex);
//             std::cout << "[DEBUG] Clock: " << clock.getTime()
//                       << " | Active Cores: " << activeCores 
//                       << " vs MAX: " << maxCores
//                       << " | ProcessQueue Size: " << processQueue.size() 
//                       << std::endl;
//         }
        
//         // Wait for: (1) a process to be ready AND (2) a free core OR shutdown
//         cv.wait(lock, [&]() {
//             return (std::any_of(processQueue.begin(), processQueue.end(), [&](Process* p) {
//                     return clock.getTime() >= p->getStartTime() * 1000;
//                 }) && 
//                 activeCores.load() < maxCores) || 
//                 done;
//         });


//         if (done) break;

//         // Find the first ready process
//         auto it = std::find_if(processQueue.begin(), processQueue.end(), [&](Process* p) {
//             return clock.getTime() >= p->getStartTime() * 1000;
//         });

//         if (it != processQueue.end()) {
//             Process* proc = *it;
//             processQueue.erase(it);

//             // Log before launching thread
//             {
//                 std::lock_guard<std::mutex> logLock(logMutex);
//                 logger.log("\nClock: " + std::to_string(clock.getTime()) + 
//                           ", Scheduler: Launching Process " + std::to_string(proc->getId()) + 
//                           " (Start time: " + std::to_string(proc->getStartTime()) + ")");
//                 std::cout << "[DEBUG] Launching Process " << proc->getId() 
//                           << " | Active Cores before: " << activeCores.load() << std::endl;
//             }

//             // Launch thread for the process
//             runningThreads.emplace_back([this, proc]() {
//                 {
//                     std::lock_guard<std::mutex> lock(queueMutex);
//                     activeCores.fetch_add(1);  // Increment HERE (after thread starts)
//                 }

//                 std::cout << "[THREAD " << proc->getId() << "] Started\n";
//                 proc->run();

//                 // After process execution, decrement active cores and notify the scheduler
//                 {
//                     std::lock_guard<std::mutex> lock(queueMutex);
//                     activeCores.fetch_sub(1); 
//                 }

//                 cv.notify_one();  // Notify scheduler that a core is free

//                 // Log completion
//                 std::lock_guard<std::mutex> logLock(logMutex);
//                 logger.log("Clock: " + std::to_string(clock.getTime()) + 
//                            ", Process " + std::to_string(proc->getId()) + ": Finished.");
//             });
//         }
//     }   

//     // After all processes have been scheduled, wait for all running threads to finish
//     for (auto& t : runningThreads) {
//         if (t.joinable()) {
//             t.join();  // Ensure that the thread finishes execution
//         }
//     }

//     std::cout << "Simulation completed." << std::endl;  // Print when simulation is done

// }
void Scheduler::run() {
    // Loop until all processes have been processed or we stop manually
    while (!done || !processQueue.empty()) {
        // Lock the mutex to ensure thread safety when accessing the queue
        std::unique_lock<std::mutex> lock(queueMutex);

        // Debug output (thread-safe)
        {
            // std::lock_guard<std::mutex> logLock(logMutex);
            // std::cout << "[DEBUG] Clock: " << clock.getTime()
            //           << " | Active Cores: " << activeCores
            //           << " vs MAX: " << maxCores
            //           << " | ProcessQueue Size: " << processQueue.size()
            //           << std::endl;
        }

    
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

            // Launch thread for the process
            runningThreads.emplace_back([this, proc]() {
                {
                    std::lock_guard<std::mutex> lock(queueMutex);
                    activeCores.fetch_add(1);  // Increment core count after thread starts
                }

                std::cout << "[THREAD " << proc->getId() << "] Started\n";
                proc->run();

                // After process execution, decrement active cores and notify the scheduler
                {
                    std::lock_guard<std::mutex> lock(queueMutex);
                    activeCores.fetch_sub(1);
                }

                cv.notify_one();  // Notify scheduler that a core is free

                // Log completion
                std::lock_guard<std::mutex> logLock(logMutex);
                logger.log("Clock: " + std::to_string(clock.getTime()) +
                           ", Process " + std::to_string(proc->getId()) + ": Finished.");
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