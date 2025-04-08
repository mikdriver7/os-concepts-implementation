#ifndef COMMAND_H
#define COMMAND_H

#include <string>

enum CommandType {
    STORE,
    RELEASE,
    LOOKUP,
    UNKNOWN
};

struct Command {
    CommandType type;
    std::string variableId;
    unsigned int value; // Only used for STORE

    bool executed;      // Flag to track if the command has been executed
    int id;             // Unique ID for the command to identify it in the scheduler

    Command();
    Command(const std::string& rawLine, int commandId);
    
        // Method to mark the command as executed
    void markExecuted() {
        executed = true;
    }

    // Check if the command has been executed
    bool isExecuted() const {
        return executed;
    }
};

#endif
