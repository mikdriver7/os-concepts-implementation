#include "Command.h"
#include <sstream>

Command::Command() : type(UNKNOWN), variableId(""), value(0) {}

Command::Command(const std::string& rawLine) {
    std::istringstream iss(rawLine);
    std::string cmd;
    iss >> cmd;

    if (cmd == "Store") {
        type = STORE;
        iss >> variableId >> value;
    }
    else if (cmd == "Release") {
        type = RELEASE;
        iss >> variableId;
    }
    else if (cmd == "Lookup") {
        type = LOOKUP;
        iss >> variableId;
    }
    else {
        type = UNKNOWN;
    }
}
