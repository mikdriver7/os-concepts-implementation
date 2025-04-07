#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <thread>

#include "Clock.h"
#include "Logger.h"
#include "VirtualMemoryManager.h"
#include "Command.h"
#include "Process.h"
#include "Scheduler.h"

using namespace std;

int main() {
    // Load memory configuration
    std::ifstream memFile("memconfig.txt");
    if (!memFile) {
        std::cerr << "Error: Could not open memconfig.txt\n";
        return 1;
    }

    int memoryPages;
    memFile >> memoryPages;
    memFile.close();

    // Initialize logger and clock
    Logger logger("output.txt");
    Clock clock;
    MainMemory mainmem(memoryPages);
    DiskManager disk("vm.txt");
    VirtualMemoryManager vmm(mainmem, disk, clock, logger);


    // Load command list
    std::ifstream commandFile("commands.txt");
    if (!commandFile) {
        std::cerr << "Error: Could not open commands.txt\n";
        return 1;
    }

    std::vector<Command> commands;
    std::string line;
    while (std::getline(commandFile, line)) {
        if (!line.empty())
            commands.emplace_back(line);
    }
    commandFile.close();

    // Load process configurations
    std::ifstream procFile("processes.txt");
    if (!procFile) {
        std::cerr << "Error: Could not open processes.txt\n";
        return 1;
    }

    int cores, numProcesses;
    procFile >> cores >> numProcesses;

    Scheduler scheduler(cores);
    std::vector<std::unique_ptr<Process>> processes;

  
    for (int i = 0; i < numProcesses; ++i) {
        int startTime, duration;
        procFile >> startTime >> duration;

        auto process = std::make_unique<Process>(i + 1, startTime, duration, vmm, clock, commands);
        scheduler.addProcess(process.get());
        processes.push_back(std::move(process));
        cout << "Process id:" << i + 1 << " start time: " << startTime << " duration: " << duration << endl;
    }

    procFile.close();

    // Start the clock and scheduler threads
    std::thread clockThread(&Clock::start, &clock);
    std::thread schedulerThread(&Scheduler::run, &scheduler);

    schedulerThread.join();
    scheduler.stop();

    
    clockThread.join();
    clock.stop();

    std::cout << "Simulation completed. Output written to output.txt\n";
    return 0;
}
