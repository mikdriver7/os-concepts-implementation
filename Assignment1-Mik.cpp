/*
COEN 346
Lab Assignment #1
Merge-Sort using multithreading
Mik Driver (40244456)

Merge sort reference: https://www.geeksforgeeks.org/merge-sort/

Terminal Inputs to run: 
Compile: g++ -std=c++11 -pthread Assignment1-Mik.cpp -o Assignment1-Mik 
Run: ./Assignment1-Mik
*/


#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <fstream>


using namespace std;

// Global Variables
mutex mtx;
ofstream outputFile("output.txt");

void logThreadStart(int threadID) {
    lock_guard<mutex> lock(mtx);
    outputFile << "Thread " << threadID << " started" << endl;
    outputFile.flush();
}

void logThreadFinish(int threadID, vector<int>& sublist) {
    lock_guard<mutex> lock(mtx);
    outputFile << "Thread " << threadID << " finished: ";
    for (int num : sublist) outputFile << num << " ";
    outputFile << endl;
    outputFile.flush();
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
    logThreadStart(threadID);

    // Base case: If the sublist has only one element, log its completion
    if (left == right) {
        vector<int> singleElement = {arr[left]};
        logThreadFinish(threadID, singleElement);
        return;
    }

    int mid = left + (right - left) / 2;
    int leftThreadID = threadID * 10;
    int rightThreadID = threadID * 10 + 1;

    // Create new threads for left and right halves
    thread leftThread(mergeSort, ref(arr), left, mid, leftThreadID);
    thread rightThread(mergeSort, ref(arr), mid + 1, right, rightThreadID);

    // Wait for both halves to finish
    leftThread.join();
    rightThread.join();

    // Merge the sorted halves
    merge(arr, left, mid, right);

    // Log merged result
    vector<int> sublist(arr.begin() + left, arr.begin() + right + 1);
    logThreadFinish(threadID, sublist);
}

// Function to print a vector
void printVector(vector<int>& arr, ostream& out = cout) {
    for (int i = 0; i < arr.size(); i++) out << arr[i] << " ";
    cout << endl;
}



int main() {
     cout << "Program Started..." << endl;

    if (!outputFile) {
        cerr << "Error: Unable to open output.txt" << endl;
        return 1;
    }

    vector<int> arr;
    ifstream inputFile("input.txt");
    if (!inputFile) {
        cerr << "Error: Unable to open input.txt" << endl;
        return 1;
    }
   
    int num;
    while (inputFile >> num) {
        arr.push_back(num);
    }
    inputFile.close();

    outputFile << "Given vector is:\n";
    printVector(arr, outputFile);
    outputFile << "\n\n";

    
    mergeSort(arr, 0, arr.size() - 1, 1);
    
    outputFile << "\n\nSorted vector is:\n";
    printVector(arr, outputFile);
    
    outputFile.close();
    cout << "Program Finished..." << endl;
    
    return 0;

}