# Assignment 1: Primes


## Instructions: How to run the code
I have written my code in C++. Below are instructions on how to run my code from the Command-line(Windows).


1) Go to the command-line and generate the executable: **g++ primes.cpp -o primes**
2) To run the executable simply type the name of the executable: **primes**

Usage:

C:\Users> g++ primes.cpp -o primes   
C:\Users> primes

The **primes.txt** file will be generated in the current working directory(directory with the **primes.cpp** file).

## Task

The task of this problem is to find Primes from 2 to 100,000,000. Specifically, we want to parallelize the load across several cors 
to improve our performance time. In this project, we use 8 threads to optimize our run-time. 

##  Algorithm & Parallelization Approach

To identify all the primes up until 100,000,000 we can utilize the Sieves of Eratosthenes algorithm. Unlike a traditional approach of manually
checking each number in the range for primality, we are essentially identifying all "non-prime" numbers and marking them as such. We start by identifying
some initial prime number *n* and marking all multiples of *n* as non-prime. The first prime number we identify is 2, so we go ahead and mark 4, 6, 8, 10, 12,.. as non-prime in our boolean array. The next prime would be 3, and we mark 6, 9, 12, 15, 18,... as non-prime. We repeat this process with all identified primes up until the square-root of 10^8. Once we are done, we can iterate our boolean array to check which values have not been marked as "not-prime" to get our final list of prime numbers.

This approach can be parallelized by load and over 8 threads for faster computation. Instead of a sequential calculations of non-primes, we can distribute the calcuations over 8 threads. One technique can be designating a thread to a certain input range. For example thread 1 gets input from 2 to 10^8, thread 2 gets input from 10^8 + 1 to 2*10^8, thread 3: 2*10^8 + 1 to 3*10^8. However some analysis of this approach tells us that the work per thread will be **distributed unevenly**.This is because primes are more heavily concentrated in smaller input ranges than larger ones. Thread 1 and 2 may be doing almost all the work compared to the others and this may be **inefficient**. 

**Better approach**: A better approach to equal division of work is to have threads work concurrently in equal intervals over the range of 1 to square root 10^8. Specifically, by this I mean that if a given thread is working on a value **a**, having that thread check for prime multiple values of a+threadCount, a + 2* threadCount, a + 3* threadcount, where threadcount = 8. So if the first thread is working on 2, after it has eliminated all multiples, it will go to 2+8 = 10. Because 10 will have been marked as non-prime, it will jump to 10+8 = 18. The thread will continue to jump and eliminate multiples that have not been marked. Thread two would start at 3 and mark 11, 19, 27, etc. We are essentially cutting our input into 8 seperate pieces by having threads increment by the threadCount and check for multiples of every 8th number. The distribution of work amongst the threads will not be exactly equal since a previous thread may mark a non-prime before another