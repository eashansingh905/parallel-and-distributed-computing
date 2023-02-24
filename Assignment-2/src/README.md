# Assignment 2

## Instructions: How to run the code
I have written my code in C++. Below are instructions on how to run my code from the Command-line(Windows).

problem1.cpp:

1) Go to the command-line and generate the executable: **g++ problem1.cpp -o problem1**
2) To run the executable simply type the name of the executable: **problem1**

problem2.cpp

1) Go to the command-line and generate the executable: **g++ problem2.cpp -o problem2**
2) To run the executable simply type the name of the executable: **problem2**

Example Usage:

C:\Users> **g++ problem1.cpp -o problem1**

C:\Users> **problem1**

# Problem 1: Minotaur's Birthday Party

## Problem 1: Task
For this task, N guests are invited over to the Minotaurs party. Guests may enter the labyrinth one at a time to eat a cupcake or leave it. The cupcake may or may not be on the plate when they enter so they may request the servant to bring a cupcake if they so wish. Guests may not communicate with each other about whether they had a cupcake but are allowed to come up with a strategy beforehand to tell the Minotaur that every guest has had a cupcake. You can assume that a cupcake has been left on the plate at the beginning of the party. 

## Problem 1: Algorithm & Parallelization Approach
In my solution, each guest is represented by a single thread for a total of N running thread. As discussed in class about the prisoner's light problem, the first guest to enter the labyrinth is a leader guest which keeps track of whether a cupcake has 
been eaten or not. Only the leader guest requests a new cupcake to be added on the plate. Each guest is to only eat 1 cupcake
and not request one to be replaced on the plate. If a guest has already visited the labyrinth and ate a cupcake, they do not eat another
cupcake. Every time the leader guest visits the labyrinth and find the plate to be empty they can assume 1 guest has ate a cupcake and
increase the counter. Once a total of N guests have been tallied for the threads are terminated and the program ends. 

This strategy ensures that every guest is accounted for and that mutex locks are used to make sure variables are not being modified by multiple threads at once. To decide the order by which a guest enters the labyrinth we create a random number generation(RNG) operation which decides which thread is allowed to enter the labyrinth. To implement the RNG, we make use of C++ std::uniform_int_distribution to make sure that roughly every thread has an equal probability of being called. A previous approach was tried using modulo operator however this RNG procedure was heavily biased towards low values and severely affected the runtime of the program. Due to the randomness involved in the implementation of the program, there can be variation in the runtime from run to run. For the leader thread to increase the counter of the number of eaten cupcakes, it takes N generations on average before it is able to do so.  


## Problem 1: Output

## Problem 1: Experimental Evaluation
| Guests(threads) | Execution Time   |
| --------------  | -----------------|
| 10              |       19ms       |
| 30              |       118ms      |
| 50              |       183ms      |
| 70              |       392ms      |
| 100             |       689ms      |

# Problem 2: Minotaur's Crystal Vase

## Problem 2: Task

## Problem 2: Algorithm & Parallelization Approach

## Problem 2: Output

## Problem 2: Experimental Evaluation
| Guests(threads) | Execution Time   |
| --------------  | -----------------|
| 10              |       0.55s      |
| 30              |       1.58s      |
| 50              |       3.87s      |
| 70              |       5.80s      |
| 100             |       9.42s      |
