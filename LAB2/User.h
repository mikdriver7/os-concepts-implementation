#ifndef USER_H
#define USER_H

#include <vector>
#include "Process.h"

class User {
public:
    int user_id;
    std::vector<Process> processes;

    User(int id);
    void add_process(int process_id, int ready_time, int service_time);
    std::vector<Process*> get_ready_processes(int current_time);
};

#endif // USER_H