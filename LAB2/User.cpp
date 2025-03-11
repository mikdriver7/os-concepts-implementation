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

Process* User::get_next_ready_process(int current_time) {
    auto ready_processes = get_ready_processes(current_time);

    if (ready_processes.empty()) {
        return nullptr; // No process ready to run
    }

    if (current_process_index >= ready_processes.size()) {
        current_process_index = 0; // Wrap around if index too big
    }

    // Pick the next process in RR order
    Process* next_process = ready_processes[current_process_index];

    // Move index forward for next round
    current_process_index++;

    return next_process;
}
