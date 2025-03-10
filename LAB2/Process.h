#ifndef PROCESS_H
#define PROCESS_H

// Enum class to represent the different states of the process
enum class State {
    READY,
    RUNNING,
    PAUSED,
    FINISHED
};

class Process {
public:
    int process_id;
    int user_id;
    int ready_time;
    int service_time;
    int remaining_time;
    bool finished;
    State state;  // To track the current state of the process

    // Constructor to initialize the process
    Process(int id, int uid, int ready, int service);

    // Method to run the process with a given time slice
    void run(int time_slice, int& current_time);

    // Method to check if the process is finished
    bool is_finished() const;

    // Getter for the process state
    State get_state() const;

    // Setter for the process state
    void set_state(State new_state);
};

#endif // PROCESS_H
