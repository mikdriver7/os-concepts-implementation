/*
COEN 346
Lab Assignment #1
Merge-Sort using multithreading
Mik Driver (40244456)

Merge sort reference: https://www.geeksforgeeks.org/merge-sort/

Terminal Input to run: g++ -std=c++11 -pthread Assignment1-Mik.cpp -o Assignment1-Mik ./Assignment1-Mik
*/


#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

// Global Variables
mutex mtx;
int threadCounter = 1;

// Functions
void logThreadStart(int threadID) {
    lock_guard<mutex> lock(mtx);
    cout << "Thread " << threadID << " started" << endl;
}

void logThreadFinish(int threadID, vector<int>& sublist) {
    lock_guard<mutex> lock(mtx);
    cout << "Thread " << threadID << " finished: ";
    for (int num : sublist) cout << num << " ";
    cout << endl;
}

void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temp vectors
    vector<int> L(n1), R(n2);

    // Copy data to temp vectors L[] and R[]
    for (int i = 0; i < n1; i++) { L[i] = arr[left + i]; }
    for (int j = 0; j < n2; j++) { R[j] = arr[mid + 1 + j]; }

    int i = 0, j = 0, k = left;

    // Merge the temp vectors back into arr[left..right]
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(vector<int>& arr, int left, int right, int threadID) {
    if (left >= right) { return; }

    int mid = left + (right - left) / 2;

   logThreadStart(threadID);

    int leftThreadID = threadID * 10;
    int rightThreadID = leftThreadID + 1;
    
    std::thread leftThread, rightThread;
    if (threadID < 1000) {
        leftThread = thread(mergeSort, ref(arr), left, mid, leftThreadID);
        rightThread = thread(mergeSort, ref(arr), mid + 1, right, rightThreadID);
        leftThread.join();
        rightThread.join();
    } else {
        mergeSort(arr, left, mid, leftThreadID);
        mergeSort(arr, mid + 1, right, rightThreadID);
    }

    merge(arr, left, mid, right);
    vector<int> sublist(arr.begin() + left, arr.begin() + right + 1);
    logThreadFinish(threadID, sublist);
}

// Function to print a vector
void printVector(vector<int>& arr) {
    for (int i = 0; i < arr.size(); i++) cout << arr[i] << " ";
    cout << endl;
}



int main() {
    cout << "Program Started..." << endl; // Add this line

    vector<int> arr;
    arr = { 3304, 8221, 26849, 14038, 1509, 6367, 7856, 21362 };

    cout << "Given vector is:\n";
    printVector(arr);
    
    mergeSort(arr, 0, arr.size() - 1, 1);
    
    cout << "\nSorted vector is:\n";
    printVector(arr);
    return 0;

}