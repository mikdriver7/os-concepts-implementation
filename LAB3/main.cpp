/*
Scheduling and Memory Management
LAB ASSIGNMENT 3
Mik Driver (40244456)
Victor Depaz (40242703)

g++ -std=c++14 main.cpp Clock.cpp Logger.cpp VirtualMemoryManager.cpp Process.cpp Scheduler.cpp MainMemory.cpp DiskManager.cpp Command.cpp Page.cpp -o scheduler -pthread
./scheduler
*/


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

    // Initialize memory and virtual memory manager
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
            commands.emplace_back(line); // Add each command from the file
    }
    commandFile.close();

    // Load process configurations
    std::ifstream procFile("processes.txt");
    if (!procFile) {
        std::cerr << "Error: Could not open processes.txt\n";
        return 1;
    }

    int cores, numProcesses;
    procFile >> cores >> numProcesses; // Read the number of cores and processes from file

    cout << cores << " Cores\n\n";
    
    Scheduler scheduler(cores, logger, clock);  // Pass the logger and clock to the Scheduler
    std::vector<std::unique_ptr<Process> > processes; 

  
    for (int i = 0; i < numProcesses; ++i) {
        int startTime, duration;
        procFile >> startTime >> duration;

        auto process = std::make_unique<Process>(i + 1, startTime, duration, vmm, clock, commands);
        scheduler.addProcess(process.get());
        processes.push_back(std::move(process));
        cout << "Process id: " << i + 1 << " start time: " << startTime << " duration: " << duration << endl;
    }

    procFile.close();

    // Start the clock and scheduler threads
    std::thread clockThread(&Clock::start, &clock);
    std::thread schedulerThread(&Scheduler::run, &scheduler);

    schedulerThread.join();
    std::cout << "S\n";

    scheduler.stop();

    std::cout << "S\n";

    clockThread.join();
    clock.stop();

    std::cout << "Simulation completed. Output written to output.txt\n";
    return 0;
}
