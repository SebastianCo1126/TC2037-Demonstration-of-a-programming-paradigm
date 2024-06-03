// =========================================================
// File: zeroEvenOdd.cpp
// Author: Sebastián Colín
// Date: 02/06/2024
// Description: This file proposes a solution to the 
// zero even odd problem, using threads and mutex locks. 
// =========================================================

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <sstream>
#include<array> 

using namespace std;

// Struct that holds state temp information before printing
struct stateCheck {
   
    int n;  // max number to be printed 

    int current; // the current number to be printed

    bool zeroTurn;  // indicates when it's the turn for a zero to be printed

    mutex m1; // mutex lock to protect data that is shared across threads

    condition_variable cv; // conditional variable to ensure thread synchronization

    stringstream check; // adds all numbers to a stringstream for answer comparison

};

/**
 * Determines if it's the turn of a zero
 * to be printed
 * 
 * @param state: a state object (struct) with values of curr state, function: to print numbers
 * @return n/A
*/
void zero(stateCheck* state, function<void(int)> printNumber) {
    for (int i = 0; i < state->n; ++i) {
        unique_lock<mutex> lock(state->m1);
        state->cv.wait(lock, [&state] { return state->zeroTurn; });
        printNumber(0);
        state->check << 0;
        state->zeroTurn = false;
        state->cv.notify_all();
    }
}

/**
 * Determines if it's the turn of an even number
 * to be printed
 * 
 * @param state: a state object (struct) with values of curr state, function: to print numbers
 * @return n/A
*/
void even(stateCheck* state, function<void(int)> printNumber) {
    for (int i = 2; i <= state->n; i += 2) {
        unique_lock<mutex> lock(state->m1);
        state->cv.wait(lock, [&state] { return !state->zeroTurn && state->current % 2 == 0; });
        state->check << state->current;
        printNumber(state->current++);
        state->zeroTurn = true;
        state->cv.notify_all();
    }
}

/**
 * Determines if it's the turn of an odd number
 * to be printed
 * 
 * @param state: a state object (struct) with values of curr state, function: to print numbers
 * @return n/A
*/
void odd(stateCheck* state, function<void(int)> printNumber) {
    for (int i = 1; i <= state->n; i += 2) {
        unique_lock<mutex> lock(state->m1);
        state->cv.wait(lock, [&state] { return !state->zeroTurn && state->current % 2 == 1; });
        state->check << state->current;
        printNumber(state->current++);
        state->zeroTurn = true;
        state->cv.notify_all();
    }
}

/**
 * Helper function to print numbers
 * 
 * @param int: the current number
 * @return n/A
*/
void printNumber(int x) {
    cout << x;
}


/**
 * main function to run the program
 * 
 * @param n/A
 * @return n/A
*/
int main() {

    // Array for test values
    int arr[] = {5, 10, 15, 20, 30};

    // Test answers
    string ans1 = "0102030405";
    string ans2 = "010203040506070809010";
    string ans3 = "010203040506070809010011012013014015";
    string ans4 = "010203040506070809010011012013014015016017018019020";
    string ans5 = "010203040506070809010011012013014015016017018019020021022023024025026027028029030";

    string stringArr[] = {ans1, ans2, ans3, ans4, ans5};

    for (int i = 0; i < sizeof(arr)/sizeof(arr[0]); i++) {

        cout << "Initiating test " << (i + 1) << ":\n";

        stateCheck state = {arr[i], 1, true};   // Initialize the shared state

        thread t1(zero, &state, printNumber);
        thread t2(even, &state, printNumber);
        thread t3(odd, &state, printNumber);

        t1.join();
        t2.join();
        t3.join();

        cout << "\n\n";

        if (state.check.str() == stringArr[i]) {
            cout << "Test " << i + 1 << " successful\n\n\n";
        } else {    
            cout << "Prueba " << i + 1 << " NOT successful\n\n\n";

        }

    }

    return 0;

}
