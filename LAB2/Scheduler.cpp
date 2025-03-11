#include "Scheduler.h"
#include <iostream>
#include <chrono>
#include <algorithm>

using namespace std;

// Initializes the scheduler with a given time quantum and a logger for events.
Scheduler::Scheduler(int quantum, const std::string& log_filename)
    : time_quantum(quantum), current_time(1), logger(log_filename), running(false) {}

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

    // Step 1: Collect users with ready processes
    for (auto& user : users) {
        auto ready_processes = user->get_ready_processes(current_time);
        if (!ready_processes.empty()) {
            active_users.push_back(user.get());
        }
    }

    // If no users are ready, advance time and return
    if (active_users.empty()) {
        current_time += 1;
        return;
    }

    // Make sure index is in bounds
    if (current_user_index >= active_users.size()) {
        current_user_index = 0;
    }

    // Get the current user in round-robin fashion
    User* user = active_users[current_user_index];

    auto ready_processes = user->get_ready_processes(current_time);

    if (!ready_processes.empty()) {
        int process_quantum = time_quantum / active_users.size();
        if (process_quantum == 0) process_quantum = 1;

        auto process = ready_processes.front();

        if (!process->is_finished()) {
            int start_time = current_time;

            // First time ever? Log Started
            if (process->get_state() == State::READY) {
                logger.log_event(start_time, "User " + user->user_id, process->process_id, "Started");
            }

            logger.log_event(start_time, "User " + user->user_id, process->process_id, "Resumed");

            int actual_run_time = process->run(process_quantum);

            int end_time = current_time + actual_run_time;

            if (process->is_finished()) {
                logger.log_event(end_time, "User " + user->user_id, process->process_id, "Finished");
            } else {
                logger.log_event(end_time, "User " + user->user_id, process->process_id, "Paused");
            }

            // Advance time
            current_time = end_time;
        }
    }

    // Move to the next user index for round-robin fairness
    current_user_index++;

    // Wrap-around if we reach the end
    if (current_user_index >= active_users.size()) {
        current_user_index = 0;
    }
}




