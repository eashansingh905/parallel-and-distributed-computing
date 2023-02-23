#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>
#include <set>

#define GUESTS 30

bool roomAvailable = true; // sign on door marking room's availability
int visitingGuest; // current visiting guest
std::set<int> visited;  // set of guests who have visited the vase

std::mutex door;

// Selects a guest to that would like to visit the vase off uniform probability
int selectGuest()
{
	std::random_device seed;
    std::mt19937 rng(seed());
    std::uniform_int_distribution<std::mt19937::result_type> uniform_dist(0, GUESTS);
    return uniform_dist(rng);
}

// Generates a random duration of time for the guest to visit the vase
int durationOfVisit()
{
	std::random_device seed;
    std::mt19937 rng(seed());
    std::uniform_int_distribution<std::mt19937::result_type> uniform_dist(1, 5);
    return uniform_dist(rng);
}

void vase(int thread)
{
	while (visited.size() < GUESTS)
	{
		door.lock();

		// Checks if a guest can visit the room 
		if ((visitingGuest == thread) && roomAvailable)
		{
			// While the guest is visiting mark it BUSY
			roomAvailable = false; 

			// Random amount of time the guest visits
			std::this_thread::sleep_for(std::chrono::milliseconds(durationOfVisit()));
			
			// Once they leave mark it AVAILABLE
			roomAvailable = true;

			// If this is the first time the guest is visitng the vase
			if(visited.find(thread) == visited.end())
			{
				std::cout << "Guest #"<<(thread+1)<<" has visited the vase."<<std::endl;
				visited.insert(thread);
			}
		}
		door.unlock();
	}
}

int main()
{
	std::thread threads[GUESTS];
	auto start = std::chrono::high_resolution_clock::now();


	for (int i = 0; i < GUESTS; i++)
	{
		threads[i] = std::thread(vase, i);
	}

	// Run until all guests have visited the vase
	while (visited.size() < GUESTS)
	{
		visitingGuest = selectGuest(); 
	}

	for (auto &x: threads)
	{
		x.join();
	}


	std::cout<<"----------------------------------------------------"<<std::endl;
	std::cout<<"All " << GUESTS	<< " guests have visited the vase!"<<std::endl;
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
	std::cout<<"Runtime: "<<duration.count()<<"ms"<<std::endl;

	return 0;
}