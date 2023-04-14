# Assignment 3

## Instructions: How to run the code
I have written my code in C++. Below are instructions on how to run my code from the Command-line(Windows).
Code for problem 1 is in **presents.cpp**. Code for problem 2 is in **temperature.cpp**.

presents.cpp:

1) Go to the command-line and generate the executable: **g++ presents.cpp -o presents**
2) To run the executable simply type the name of the executable: **presents**

temperature.cpp

1) Go to the command-line and generate the executable: **g++ temperature.cpp -o temperature**
2) To run the executable simply type the name of the executable: **temperature**

Example Usage:

C:\Users> **g++ presents.cpp -o presents**

C:\Users> **presents**

# Problem 1: 

## Problem 1: Task
The Minotaur’s birthday party was a success. The Minotaur received a lot of presents
from his guests. The next day he decided to sort all of his presents and start writing
“Thank you” cards. Every present had a tag with a unique number that was associated
with the guest who gave it. Initially all of the presents were thrown into a large bag with
no particular order. The Minotaur wanted to take the presents from this unordered bag
and create a chain of presents hooked to each other with special links (similar to storing
elements in a linked-list). In this chain (linked-list) all of the presents had to be ordered
according to their tag numbers in increasing order. The Minotaur asked 4 of his servants
to help him with creating the chain of presents and writing the cards to his guests. Each
servant would do one of three actions in no particular order: 1. Take a present from the
unordered bag and add it to the chain in the correct location by hooking it to the
predecessor’s link. The servant also had to make sure that the newly added present is
also linked with the next present in the chain. 2. Write a “Thank you” card to a guest and
remove the present from the chain. To do so, a servant had to unlink the gift from its
predecessor and make sure to connect the predecessor’s link with the next gift in the
chain. 3. Per the Minotaur’s request, check whether a gift with a particular tag was
present in the chain or not; without adding or removing a new gift, a servant would scan
through the chain and check whether a gift with a particular tag is already added to the
ordered chain of gifts or not. As the Minotaur was impatient to get this task done
quickly, he instructed his servants not to wait until all of the presents from the
unordered bag are placed in the chain of linked and ordered presents. Instead, every
servant was asked to alternate adding gifts to the ordered chain and writing “Thank you”
cards. The servants were asked not to stop or even take a break until the task of writing
cards to all of the Minotaur’s guests was complete. After spending an entire day on this
task the bag of unordered presents and the chain of ordered presents were both finally
empty! Unfortunately, the servants realized at the end of the day that they had more
presents than “Thank you” notes. What could have gone wrong? Can we help the
Minotaur and his servants improve their strategy for writing “Thank you” notes? Design
and implement a concurrent linked-list that can help the Minotaur’s 4 servants with this
task. In your test, simulate this concurrent “Thank you” card writing scenario by
dedicating 1 thread per servant and assuming that the Minotaur received 500,000
presents from his guests. 

## Problem 1: Algorithm & Parallelization Approach

For this problem, I first implemented the base Linked List code, and parallelized it accordingly to the race conditions. The insertion function
is activated when the case 1 is randomly generated and the present is added to the chain. Deletion takes place when the card is made for the 
individual. A simple O(n) traversal is made to check if a given present is in the chain or not. 


## Problem 1: Output
The output of problem two can be seen in the CMD. The output shows which guests have been added to chain, and random checks made by selection option 3
when the Minotaur requests the information of a given member. At the bottom, a summary of the runtime is provided. I tested for 500 individuals for 
ease of testing, but the number of presents can be changed to 500,000 like the problem statement by changing the NUM_GUESTS variable. 

## Problem 1: Experimental Evaluation
| Workers(threads) | Execution Time   |
| --------------  | -----------------|
| 1             |       390ms       |
| 4              |       419ms      |
| 8              |       422ms      |
| 12              |       400ms      |
| 16            |       450ms      |


Here is a sample run of the code at different number of threads on 500 individuals. Experimentation was done to see how the runtime is affected when 
there are more workers used to help delete the tasks. 


# Problem 2: 

## Problem 2: Task
You are tasked with the design of the module responsible for measuring the atmospheric
temperature of the next generation Mars Rover, equipped with a multicore CPU and 8
temperature sensors. The sensors are responsible for collecting temperature readings at
regular intervals and storing them in shared memory space. The atmospheric
temperature module has to compile a report at the end of every hour, comprising the top
5 highest temperatures recorded for that hour, the top 5 lowest temperatures recorded
for that hour, and the 10-minute interval of time when the largest temperature
difference was observed. The data storage and retrieval of the shared memory region
must be carefully handled, as we do not want to delay a sensor and miss the interval of
time when it is supposed to conduct temperature reading. Design and implement a
solution using 8 threads that will offer a solution for this task. Assume that the
temperature readings are taken every 1 minute. In your solution, simulate the operation
of the temperature reading sensor by generating a random number from -100F to 70F at
every reading. In your report, discuss the efficiency, correctness, and progress guarantee
of your program.


## Problem 2: Algorithm & Parallelization Approach

For this problem, the input size is divided up amongst NUM_THREADS, or in this case, 8 threads. The data collection phase collects all the values in a contiguous array
and is parsed appropriately for the number of hours of calculation. For my study, I have selected 12 hours for calcuation with data collections being done every 60 seconds
as per the problem requirements. Helper functions are made to check for race conditions such that all threads start computation after full collection of data is done. Helper
functions are also made for 4 calculations: the highest 5 temperatures, the lowest 5 temperatures, the biggest 10 minute delta in temperature over an hour, and an RNG function. 

## Problem 2: Output


## Problem 2: Experimental Evaluation
| Sensors(threads) | Execution Time   |
| --------------  | -----------------|
| 1              |       7ms      |
| 4             |       159ms      |
| 8             |       257ms      |
| 16              |       257ms      |
| 32             |       644s      |

Similar to problem 1, I ran some experiments to see how runtime varies by changing the number of used threads. The table above summarizes these results. Interestingly, the I/O overhead actually adds runtime because the calculations being done per thread
are actually not very computatonally expensive. 