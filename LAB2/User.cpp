#include "User.h"

// Constructor
User::User(const std::string& id) : user_id(id) {}

// Destructor - Ensure processes are stopped
User::~User() {
    stop_processes();
}

// Add a new process to the user
void User::add_process(int process_id, int ready_time, int service_time) {
    processes.emplace_back(std::make_unique<Process>(process_id, user_id, ready_time, service_time));
}

// Get processes ready at the current time
std::vector<Process*> User::get_ready_processes(int current_time) {
    std::vector<Process*> ready_processes;
    for (auto& process : processes) {
        if (!process->is_finished() && process->ready_time <= current_time) {
            ready_processes.push_back(process.get());
        }
    }
    return ready_processes;
}

// Start all process threads
void User::start_processes() {
    for (auto& process : processes) {
        process->start();
    }
}

// Stop all process threads
void User::stop_processes() {
    for (auto& process : processes) {
        process->stop();
    }
}

bool User::is_all_processes_finished() const {
    for (const auto& process : processes) {
        if (!process->is_finished()) {
            return false;
        }
    }
    return true;
}
