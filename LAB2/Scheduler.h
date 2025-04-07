#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "User.h"
#include "EventLogger.h"
#include <vector>
#include <mutex>
#include <condition_variable>
#include <string>
#include <memory>

class Scheduler {
public:
    // Constructor: initializes the scheduler with a time quantum and log filename
    Scheduler(int quantum, const std::string& log_filename);

    // Add a user to the scheduler
    void add_user(std::unique_ptr<User> user);

    // Starts the scheduling loop
    void run_scheduler();

private:
    // Distributes the time quantum among active users and their processes
    void distribute_quantum();

    // Checks if all processes from all users are finished
    bool all_processes_finished();

    int time_quantum;  // Time quantum per scheduling cycle
    int current_time;  // Global scheduler time
    int current_user_index = 0;


    std::vector<std::unique_ptr<User>> users;  // List of users in the system

    EventLogger logger;  // Logger for events

    // Not used in current logic!!!!
    std::mutex mtx;
    std::condition_variable cv;
    bool running;
};

#endif // SCHEDULER_H

