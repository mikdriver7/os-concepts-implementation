#include "Process.h"
#include <iostream>
using namespace std;
// Constructor to initialize a process
Process::Process(int id, int uid, int ready, int service)
    : process_id(id), user_id(uid), ready_time(ready), service_time(service), remaining_time(service), finished(false), state(State::READY) {}

// Method to run the process with a given time slice
void Process::run(int time_slice, int& current_time) {
    remaining_time -= time_slice;  // Subtract time slice from remaining time

    

    if (remaining_time > 0) {
        state = State::RUNNING;  // The process is still running
    }
    else {
        state = State::FINISHED;  // Process is finished
    }
}

// Method to check if the process is finished
bool Process::is_finished() const {
    return remaining_time <= 0;
}

// Getter for the process state
State Process::get_state() const {
    return state;
}

// Setter for the process state
void Process::set_state(State new_state) {
    state = new_state;
}
