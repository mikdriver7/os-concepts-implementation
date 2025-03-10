#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "User.h"
#include "EventLogger.h"

class Scheduler {
private:
    int time_quantum;
    std::vector<User> users;
    int current_time;
    EventLogger logger;

    std::mutex mtx;
    std::condition_variable cv;
    bool running;

public:
    Scheduler(int quantum, const std::string& log_filename);
    void add_user(const User& user);
    void run_scheduler();

private:
    void distribute_quantum();
};

#endif // SCHEDULER_H