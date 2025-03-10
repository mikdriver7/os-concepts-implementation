#include "User.h"

User::User(int id) : user_id(id) {}

void User::add_process(int process_id, int ready_time, int service_time) {
    processes.emplace_back(process_id, user_id, ready_time, service_time);
}

std::vector<Process*> User::get_ready_processes(int current_time) {
    std::vector<Process*> ready_processes;
    for (auto& process : processes) {
        if (!process.is_finished() && process.ready_time <= current_time) {
            ready_processes.push_back(&process);
        }
    }
    return ready_processes;
}
