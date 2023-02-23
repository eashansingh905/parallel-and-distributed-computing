#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>

#define GUESTS 100

int currentThread; // current guest(thread) in labyrinth
int cupcakeCounter = 0; // number of cupcakes eaten

bool isThereACupcake = true; // returns true if a cupcake is left on a plate
bool guestVisited[GUESTS] = {false}; // tracks which guests have eaten a cupcake

std::mutex labyrinth;

void party(int thread)
{
	// while guests have not eaten cupcakes
	while (cupcakeCounter < GUESTS)
	{
		labyrinth.lock();
		
		// Leader guest(a.k.a. counter thread)
		if (currentThread == 0)
		{
			std::cout << "Guest #1(leader) has entered the labyrinth"<<std::endl;
			if (!isThereACupcake)
			{
				cupcakeCounter++;
				isThereACupcake = true;
			}

			// makes sure the leader guest eats a cupcake 
			if (!guestVisited[0] && isThereACupcake)
			{
				cupcakeCounter++;
				isThereACupcake = true;
				guestVisited[0] = true; 
			}
		}

		// A regular guest 
		else
		{
			// Checks to see if a cupcake is available, their active thread is running, and 
			// they have not already eaten a cupcake
			if (currentThread == thread)
			{
				std::cout << "Guest #"<<(thread+1)<<" has entered the labyrinth"<<std::endl;
				if(isThereACupcake && !guestVisited[thread])
				{
					isThereACupcake = false;
					guestVisited[thread] = true;
				}
			}
		}

		labyrinth.unlock();
	}
}

int main()
{
	std::thread threads[GUESTS];

	auto start = std::chrono::high_resolution_clock::now();

	// Create N guests(threads)
	for (int i = 0; i < GUESTS; i++)
	{
		threads[i] = std::thread(party, i);
	}

	// While we have people who have not eaten cupcakes, allow guests to enter the labyrinth
	// by random selection
	while (cupcakeCounter < GUESTS)
	{
		std::random_device seed;
    	std::mt19937 rng(seed());
    	std::uniform_int_distribution<std::mt19937::result_type> uniform_dist(0, GUESTS);
    	currentThread = uniform_dist(rng);
	}

	for (auto &x : threads)
	{
		x.join();
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);

	std::cout<<"----------------------------------------------------"<<std::endl;
	std::cout<<"All " << GUESTS	<< " guests have had a cupcake!"<<std::endl;
	std::cout<<"Runtime: "<<duration.count()<<"ms"<<std::endl;
	return 0; 
}