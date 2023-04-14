#include <algorithm>
#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <thread>
#include <mutex>

#define NUM_THREADS 8
#define NUM_HOURS 12

std::mutex mtx; 
using namespace std; 

int randomNumber()
{
	std::random_device seed;
    std::mt19937 rng(seed());
    std::uniform_int_distribution<std::mt19937::result_type> dist1(-100, 70);	

    return dist1(rng);
}

void calculateHighestTemperatures(int hour, vector<int>& data)
{
	cout<< "Largest Temperatures: "<<endl;

	vector<int> final_five;

	sort(data.begin(),data.end());
	for(auto it = data.rbegin(); it!= data.rend(); it++)
	{
		final_five.push_back(*it);

		if(final_five.size() ==5)
			break;
	}

	for (int temp: final_five)
	{
		cout<< temp <<" "; 
	}
	cout<<endl;
}

bool checkRaceConditionSatisfied(int thread, vector<int>& calibrateSensors)
{
	for(int i = 0; i<calibrateSensors.size(); i++)
	{
		if(calibrateSensors[i] == 0 && thread != i)
			return false;
	}

	return true; 
}

void calculateLowestTemperatures(int hour, vector<int>& data)
{
	cout<< "Lowest Temperatures: "<<endl;

	vector<int> final_five;

	sort(data.begin(),data.end());
	for(auto it = data.begin(); it!= data.end(); it++)
	{
		final_five.push_back(*it);

		if(final_five.size() ==5)
			break;
	}

	for (int temp: final_five)
	{
		cout<< temp <<" "; 
	}
	cout<<endl;
}


void calculateLargestDifference(int hour, vector<int>& data)
{
	int max = -1e6;
	int start = -1; 
	int end = start+10; 	

	for(int i = 0; i<NUM_THREADS; i++)
	{
		int buffer = i*60;

		for(int j = buffer; j < 60*9; j++)
		{
			int maxValinTenMinute = *std::max_element(data.begin()+j,data.begin()+j+10);
			int minValinTenMinute = *std::min_element(data.begin()+j,data.begin()+j+10);
			int delta = maxValinTenMinute-minValinTenMinute;

			//cout<<delta<<endl;

			if(delta>max && delta<170)
			{
				max = delta; 
				start = j;
				end = j+10; 
			}
		}
	}

	cout<<"Hour: "<<hour<<endl; 
	cout<<"Biggest temperature difference: "<<max<<"F."<<endl;
	cout<<"Start: "<<start<<endl;
	cout<<"End: "<<end<<endl;
}
void dataCollection(vector<int>& data,vector<int>& calibrateSensors, int threadNum)
{
	for(int i = 0; i <NUM_HOURS; i++)
	{
		for(int j = 0; j<60; j++)
		{
			calibrateSensors[threadNum] = 0; 
		

			data[(threadNum*60) + j] = randomNumber();
			calibrateSensors[threadNum] = 1; 	 	
			// cout<<data[(threadNum*60) + j]<<endl;

			while(!checkRaceConditionSatisfied(threadNum, calibrateSensors))
			{
				this_thread::sleep_for(std::chrono::milliseconds(40));
			}
			
		}
		
		if(threadNum==1)
		{
			mtx.lock();
			calculateLargestDifference(i, data);
			calculateHighestTemperatures(i, data);
			calculateLowestTemperatures(i, data);
			mtx.unlock();
		}
	}
}

int main()
{		

	vector<int> calibrateSensors(NUM_THREADS);
	vector<int> data(NUM_THREADS*60);
	
	std::thread threads[NUM_THREADS]; 


	for(int i = 0; i < NUM_THREADS; i++)
	{
		threads[i] = std::thread(dataCollection, ref(data), ref(calibrateSensors), i);
	    
	}	

	auto start = std::chrono::high_resolution_clock::now();
	for(thread& thread: threads)
	{
		thread.join();
	}
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);

	std::cout<<"----------------------------------------------------"<<std::endl;
	std::cout<<"Runtime: "<<duration.count()<<"ms"<<std::endl;
	cout<<endl;

	return 0; 
}