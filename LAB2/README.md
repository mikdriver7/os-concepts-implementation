# Lab 2: Fair-Share Process Scheduler

## Overview
This project implements a fair-share process scheduler that manages multiple users and their processes with equal resource allocation. The scheduler uses a round-robin algorithm with a configurable time quantum to ensure fair CPU time distribution among users.

## Algorithm Description
The fair-share scheduler operates on the following principles:
1. **User Equality**: Each user receives equal CPU time regardless of process count
2. **Round-Robin**: Processes within a user are scheduled in round-robin fashion
3. **Time Quantum**: Configurable time slice for each process execution
4. **Fair Distribution**: CPU time is distributed proportionally among users

## Key Features
- **Multi-User Support**: Manages multiple users with their respective processes
- **Fair-Share Algorithm**: Ensures equal resource allocation among users
- **Round-Robin Scheduling**: Processes are scheduled in circular order
- **Comprehensive Logging**: Detailed event logging for analysis
- **Configurable Parameters**: Adjustable time quantum and user management

## Technical Implementation

### Core Components

#### Scheduler Class
- Manages the overall scheduling algorithm
- Maintains user queues and process states
- Implements fair-share time distribution logic

#### User Class
- Represents a user with associated processes
- Manages process queue for the user
- Tracks user-specific scheduling metrics

#### Process Class
- Individual process representation
- Tracks execution time, service time, and state
- Manages process lifecycle and transitions

#### EventLogger Class
- Thread-safe logging system
- Records all scheduling events and decisions
- Provides detailed execution trace

### Scheduling Algorithm
1. **Initialization**: Load users and processes from input file
2. **Time Quantum Distribution**: Allocate equal time to each user
3. **Process Selection**: Select next process from current user's queue
4. **Execution**: Run process for allocated time quantum
5. **State Management**: Update process states and user metrics
6. **Logging**: Record all scheduling decisions and events

## File Structure
```
LAB2/
├── main.cpp           # Program entry point
├── Scheduler.cpp/h    # Core scheduler implementation
├── Process.cpp/h      # Process management
├── User.cpp/h         # User management
├── EventLogger.cpp/h  # Logging system
├── input.txt          # Input configuration
├── output.txt         # Generated output (created at runtime)
└── README.md          # This documentation
```

## Compilation and Execution

### Prerequisites
- C++ compiler with C++14 support
- POSIX threads library

### Build Instructions
```bash
# Compile the program
g++ -std=c++14 main.cpp EventLogger.cpp Process.cpp User.cpp Scheduler.cpp -o scheduler -pthread

# Run the program
./scheduler
```

### Input Format
The program reads configuration from `input.txt`:
```
2                    # Time quantum
user1 3              # User name and number of processes
0 5                  # Process ready time and service time
1 3
2 4
user2 2              # Another user
0 4
1 6
```

### Output Format
The program generates detailed scheduling logs in `output.txt`:
```
Time 0: Process P1 from user1 started
Time 2: Process P1 from user1 finished
Time 2: Process P2 from user1 started
...
```

## Algorithm Analysis

### Fairness Metrics
- **User Fairness**: Equal CPU time allocation among users
- **Process Fairness**: Round-robin scheduling within user queues
- **Response Time**: Minimized waiting time for process execution

### Performance Characteristics
- **Time Complexity**: O(n) per scheduling decision
- **Space Complexity**: O(n) for user and process queues
- **Scalability**: Efficient handling of multiple users and processes

## Learning Outcomes
This project demonstrates:
- **Process Scheduling**: Implementation of scheduling algorithms
- **Resource Management**: Fair allocation of system resources
- **System Design**: Modular architecture with clear separation of concerns
- **Event Logging**: Comprehensive system monitoring and debugging

## Configuration Options
- **Time Quantum**: Adjustable process execution time slice
- **User Management**: Dynamic user addition and removal
- **Process Parameters**: Configurable ready time and service time
- **Logging Level**: Detailed or summary output options

## Future Enhancements
- Priority-based scheduling within users
- Dynamic time quantum adjustment
- Performance metrics and statistics
- Interactive configuration interface

## Authors
Mik Driver (40244456)

Victor Depaz (40242703)
