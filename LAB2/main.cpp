/*
fair-share process scheduler
LAB ASSIGNMENT 2
Mik Driver (40244456)

g++ -std=c++14 main.cpp EventLogger.cpp Process.cpp User.cpp Scheduler.cpp -o scheduler -pthread
./scheduler
*/

#include <iostream>
#include <fstream>
#include "Scheduler.h"
#include "User.h"

using namespace std;

int main() {
    ifstream inputFile("input.txt");
    if (!inputFile) {
        cerr << "Unable to open input file!" << endl;
        return 1;
    }

    int time_quantum;
    inputFile >> time_quantum;

    Scheduler scheduler(time_quantum, "output.txt");

    string user_name;
    int num_processes;

    while (inputFile >> user_name >> num_processes) {
        auto user = std::make_unique<User>(user_name);

        for (int i = 0; i < num_processes; ++i) {
            int ready_time, service_time;
            inputFile >> ready_time >> service_time;

            user->add_process(i, ready_time, service_time); 
        }

        scheduler.add_user(std::move(user));
    }



    inputFile.close();

    scheduler.run_scheduler();

    return 0;
}

