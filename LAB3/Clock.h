#ifndef CLOCK_H
#define CLOCK_H

#include <atomic>
#include <thread>
#include <chrono>

class Clock {
private:
    std::atomic<int> currentTime;
    std::atomic<bool> running;
    std::thread clockThread;

    

public:
    Clock();
    void start();
    void stop();
    int getTime() const;
    void run();
};

#endif
