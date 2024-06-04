# TC2037-Demonstration-of-a-programming-paradigm

## Zero Even Odd Problem 

### Description

My problem of choice states that we have a 'printNumber' function, that prints out the given 
number in the console. Also we have an instance of the class Zero Even Odd. In which we have 
a function zero, even and odd. Then three separate threds should be created, and each thread 
should manage a single function. 

- Thread A: zero function (only ouput **0's**)
- Thread B: even function (only output **even numbers**)
- Thread A: odd function (only output **odd numbers**)

The output should print all the numbers up to **n**, with zeroes in between them, so if
n = 5 the expected output should be: 0102030405, with a length of **2n**. The problem can 
be found in https://leetcode.com/problems/print-zero-even-odd/description/

I chose this problem and I think it is relevant because it requires the implementation of a solution 
that coordinates three separate threads to produce a specific sequence of outputs without race conditions or deadlocks.
Such synchronization challenges are highly relevant in real-world applications, where multiple threads or processes must 
work together to ensure data integrity and consistent program behavior. Solving this problem showcases an understanding 
of the concurrency paradigm, with concepts such as thread synchronization, mutual exclusion, and cross thread communication. 

This problem is particularly useful because it works as an educational tool that clearly demonstrates what concurrency is, 
and how crucial it is in efficient and modern software solutions. This paradigm allows programs to execute multiple operations
simultaneously, improving performance and responsiveness, useful for multi-core systems. Clearly understanding and implementing
solutions to this type of problems helps me develop critical skills common in the current industry. The aforementioned skills, 
are critical in high-performance applications such as web services or large-scale data processing. 

### Models

![zeroEvenOdd_Diagram](https://github.com/SebastianCo1126/TC2037-Demonstration-of-a-programming-paradigm/blob/main/zeroEvenOdd_Diagram.png)

### Implementation

For the implementation I decided to use C++, using the previous diagram as a starting point. The finished implementation 
was made without the use of a class for simplicity. The final file is zeroEvenOdd.cpp in which the implementation is made, 
in the main a series of tests are run. 

I created a struct object called "stateCheck" that hold information necessary to check the following number to print. It holds
the max number to be printed, the current number to be printed, if it is a zero's turn to be printed, a mutex lock, a conditional 
variable, and a stringstream to build a string of the numbers to check against the expected outputs. 

Then, there are three void functions called zero, even, odd, each handling the logic for each type of number to be printed. 

For this you must simply compile the .cpp file and run the .exe file. The main function initializes the stateCheck struct, 
with three parameters which are the max number to be printed, the current following number, and a boolean value set to true, 
for printing a zero first. 

### Test

The program runs over an integer array for different test cases, the outputs are then printed 
and converted to a string to compare it to the expected answers. If the tests are completed successfully, the program outputs
"Test (corresponding number of test) successful", if not the program outputs "Test (corresponding number of test) NOT successful".

The tests are contained in an array like the following: 

- int arr[] = {5, 10, 15, 20, 30}

And the expected outputs are:

- 0102030405
- 010203040506070809010
- 010203040506070809010011012013014015
- 010203040506070809010011012013014015016017018019020
- 010203040506070809010011012013014015016017018019020021022023024025026027028029030


![Diagrama en blanco - Página 4](https://github.com/SebastianCo1126/TC2037-Demonstration-of-a-programming-paradigm/assets/150994751/0cc665de-32a1-43ec-bf66-05e57a1b065b)

### Analysis

**Time Complexity**: 

To analyze the time complexity we need to evaluate the time complexities of the three separate functions zero, even and odd. 
The zero function runs n times, with all of the operations in between the loop running at O(1), even though the conditional
variable wait can be blocked until the condition is met, so it is more difficult to analyze in multi-thread contexts, but 
for the sake of this problem, it'll be considered as O(1). 

With this said, the functions evaluate to the following time complexities: 

- Zero function: runs `n` iterations with `O(1)` work per iteration: `O(n)`

```
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
```

- Even function: runs ≈ `n/2` iterations with `O(1)` work per iteration: `O(n/2) ≈ O(n)`

```
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
```

- Odd function: runs ≈ `n/2` iterations with `O(1)` work per iteration: `O(n/2) ≈ O(n)`

```
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
```

The combination of these, evaluates to: `O(n) + O(n) + O(n) = O(n)`. So the final time complexity is `O(n)`. 

**Other possible paradigms that could have been used**

The clear other paradigm that this could have been solved with is with imperative programming. Giving the 
computer detailed sequential steps on what to do to solve the problem. The advantage of this is that it is way 
less complex and has better code readability, but it isn't as fast as a concurrent program can be. 



