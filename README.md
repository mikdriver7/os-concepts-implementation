# COEN346 - Operating Systems Laboratory Projects

This repository contains three comprehensive operating systems projects developed as part of COEN346 (Operating Systems Laboratory) coursework. Each project demonstrates different aspects of operating system concepts including multithreading, process scheduling, and memory management.

## Projects Overview

### [Lab 1: Multithreaded Merge Sort](./LAB1/)
**Concepts:** Multithreading, Thread Synchronization, Divide-and-Conquer Algorithms

A parallel implementation of the merge sort algorithm using C++ threads. The program demonstrates:
- Thread creation and management
- Mutex-based synchronization
- Divide-and-conquer parallelization
- Thread-safe logging and output

**Key Features:**
- Recursive thread spawning for parallel sorting
- Thread-safe logging with mutex protection
- Configurable input/output file handling
- Performance comparison capabilities

### [Lab 2: Fair-Share Process Scheduler](./LAB2/)
**Concepts:** Process Scheduling, Fair-Share Scheduling, Round-Robin Algorithm

A comprehensive process scheduler implementing fair-share scheduling with round-robin time quantum. The system manages multiple users and their processes with equal resource allocation.

**Key Features:**
- Fair-share scheduling algorithm
- Round-robin time quantum management
- Multi-user process management
- Comprehensive event logging
- Configurable time quantum

### [Lab 3: Virtual Memory Management System](./LAB3/)
**Concepts:** Virtual Memory, Page Replacement, Memory Management, Process Scheduling

A sophisticated virtual memory management system with integrated process scheduling. Implements page replacement algorithms, disk management, and multi-core process scheduling.

**Key Features:**
- Virtual memory management with paging
- Page replacement algorithms
- Disk-based virtual memory storage
- Multi-core process scheduling
- Comprehensive memory and process logging
- Command-based process execution

## Technical Stack

- **Language:** C++14/17
- **Threading:** POSIX threads (pthread)
- **Build System:** GNU Make
- **Platform:** Cross-platform (Linux/macOS)

## Project Structure

```
COEN346/
├── LAB1/                    # Multithreaded Merge Sort
│   ├── Assignment1.cpp      # Main implementation
│   ├── input.txt           # Sample input data
│   ├── output.txt          # Generated output
│   └── README.md           # Detailed documentation
├── LAB2/                    # Fair-Share Scheduler
│   ├── main.cpp            # Entry point
│   ├── Scheduler.cpp/h     # Core scheduler logic
│   ├── Process.cpp/h       # Process management
│   ├── User.cpp/h          # User management
│   ├── EventLogger.cpp/h   # Logging system
│   └── README.md           # Detailed documentation
├── LAB3/                    # Virtual Memory Management
│   ├── main.cpp            # Entry point
│   ├── VirtualMemoryManager.cpp/h  # VM management
│   ├── Scheduler.cpp/h     # Process scheduler
│   ├── MainMemory.cpp/h    # Physical memory
│   ├── DiskManager.cpp/h   # Virtual disk
│   ├── Process.cpp/h       # Process implementation
│   ├── Page.cpp/h          # Page management
│   ├── Clock.cpp/h         # System clock
│   ├── Logger.cpp/h        # Logging system
│   ├── Command.cpp/h       # Command processing
│   └── README.md           # Detailed documentation
├── Makefile                # Build configuration
├── .gitignore             # Git ignore rules
└── README.md              # This file
```

## Learning Outcomes

These projects demonstrate proficiency in:

- **System Programming:** Low-level C++ programming with system calls
- **Concurrency:** Multithreading, synchronization, and race condition handling
- **Memory Management:** Virtual memory, paging, and memory allocation strategies
- **Process Management:** Scheduling algorithms and process lifecycle management
- **File I/O:** Efficient file handling and data persistence
- **Software Architecture:** Modular design with clear separation of concerns

## Performance Considerations

- **Lab 1:** Demonstrates parallel algorithm efficiency with thread overhead analysis
- **Lab 2:** Shows fair resource allocation and scheduling fairness metrics
- **Lab 3:** Implements efficient memory management with page fault optimization

## Authors
Mik Driver (40244456)
Victor Depaz (40242703)

---

*These projects were developed as part of COEN346 Operating Systems Laboratory coursework, demonstrating practical implementation of core operating system concepts.*
