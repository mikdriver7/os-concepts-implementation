#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <string>
using namespace std;
mutex logMutex; 
vector<string> threadLogs; 


vector<int> merge(const std::vector<int>& left, const std::vector<int>& right) {
    vector<int> result;
    size_t i = 0, j = 0;

    while (i < left.size() && j < right.size()) {
        if (left[i] < right[j]) {
            result.push_back(left[i]);
            ++i;
        }
        else {
            result.push_back(right[j]);
            ++j;
        }
    }

    
    result.insert(result.end(), left.begin() + i, left.end());
    result.insert(result.end(), right.begin() + j, right.end());

    return result;
}


void mergeSort(std::vector<int>& arr, const std::string& threadId) {
    {
        lock_guard<std::mutex> lock(logMutex);
        threadLogs.push_back("Thread " + threadId + " started");
    }

    if (arr.size() <= 1) {
        {
            lock_guard<mutex> lock(logMutex);
            threadLogs.push_back("Thread " + threadId + " finished: " + std::to_string(arr[0]));
        }
        return;
    }

    size_t mid = arr.size() / 2;
    vector<int> left(arr.begin(), arr.begin() + mid);
    vector<int> right(arr.begin() + mid, arr.end());

    // Create threads
    string leftThreadId = threadId + "0";
    string rightThreadId = threadId + "1";

    vector<int> leftSorted;
    vector<int> rightSorted;

    thread leftThread([&left, &leftSorted, leftThreadId]() {
        mergeSort(left, leftThreadId);
    leftSorted = left;
        });

    thread rightThread([&right, &rightSorted, rightThreadId]() {
        mergeSort(right, rightThreadId);
    rightSorted = right;
        });

    // Start and wait for threads
    leftThread.join();
    rightThread.join();

    // Merge results
    vector<int> merged = merge(leftSorted, rightSorted);

    {
        lock_guard<mutex> lock(logMutex);
        string result = "[";
        for (size_t i = 0; i < merged.size(); ++i) {
            result += to_string(merged[i]);
            if (i < merged.size() - 1) result += ", ";
        }
        result += "]";
        threadLogs.push_back("Thread " + threadId + " finished: " + result);
    }

    arr = merged;
}


vector<int> readInput(const string& filename) {
    ifstream infile(filename);
    vector<int> arr;
    int num;

    while (infile >> num) {
        arr.push_back(num);
    }

    return arr;
}

// Write logs
void writeOutput(const string& filename) {
    ofstream outfile(filename);
    for (const auto& log : threadLogs) {
        outfile << log << endl;
    }
}

int main() {
    // Read input file
    vector<int> arr = readInput("input.txt");

    // Start mergesort algorithm
    mergeSort(arr, "1");

    // Write log in output file
    writeOutput("output.txt");

    return 0;
}
