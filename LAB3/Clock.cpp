#include "Clock.h"
#include <iostream>
using namespace std;

Clock::Clock() : currentTime(1000), running(false) {}

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
    int timeOffset = 1000 - currentTime.load(); 

    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        currentTime += 10;
        
        // Log the time at regular intervals
        // if (currentTime % 10 == 0) {  // Adjust the condition to log every 10ms
        //     std::cout << "Clock: " << currentTime << endl;
        // }
    }
}

int Clock::getTime() const {
    return currentTime.load();
}