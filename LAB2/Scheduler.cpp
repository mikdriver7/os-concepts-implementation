#include "Scheduler.h"
#include <iostream>
#include <chrono>
#include <algorithm>

using namespace std;

// Initializes the scheduler with a given time quantum and a logger for events.
Scheduler::Scheduler(int quantum, const std::string& log_filename) : time_quantum(quantum), current_time(1), logger(log_filename), running(false), current_user_index(0) {

}

// Adds a user to the scheduler.
void Scheduler::add_user(std::unique_ptr<User> user) {
    users.push_back(std::move(user));
}

// Runs the scheduler.
void Scheduler::run_scheduler() {
    current_time = 1;

    // Start all process threads for each user.
    for (auto& user : users) {
        user->start_processes();
    }

    // Main scheduling loop: runs until all processes are finished.
    while (true) {
        if (all_processes_finished()) {
            std::cout << "All processes finished at time: " << current_time << std::endl;
            break;
        }

        distribute_quantum();
    }

    // After scheduling is complete, stop all processes.
    for (auto& user : users) {
        user->stop_processes();
    }

    std::cout << "Scheduling complete at time: " << current_time << std::endl;
}

// Checks if all processes for all users are finished.
bool Scheduler::all_processes_finished() {
    for (auto& user : users) {
        if (!user->is_all_processes_finished()) {
            return false;
        }
    }
    return true;
}

// Distributes the time quantum to users and their processes.

void Scheduler::distribute_quantum() {
    vector<User*> active_users;

    // Gather active users who have ready processes
    for (auto& user : users) {
        auto ready_processes = user->get_ready_processes(current_time);
        if (!ready_processes.empty()) {
            active_users.push_back(user.get());
        }
    }

    if (active_users.empty()) {
        // No active users, advance time
        current_time += 1;
        return;
    }

    // Fair share quantum for each active user
    int per_user_quantum = time_quantum / active_users.size();

    if (per_user_quantum == 0) {
        per_user_quantum = 1; // At least 1 unit of time
    }

    // Round-Robin between users
    if (current_user_index >= active_users.size()) {
        current_user_index = 0;
    }

    auto user = active_users[current_user_index];
    current_user_index++; // Move to next user

    // Get the next process for this user (Round-Robin for processes)
    auto ready_processes = user->get_ready_processes(current_time);

    if (ready_processes.empty()) {
        // This user has no ready processes (could be a race condition if state changed), skip
        return;
    }

    int num_ready_processes = ready_processes.size();

    // Divide the user's quantum equally among processes
    int per_process_quantum = per_user_quantum / num_ready_processes;

    if (per_process_quantum == 0) {
        per_process_quantum = 1; // At least 1 unit of time
    }


    // Iterate over each ready process and run it for its slice
    for (auto process : ready_processes) {
        if (!process) continue;

        if (process->get_state() == State::READY) {
            logger.log_event(current_time, "User " + user->user_id, process->process_id, "Started");
        }

        logger.log_event(current_time, "User " + user->user_id, process->process_id, "Resumed");

        int actual_run_time = process->run(per_process_quantum);

        current_time += actual_run_time;

        if (process->is_finished()) {
            logger.log_event(current_time, "User " + user->user_id, process->process_id, "Paused");

            logger.log_event(current_time, "User " + user->user_id, process->process_id, "Finished");
        } else {
            logger.log_event(current_time, "User " + user->user_id, process->process_id, "Paused");
        }
    }
}



