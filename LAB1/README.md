# Lab 1: Multithreaded Merge Sort

## Overview
This project implements a parallel version of the merge sort algorithm using C++ threads. The program demonstrates multithreading concepts, thread synchronization, and divide-and-conquer parallelization techniques.

## Algorithm Description
The multithreaded merge sort works by:
1. **Divide**: Recursively split the array into smaller subarrays
2. **Conquer**: Sort each subarray in parallel using separate threads
3. **Combine**: Merge the sorted subarrays back together

## Key Features
- **Parallel Processing**: Each recursive call spawns two new threads for left and right halves
- **Thread Synchronization**: Uses mutex locks to ensure thread-safe logging
- **Dynamic Threading**: Threads are created dynamically based on array size
- **Comprehensive Logging**: Tracks thread creation, execution, and completion

## Technical Implementation

### Thread Management
- Each thread is assigned a unique ID for tracking
- Thread IDs are generated using a hierarchical numbering system (parent * 10 + offset)
- Thread-safe logging prevents race conditions in output

### Synchronization
- `std::mutex` ensures exclusive access to output file
- `std::lock_guard` provides RAII-style mutex management
- Thread-safe logging with immediate flush for real-time output

### Memory Management
- Efficient vector operations for array manipulation
- Temporary vectors for merge operations
- Automatic memory cleanup through RAII

## File Structure
```
LAB1/
├── Assignment1.cpp    # Main implementation
├── input.txt         # Input data file
├── output.txt        # Generated output (created at runtime)
└── README.md         # This documentation
```

## Compilation and Execution

### Prerequisites
- C++ compiler with C++11 support
- POSIX threads library

### Build Instructions
```bash
# Compile the program
g++ -std=c++11 -pthread Assignment1.cpp -o Assignment1

# Run the program
./Assignment1
```

### Input Format
The program reads integers from `input.txt`, one per line:
```
5
2
8
1
9
3
```

### Output Format
The program generates detailed logging in `output.txt`:
```
Given vector is:
5 2 8 1 9 3 

Thread 1 started
Thread 10 started
Thread 11 started
...
Thread 1 finished: 1 2 3 5 8 9 

Sorted vector is:
1 2 3 5 8 9 
```

## Performance Analysis

### Thread Overhead
- **Advantage**: Parallel processing reduces theoretical time complexity
- **Challenge**: Thread creation overhead may impact small datasets
- **Optimization**: Consider thread pool for production use

### Scalability
- Thread count grows exponentially with array size
- Memory usage increases with thread count
- Optimal performance depends on system capabilities

## Learning Outcomes
This project demonstrates:
- **Concurrency**: Thread creation, management, and synchronization
- **Algorithm Design**: Parallel algorithm implementation
- **System Programming**: Low-level thread operations
- **Performance Analysis**: Understanding thread overhead vs. parallelization benefits

## Future Enhancements
- Thread pool implementation to reduce overhead
- Configurable thread count limits
- Performance benchmarking and comparison tools
- Support for different data types

## Authors
Mik Driver (40244456)
Victor Depaz (40242703)
