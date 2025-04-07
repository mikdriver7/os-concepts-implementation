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
    int waitTime = startTime * 1000;
    std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));

    int timeNow = clock.getTime();
    std::string startMsg = "Clock: " + std::to_string(timeNow) + ", Process " + std::to_string(processId) + ": Started.";
    vmm.logger.log(startMsg);

    auto startClock = std::chrono::steady_clock::now();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> delay(1, 1000);

    while (running) {
        auto elapsed = std::chrono::steady_clock::now() - startClock;
        if (std::chrono::duration_cast<std::chrono::seconds>(elapsed).count() >= duration) break;

        const Command& cmd = commands[commandIndex];
        commandIndex = (commandIndex + 1) % commands.size();

        switch (cmd.type) {
        case STORE:
            vmm.store(cmd.variableId, cmd.value, processId);
            break;
        case RELEASE:
            vmm.release(cmd.variableId, processId);
            break;
        case LOOKUP:
            vmm.lookup(cmd.variableId, processId);
            break;
        default:
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(delay(gen)));
    }

    int timeEnd = clock.getTime();
    std::string finishMsg = "Clock: " + std::to_string(timeEnd) + ", Process " + std::to_string(processId) + ": Finished.";
    vmm.logger.log(finishMsg);
}
