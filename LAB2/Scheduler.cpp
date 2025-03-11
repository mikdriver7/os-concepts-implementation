#include "Scheduler.h"
#include "Process.h"
#include <iostream>
using namespace std;
#include <chrono>
#include <algorithm>

Scheduler::Scheduler(int quantum, const std::string& log_filename)
    : time_quantum(quantum), current_time(0), logger(log_filename), running(false) {}

void Scheduler::add_user(const User& user) {
    users.push_back(user);
}

void Scheduler::run_scheduler() {
    current_time = 1;  // Start at time 0

    while (true) {
        // Check if all processes are finished
        bool all_finished = true;
        for (auto& user : users) {
            for (auto& process : user.processes) {
                if (!process.is_finished()) {
                    all_finished = false;
                    break;
                }
            }
            if (!all_finished) break;
        }
        if (all_finished) break;

        // Increment time and check at each step
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Sleep for 1 second
       

        // Distribute time quantum among users and processes
        distribute_quantum();
    }
}

// checkpoint

// checkpoint

void Scheduler::distribute_quantum() {
    std::vector<User*> active_users;
    std::vector<User*> all_users;
    for (auto& user : users) {
        all_users.push_back(&user);
        auto ready_processess = user.get_ready_processes(current_time);
        if (!ready_processess.empty()) {
            active_users.push_back(&user);
        }
        
    }

    if (active_users.empty()) return;

    for (auto user : active_users) {

        
        

        // Divide the time quantum equally between active users
        int user_quantum = time_quantum / active_users.size();

        auto ready_processes = user->get_ready_processes(current_time);
        int proc = 0;
        for (auto process : ready_processes) {
            proc++;

        }
        int process_quantum = user_quantum;
        if (proc != 0) {
            
            process_quantum = user_quantum / proc;
        }

        
            
        for (auto process : ready_processes) {
            std::unique_lock<std::mutex> lock(mtx);
            running = true;
            cv.notify_all();

            // Log the "Started" event when process starts
            if (process->get_state() == State::READY) {
                logger.log_event(current_time, "User" + std::to_string(user->user_id), process->process_id, "Started");
                process->set_state(State::RUNNING);
            }

            // Log "Resumed" if the process is already running
            if (process->get_state() == State::RUNNING) {
                logger.log_event(current_time, "User" + std::to_string(user->user_id), process->process_id, "Resumed");
            }

            // Run the process for the allocated quantum or until it finishes
            int run_time = std::min(process_quantum, process->remaining_time);
            process->run(run_time, current_time);
            current_time=current_time+run_time;

            // Pause the process after running for the quantum or if it's finished
            logger.log_event(current_time, "User" + std::to_string(user->user_id), process->process_id, "Paused");
            process->set_state(State::RUNNING);

            running = false;
            cv.notify_all();

            // Finish the process if it's done running
            if (process->is_finished()) {
                logger.log_event(current_time, "User" + std::to_string(user->user_id), process->process_id, "Finished");
                process->set_state(State::FINISHED);
            }
        }
    }
}
