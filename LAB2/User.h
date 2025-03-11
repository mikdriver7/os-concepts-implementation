#ifndef USER_H
#define USER_H

#include <vector>
#include <string>
 #include <memory>
#include "Process.h"


class User {
public:
    explicit User(const std::string& id);
    ~User();

    void add_process(int process_id, int ready_time, int service_time);
    std::vector<Process*> get_ready_processes(int current_time);

    void start_processes();
    void stop_processes();

    std::string user_id;

    bool is_all_processes_finished() const;
    Process* get_next_ready_process(int current_time);



private:
    std::vector<std::unique_ptr<Process>> processes;
      size_t current_process_index = 0;
};

#endif // USER_H
