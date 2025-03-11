#include "Process.h"
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

// Constructor
Process::Process(int id, const std::string& uid, int ready, int service)
    : process_id(id),
      user_id(uid),
      ready_time(ready),
      service_time(service),
      remaining_time(service),
      finished(false),
      state(State::READY),
      should_run(false),
      allocated_time_slice(0),
      terminate_thread(false) {}

// Destructor - make sure the thread is stopped
Process::~Process() {
    stop();
}

// Start the thread
void Process::start() {
    if (thread_obj.joinable()) return;  // Don't restart a running thread!
    thread_obj = thread(&Process::process_thread, this);
}


// Stop the thread gracefully
void Process::stop() {
    {
        unique_lock<mutex> lock(mtx);
        terminate_thread = true;
        cv.notify_one();
    }

    if (thread_obj.joinable()) {
        thread_obj.join();
    }
}

// The function run by the thread
void Process::process_thread() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);

        cv.wait(lock, [this]() { return should_run || terminate_thread; });

        if (terminate_thread) {
            break;
        }

        if (state == State::FINISHED) {
            should_run = false;
            done_cv.notify_one();
            continue;
        }

        state = State::RUNNING;

        int run_time = std::min(allocated_time_slice, remaining_time);
        last_run_time = run_time;

        std::cout << "Process " << process_id << " running for " << run_time << " seconds" << std::endl;

        lock.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(run_time));
        lock.lock();

        remaining_time -= run_time;

        if (remaining_time <= 0) {
            finished = true;
            state = State::FINISHED;
            std::cout << "Process " << process_id << " finished!" << std::endl;
        } else {
            state = State::PAUSED;
        }

        should_run = false;
        done_cv.notify_one();
    }
}



// Scheduler calls this to give the process CPU time
int Process::run(int time_slice) {
    std::unique_lock<std::mutex> lock(mtx);

    if (finished || state == State::FINISHED) {
        return 0;
    }

    allocated_time_slice = time_slice;
    should_run = true;

    cv.notify_one();

    // Wait until the thread finishes its time slice
    done_cv.wait(lock, [this]() { return !should_run; });

    return last_run_time; 
}





bool Process::is_finished() const {
    std::unique_lock<std::mutex> lock(mtx);
    return finished;
}

State Process::get_state() const {
    return state;
}

void Process::set_state(State new_state) {
    unique_lock<mutex> lock(mtx);
    state = new_state;
}
