# Lab 3: Virtual Memory Management System

## Overview
This project implements a comprehensive virtual memory management system with integrated process scheduling. The system features page replacement algorithms, disk-based virtual memory storage, multi-core process scheduling, and sophisticated memory management capabilities.

## System Architecture

### Core Components

#### Virtual Memory Manager (VMM)
- Manages virtual-to-physical address translation
- Implements page replacement algorithms
- Handles page faults and memory allocation
- Coordinates between main memory and disk storage

#### Main Memory
- Physical memory simulation with configurable page count
- Page allocation and deallocation management
- Memory access tracking and statistics

#### Disk Manager
- Virtual disk storage for swapped pages
- Page read/write operations
- Disk I/O simulation and timing

#### Process Scheduler
- Multi-core process scheduling
- Process state management
- Command execution and monitoring
- Integration with the virtual memory system

#### System Clock
- Global time management
- Event timing and synchronization
- Process scheduling coordination

## Key Features

### Memory Management
- **Virtual Memory**: Full virtual memory implementation with paging
- **Page Replacement**: Efficient page replacement algorithms
- **Memory Allocation**: Dynamic page allocation and deallocation
- **Page Fault Handling**: Automatic page fault resolution

### Process Management
- **Multi-Core Scheduling**: Support for multiple CPU cores
- **Command Execution**: Process command processing and execution
- **State Management**: Comprehensive process state tracking
- **Resource Coordination**: Integration with memory and I/O systems

### System Integration
- **Event Logging**: Comprehensive system event logging
- **Configuration Management**: Flexible system configuration
- **Performance Monitoring**: System performance tracking
- **Modular Design**: Clean separation of system components

## Technical Implementation

### Memory Management Algorithm
1. **Page Allocation**: Allocate pages to processes on demand
2. **Page Replacement**: Select pages for replacement when memory is full
3. **Page Fault Handling**: Load pages from disk when accessed
4. **Memory Cleanup**: Deallocate pages when processes terminate

### Process Scheduling
1. **Multi-Core Support**: Distribute processes across available cores
2. **Command Processing**: Execute process commands in sequence
3. **State Transitions**: Manage process lifecycle states
4. **Resource Coordination**: Coordinate with memory and I/O systems

### System Coordination
1. **Clock Synchronization**: Global time management across components
2. **Event Logging**: Comprehensive system event recording
3. **Configuration Loading**: Dynamic system configuration
4. **Performance Tracking**: Monitor system performance metrics

## File Structure
```
LAB3/
├── main.cpp                    # Program entry point
├── VirtualMemoryManager.cpp/h  # Virtual memory management
├── Scheduler.cpp/h            # Process scheduler
├── MainMemory.cpp/h           # Physical memory management
├── DiskManager.cpp/h          # Virtual disk management
├── Process.cpp/h              # Process implementation
├── Page.cpp/h                 # Page management
├── Clock.cpp/h                # System clock
├── Logger.cpp/h               # Logging system
├── Command.cpp/h              # Command processing
├── memconfig.txt              # Memory configuration
├── processes.txt              # Process configuration
├── commands.txt               # Command definitions
├── vm.txt                     # Virtual memory configuration
├── output.txt                 # Generated output (created at runtime)
└── README.md                  # This documentation
```

## Compilation and Execution

### Prerequisites
- C++ compiler with C++14 support
- POSIX threads library

### Build Instructions
```bash
# Compile the program
g++ -std=c++14 main.cpp Clock.cpp Logger.cpp VirtualMemoryManager.cpp Process.cpp Scheduler.cpp MainMemory.cpp DiskManager.cpp Command.cpp Page.cpp -o scheduler -pthread

# Run the program
./scheduler
```

### Configuration Files

#### Memory Configuration (memconfig.txt)
```
10    # Number of physical memory pages
```

#### Process Configuration (processes.txt)
```
2     # Number of CPU cores
3     # Number of processes
0 10  # Process start time and duration
2 15
5 8
```

#### Command Configuration (commands.txt)
```
READ 100
WRITE 200 42
READ 300
```

#### Virtual Memory Configuration (vm.txt)
```
# Virtual memory disk file (created automatically)
```

## System Operation

### Initialization Phase
1. Load memory configuration
2. Initialize virtual memory manager
3. Set up disk storage
4. Load process and command configurations

### Execution Phase
1. Start system clock
2. Begin process scheduling
3. Handle memory operations
4. Process commands and page faults
5. Log all system events

### Termination Phase
1. Complete all running processes
2. Flush memory to disk
3. Generate final statistics
4. Clean up system resources

## Performance Analysis

### Memory Efficiency
- **Page Hit Rate**: Percentage of successful memory accesses
- **Page Fault Rate**: Frequency of page faults requiring disk I/O
- **Memory Utilization**: Percentage of physical memory in use

### System Performance
- **Throughput**: Processes completed per unit time
- **Response Time**: Average time for process completion
- **Resource Utilization**: CPU and memory usage efficiency

## Learning Outcomes
This project demonstrates:
- **Virtual Memory**: Implementation of virtual memory systems
- **Page Replacement**: Algorithm design and implementation
- **Process Scheduling**: Multi-core process management
- **System Integration**: Complex system component coordination
- **Performance Analysis**: System performance evaluation

## Configuration Options
- **Memory Size**: Configurable physical memory page count
- **CPU Cores**: Adjustable number of processing cores
- **Process Parameters**: Configurable process timing and commands
- **Logging Level**: Detailed or summary system logging

## Future Enhancements
- Advanced page replacement algorithms (LRU, LFU)
- Memory prefetching and caching strategies
- Real-time performance monitoring
- Interactive system configuration interface

## Authors
Mik Driver (40244456)

Victor Depaz (40242703)
