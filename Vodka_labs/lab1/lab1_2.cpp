#include <iostream>
#include <list>
#include <mutex> 
#include <thread>
#include <ctime>

using namespace std;

int n = 20; // length of list
list<int> x;
mutex mut;

void thread_func1() 
{
	srand(time(NULL));

	for (int i = 0; i < n; i++) {
		mut.lock();
		x.push_back(rand());
		cout<<"Pushed " + to_string(i) + " element"<<endl;
		mut.unlock();
	}
}

void thread_func2()
{
	int i = 0;
	auto it = x.begin();
	while (i < n) {
		mut.lock();
		if (it != x.end()) {
			cout<<"x["+to_string(i)+"] = "<<*it<<endl;
			i++;
		}
		it = next(x.begin(), i);
		mut.unlock();
	}
}

int main() 
{
	thread th1(thread_func1); 
	thread th2(thread_func2);

	th1.join();
	th2.join();

	return 0;
}

