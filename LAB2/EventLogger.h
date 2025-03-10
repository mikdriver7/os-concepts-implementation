#ifndef EVENTLOGGER_H
#define EVENTLOGGER_H

#include <string>
#include <fstream>

class EventLogger {
private:
    std::ofstream log_file;

public:
    EventLogger(const std::string& filename);
    ~EventLogger();
    void log_event(int time, const std::string& user, int process_id, const std::string& event);
};

#endif // EVENTLOGGER_H