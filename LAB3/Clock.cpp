#include "Clock.h"
#include <iostream>
using namespace std;

Clock::Clock() : currentTime(0), running(false) {}

void Clock::start() {
    running = true;
    clockThread = std::thread(&Clock::run, this);
}

void Clock::stop() {
    running = false;
    if (clockThread.joinable())
        clockThread.join();
}

void Clock::run() {
    std::cout << "current time:" << currentTime << endl;
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        ++currentTime;
        
    }
}

int Clock::getTime() const {
    return currentTime.load();
}
