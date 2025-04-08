#include "Process.h"
#include <chrono>
#include <thread>
#include <random>

Process::Process(int id, int start, int dur, VirtualMemoryManager& vm, Clock& clk, std::vector<Command>& cmds)
    : processId(id), startTime(start), duration(dur), vmm(vm), clock(clk), commands(cmds), commandIndex(0), running(false) {}

std::thread Process::spawn() {
    return std::thread(&Process::run, this);
}

void Process::run() {
    running = true;
    
    int commandIndex = 0; // Local commandIndex for each process

    // Wait until the clock reaches the start time
    // while (clock.getTime() < startTime) {
    //     std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Check the clock every 100ms
    // }

    int timeNow = clock.getTime();
    std::string startMsg = "Clock: " + std::to_string(timeNow) + ", Process " + std::to_string(processId) + ": Started.";
    vmm.logger.log(startMsg);  // Log when the process starts

    auto startClock = std::chrono::steady_clock::now(); // Start clock for tracking process duration

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> delay(1, 1000); // Random delay for simulating varied process timing

    while (running) {
        for (auto& cmd : commands) {
            if (!cmd.isExecuted()) {  // Only execute commands that have not been executed
                // Execute the command

                switch (cmd.type) {
                case STORE:
                    vmm.store(cmd.variableId, cmd.value, processId); // Execute STORE command
                    break;
                case RELEASE:
                    vmm.release(cmd.variableId, processId); // Execute RELEASE command
                    break;
                case LOOKUP:
                    vmm.lookup(cmd.variableId, processId); // Execute LOOKUP command
                    break;
                default:
                    break;
                }
                
                cmd.markExecuted();  // Mark command as executed
                break; // Stop the loop, since one command should be executed at a time
            }
    }

        // Check if all commands have been executed
        bool allExecuted = std::all_of(commands.begin(), commands.end(), [](Command& cmd) {
            return cmd.isExecuted();
        });
        if (allExecuted) break;

        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Sleep to simulate execution time
    }

    int timeEnd = clock.getTime();
    // std::string finishMsg = "Clock: " + std::to_string(timeEnd) + ", Process " + std::to_string(processId) + ": Finished.";
    // vmm.logger.log(finishMsg);
}
