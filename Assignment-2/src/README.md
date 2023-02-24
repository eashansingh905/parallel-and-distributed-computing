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
The output of this program is as such:

**Every visit to the labyrinth is printed.**

1) If the leader visits the labyrinth: "Guest #1(leader) has entered the labyrinth"
2) If a guest X visits the labyrinth: "Guest #X has entered the labyrinth"

3) After all N guests have visited the labyrinth, a message is printed notifying us along with the runtime.

All X guests have had a cupcake!
Runtime: ..ms

## Problem 1: Experimental Evaluation
| Guests(threads) | Execution Time   |
| --------------  | -----------------|
| 10              |       19ms       |
| 30              |       118ms      |
| 50              |       183ms      |
| 70              |       392ms      |
| 100             |       689ms      |

Listed are the runtimes for varying values of N. The listed execution times were taken on average of 10 runs. Because of the varying nature of the RNG, we can expect to see more variation in results since some runs may
be more "lucky" than others by getting the leader thread to quickly update the counter and unblock other threads
from eating a cupcake. On average, it takes the counter thread N number of random number generations to finally
re-enter the labyrinth and add a new cupcake. As you can see, as the number of running threads increased, so 
did our runtime. This is expected because as we add more guests that need to visit the labyrinth, there is less
opportunity for a given thread to be randomly selected to enter. The increase in this runtime will be nonlinear since every marginal running thread will also need to be accounted for by the leader thread. 


# Problem 2: Minotaur's Crystal Vase

## Problem 2: Task
For this task, the Minotaur would like to show N guests his Crystal vase. A given guest can only visit the room with the vase one at a time. Three strategies are presented and we are asked to pick one and discuss the advantages and disadvantages. Here are the three strategies: 

1) Any guest could stop by and check whether the showroom’s door is open at any time 
and try to enter the room. While this would allow the guests to roam around the castle 
and enjoy the party, this strategy may also cause large crowds of eager guests to gather 
around the door. A particular guest wanting to see the vase would also have no 
guarantee that she or he will be able to do so and when. 

2) The Minotaur’s second strategy allowed the guests to place a sign on the door 
indicating when the showroom is available. The sign would read “AVAILABLE” or 
“BUSY.” Every guest is responsible to set the sign to “BUSY” when entering the 
showroom and back to “AVAILABLE” upon exit. That way guests would not bother trying 
to go to the showroom if it is not available. 

3) The third strategy would allow the quests to line in a queue. Every guest exiting the 
room was responsible to notify the guest standing in front of the queue that the 
showroom is available. Guests were allowed to queue multiple times. 


## Problem 2: Algorithm & Parallelization Approach
For this assignment I decided to implement approach 2. 

**Approach 2:** 
The Minotaur’s second strategy allowed the guests to place a sign on the door 
indicating when the showroom is available. The sign would read “AVAILABLE” or 
“BUSY.” Every guest is responsible to set the sign to “BUSY” when entering the 
showroom and back to “AVAILABLE” upon exit. That way guests would not bother trying 
to go to the showroom if it is not available. 

**Advantages of this approach:**
Unlike approach 1 where guests will have to hope that the room with the vase is empty, in this approach, 
it is made very clear whether a guest is able to view the room based off the sign on the door "AVAILABLE"
or "BUSY". Thus, large crowds of people will not have to wait constantly to see if the room is empty. A quick
indication will allow them to continue looking around in case it is busy. In approach 3, we have to make an entire
line or queue of people that will be required to wait before they can walk inside. This will consume them from doing
other activities at the party and will block. This can to some extent be viewed as inefficient. Approach 2 finds a middle ground where threads will not have to constantly check whether resources are available(approach 1) and being blocked completely from other tasks while they wait(approach 3). The status of the availability of a room can be managed with a simple boolean variable which marks whether someone can enter the room. An O(1) check is sufficient and allows threads to keep moving. 


**Disadvantages of this approach:**
Compared to approach 1, one obvious disadvantage this approach poses is the possibility of unnecessary overhead incase the room is empty. For small values of N, it may not be necessary for the room to be marked as busy or available since there are few guests and their probability of overlapping and competing for resources will be smaller. It is very plausible that every guest can visit the room at a different time and naturally never need to wait. Approach 3 fairs better for larger values of N, where several threads are competing for the same resource and several guests may end up checking the status of the room at the same time with approach 2. While approach 3 does require management of a queue structure, it is organized, and every member of the party will be informed of their place in line. People who do not wish to wait can continue exploring the party while people who are more interested in the vase can join the line, knowing very well their position of seeing the vase and not having to rely on random chance.



## Problem 2: Output
The output of this program is as such:

**The order in which every guest visits the vase is printed**

Once all guests have visited the vase, the program terminates and reports that all guests have visited the vase along with the runtime of the program. 

## Problem 2: Experimental Evaluation
| Guests(threads) | Execution Time   |
| --------------  | -----------------|
| 10              |       0.55s      |
| 30              |       1.58s      |
| 50              |       3.87s      |
| 70              |       5.80s      |
| 100             |       9.42s      |

Like problem 1, we use uniform random distribution to select a guest to visit the vase. A guest may visit the vase several times if they wish. However unlike problem 1, to better simulate the conditions of problem 2, we allow the guest to stay in the room for a random duration of time. We thus have a second RNG for "duration of stay" that lets the guest stay in the room for some random amount of time. This allows us to make sure that while a guest X is in the room for some amount of milliseconds, another guest Y cannot enter due to the "busy" sign we have on the door. With problem 1, we assume the guest is in the labyrinth purely to eat a cookie and leave. Like problem 1, the runtime increases exponentially since every additional guest may visit one or more time(s) for some non-constant amount of time. Because of the added time a guest stays in the room, we have increased the runtime significantly.
