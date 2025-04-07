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

    Command();
    Command(const std::string& rawLine);
};

#endif
