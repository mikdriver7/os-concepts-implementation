#ifndef EVENTLOGGER_H
#define EVENTLOGGER_H

#include <fstream>
#include <string>
#include <mutex>

class EventLogger {
public:
    EventLogger(const std::string& filename);
    ~EventLogger();

    void log_event(int time, const std::string& user, int process_id, const std::string& event);

private:
    std::ofstream log_file;
    std::mutex log_mutex;
};

#endif // EVENTLOGGER_H
