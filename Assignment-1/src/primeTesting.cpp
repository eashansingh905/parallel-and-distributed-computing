#include <iostream>
#include <fstream>
#include <algorithm>
#include <thread>
#include <chrono>
#include <vector>

using namespace std;
using namespace std::chrono;

# define NUM_THREADS 8
# define RANGE 100000001

bool primes[RANGE] = {false};

/*
	Modified Sieves of Eratosthenes algorithm which takes in
	a given start position "index" as a parameter and marks
	all multiples of that number until 10^8 as "true" in our
	primes boolean array.

	NOTE: primes[i] == TRUE means it is composite.
		  primes[i] == FALSE means it is prime.
	
	This is because of how we have initialized our boolean prime 
	array to default FALSE values.

	We have increments of size 8(NUM_THREADS), since we have 
	8 threads. This gives all threads an eigth of the work-load. 
*/
void findPrimes(int index)
{
	for (int i = index; i*i <= RANGE-1; i += NUM_THREADS)
	{
		if (primes[i] == false)
		{
			for (int j = i*i; j <= RANGE-1; j+=i)
			{
				primes[j] = true; 
			}
		}
	}
}

// Calculates the sum of primes in our range(10^8)
long long int primeSum()
{
	long long int sum = 0;
	for (int i = 2; i < RANGE; i++)
	{
		if (!primes[i])
		{
			sum+=i;
		}
	}

	return sum;
}

// Counts the number of primes in our range(10^8)
long primeCount()
{
	long counter = 0;
	for (int i = 2; i < RANGE; i++)
	{
		if(!primes[i])
		{
			counter+=1;
		}
	}

	return counter;
}

/* 
   Returns the ten biggest primes in our range(10^8)
   in increasing order
*/
vector<long long int> topTenPrimes()
{
	vector<long long int> v;

	for (int i = RANGE-1; i >= 0; i--)
	{
		if (!primes[i])
		{
			v.push_back(i);
		}

		if (v.size() == 10)
		{
			break;
		}
	}

	// Sort in ascending order 
	reverse(v.begin(), v.end());
	
	return v;
}

int main()
{
	thread threads[NUM_THREADS];
	
	auto start = high_resolution_clock::now();

	for (int i = 0; i < NUM_THREADS; i++)
	{
		// We start our index at 2 since 0 and 1 are not prime
		threads[i] = thread(findPrimes, i+2);
	}
	
	for (auto &x: threads)
	{
		x.join();
	}

	auto end = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(end-start);
	
	// File output to "primes.txt"
	ofstream file("primes.txt");
	
	file << duration.count() << " " << primeCount() << " " << primeSum() << endl;

	vector<long long int> tenLastPrimes = topTenPrimes();
	for (int i = 0; i < 10; i++)
	{
		file << tenLastPrimes[i] << " ";
	}

	file.close();

	return 0;
}