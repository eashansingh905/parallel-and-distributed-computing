#include <algorithm>
#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <thread>
#include <mutex>

#define NUM_THREADS 4
#define NUM_GUESTS 500

using namespace std;
std::mutex mtx;

vector<int> bagOfPresents = {};
vector<int> presentsInChain = {};
vector<int> cardsWritten = {};

struct Node
{
	int presentID; 
	struct Node *next;
};


void insertPresent(struct Node**head, int presentID)
{
	// mtx.lock();

	struct Node* newPresent = new Node;
	newPresent->presentID = presentID; 

	// if head is null
	if(*head == NULL)
	{
		newPresent->next = NULL;
		*head = newPresent;
		// mtx.unlock();
		return; 
	}
	else
	{
		struct Node* temp = *head;

		while((temp->next != NULL) && (temp->next->presentID < presentID))
		{
			temp = temp->next; 
		}

		if(temp->next == NULL) // next element is empty
		{
			if(temp->presentID < presentID) // insert after
			{
				temp->next = newPresent; 
				newPresent->next = NULL;
				// mtx.unlock(); 
				return; 
			}
			else  // insert before (at head)
			{
				newPresent->next = temp;
				*head = newPresent;
				// mtx.unlock();
				return;
			}
		}
		else if(temp == *head) // if we are inserting near head
		{
			if(temp->presentID > presentID) // before head
			{
				newPresent->next = temp;
				*head = newPresent;
				// mtx.unlock();
				return;
			}
			else // after head
			{
				newPresent->next = temp->next;
				temp->next = newPresent;
				// mtx.unlock();
				return; 
			}
			
		}
		else // in between 2 elements
		{
			newPresent->next = temp->next;
			temp->next = newPresent;
			// mtx.unlock(); 
			return;
		}
	}

	//mtx.unlock();

}

Node* deletePresent(struct Node* head, int presentID)
{
	//mtx.lock();
	if(head == NULL) // empty
	{
		cout <<"No presents left to remove(delete)."<<endl;
		//mtx.unlock();
		return NULL; 
	}

	struct Node* temp = head;

	if((temp->presentID == presentID) && (temp == head)) // delete head
	{
		if(temp->next == NULL) // only element is head
		{
		//	mtx.unlock();
			return NULL;
		}
		else // point head to next element
		{
			head = head->next;
		//	mtx.unlock();
			return head;
		}
	}
	else // delete a non head element
	{
		while(temp->next->presentID != presentID)
		{
			temp = temp->next; 
		}

		if(temp->next->next == NULL) // if we are deleting the last element
		{
			temp->next = NULL;
		//	mtx.unlock(); 
			return head;
		}
		else
		{
			Node* newLink = temp->next->next; // middle element 
			temp->next = newLink;
		//	mtx.unlock();
			return head; 
		}
	}
//	mtx.unlock();
	return head;
}

void presentInChain(struct Node* head, int presentID)
{
	 //mtx.lock();
	struct Node* temp = head;

	while(temp != NULL)
	{
		if(temp->presentID == presentID)
		{
			cout<<"Gift #"<<presentID<<" is in the chain!"<<endl;
			 mtx.unlock();
			return; 
		}

		temp = temp->next; 
	}

	cout<<"Gift #"<<presentID<<" is not in the chain!"<<endl;
	 //mtx.unlock(); 
}

void display(struct Node *node)
{
	while (node != NULL){
		cout<<node->presentID<<"-->"; 
		node = node->next; 
	}
	cout<<endl;
}

//std::vector<int>& bagOfPresents, std::vector<int>& presentsInChain, std::vector<int>& cardsWritten, struct Node* head, int threadNumber
void performTasks(struct Node* head, int threadNumber)
{
	// cout<<presentsInChain.size()<<endl;

	while(cardsWritten.size() < NUM_GUESTS && presentsInChain.size() < NUM_GUESTS)
	{
		std::random_device seed;
        std::mt19937 rng(seed());
        std::uniform_int_distribution<std::mt19937::result_type> dist1(1,3);

		int job = dist1(rng);

		// cout<<"JOB NUMBER: "<<job<<", Thread number: "<<threadNumber<<endl;
		switch (job){


			case 1: { // Add a present to the chain and remove from bagOfPresents
				//mtx.lock();
			
				if(bagOfPresents.size() != 0)
				{		
						
						int present = bagOfPresents.back();
						//cout<<present;
						bagOfPresents.pop_back();
						presentsInChain.push_back(present);
						cout<<"Adding present #"<<present<<" to the chain. Thread: "<<threadNumber<<endl;
						//mtx.lock();
						insertPresent(&head, present);
						//mtx.unlock();
	
						// cout<<"Adding: "<<present<<", Thread Number: "<<threadNumber<<endl;

				}
				//mtx.unlock();
				break;
			}

			case 2: { // Write a thank you card to a guest, and remove present from the chain 
	
				// if(presentsInChain.size() != 0)
				// {
				// 		//mtx.lock();
				// 		int removedPresent = presentsInChain.back();
				// 		presentsInChain.pop_back();
				// 		cardsWritten.push_back(removedPresent);	
				// 		cout<<"Card written to #"<<removedPresent<<", Thread: "<<threadNumber<<endl;
				// 		mtx.lock();
				// 		deletePresent(head, removedPresent);
				// 		mtx.unlock();
						
				// }


				break;
			}
			case 3: {
				
				std::uniform_int_distribution<std::mt19937::result_type> dist2(1,NUM_GUESTS);
				int giftToCheck	= dist2(rng);
				if(presentsInChain.size() != 0)
				{
					// mtx.lock();
					presentInChain(head, giftToCheck);
					// mtx.unlock();
				}
				else
				{
					cout<<"Gift #"<<giftToCheck<<" is not in the chain!"<<endl;
				}

				break;
			}
			default : { // Do nothing

			}
		

		}
	}
	
}

int main()
{
	
  
	// Create an unordered bag of presents
	
	for(int i = 1; i<= NUM_GUESTS; i++)
	{
		bagOfPresents.push_back(i);
	}


	// Randomize ordering 
	auto rng_engine = default_random_engine {};
	shuffle(bagOfPresents.begin(),bagOfPresents.end(), rng_engine);

	// Array of threads(servants)
	thread threads[NUM_THREADS];
	struct Node* head = NULL;
 	
	for(int i = 1; i <= NUM_THREADS; i++)
	{
		// std::ref(bagOfPresents), std::ref(presentsInChain), std::ref(cardsWritten)
		threads[i-1] = thread(performTasks, std::ref(head), i);
	}
	auto start = std::chrono::high_resolution_clock::now();

	for (thread& thread : threads)
	{
		thread.join(); 
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);

	std::cout<<"----------------------------------------------------"<<std::endl;
	std::cout<<"All " << NUM_GUESTS	<< " guests have got a present!"<<std::endl;
	std::cout<<"Runtime: "<<duration.count()<<"ms"<<std::endl;
	cout<<endl;
	std::cout<<"(You can increase the number of guests by modifying the NUM_GUESTS var.)"<<endl;
	return 0; 
}