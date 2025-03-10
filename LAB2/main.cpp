/*
fair-share process scheduler
LAB ASSIGNMENT 2
Mik Driver (40244456)



g++ -std=c++11 main.cpp EventLogger.cpp Process.cpp User.cpp Scheduler.cpp -o scheduler -pthread
./scheduler
*/

#include <iostream>
#include "Scheduler.h"
#include "User.h"

using namespace std;


int main() {
    Scheduler scheduler(4, "output.txt");

    User user1(1);
    user1.add_process(0, 1, 5);
    user1.add_process(1, 4, 3);

    User user2(2);
    user2.add_process(0, 5, 6);

    scheduler.add_user(user1);
    scheduler.add_user(user2);

    scheduler.run_scheduler();

    return 0;
}