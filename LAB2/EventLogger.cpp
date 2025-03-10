#include "EventLogger.h"
#include <iostream>
#include <mutex>

// Define a mutex for thread-safe logging
std::mutex log_mutex;

EventLogger::EventLogger(const std::string& filename) {
    log_file.open(filename);
    if (!log_file.is_open()) {
        std::cerr << "Error: Could not open log file " << filename << std::endl;
    } else {
        log_file.setf(std::ios::unitbuf);  // Disable buffering
    }
}

EventLogger::~EventLogger() {
    if (log_file.is_open()) {
        log_file.close();
    }
}

void EventLogger::log_event(int time, const std::string& user, int process_id, const std::string& event) {
    std::lock_guard<std::mutex> lock(log_mutex);  // Lock the mutex
    if (log_file.is_open()) {
        log_file << "Time " << time << ", User " << user << ", Process " << process_id << ": " << event << std::endl;
        log_file.flush();  // Ensure the event is immediately written to the log file
    } else {
        std::cerr << "Error: Log file is not open." << std::endl;
    }
}

