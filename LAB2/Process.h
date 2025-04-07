#pragma once

#include <condition_variable>
#include <thread>
#include <string>

// Represents process state
enum class State { READY, RUNNING, PAUSED, FINISHED };

class Process {
public:
    // Constructor / Destructor
    Process(int id, const std::string& uid, int ready, int service);
    ~Process();

    // Thread control
    void start();     // Launch the process thread
    void stop();      // Gracefully stop the process thread

    // Run request
    int run(int time_slice);  // Request to run for time_slice and wait for it to finish

    // Process status queries
    bool is_finished() const;
    State get_state() const;
    void set_state(State new_state);

    // Public read-only data 
    int process_id;
    std::string user_id;
    int ready_time;
    int service_time;
    int remaining_time;

private:
    // Internal thread logic
    void process_thread();

    // Threading and synchronization
    std::thread thread_obj;
    mutable std::mutex mtx; //  mutable so const functions can lock
    std::condition_variable cv;      // Waits for "run" permission
    std::condition_variable done_cv; // Notifies Scheduler when done running

    // Flags
    bool should_run;
    bool terminate_thread;

    // State and runtime tracking
    int allocated_time_slice;
    int last_run_time;  //  Added to track how much time the process ran in its last slice
    bool finished;      // True when the process finishes all work
    State state;
};

