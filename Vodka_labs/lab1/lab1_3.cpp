#include <iostream>
#include <vector> 
#include <mutex> 
#include <thread>
#include <ctime>
#include <chrono>
 
using namespace std;

mutex mut;

class Philosopher;

int forks = 5;
int plates = 5;
int phils = 5;

int m_random(int x, int y) // [x, y]
{
	return rand() % (y-x+1) + x;
}

class Philosopher
{
	public:
		static int philosophers;
	
	protected:
		int num;
		string state;
		thread thr;

	public:
		Philosopher()
		{
			this->num = ++philosophers;
			this->setState(" thinking");
			this->thr = thread(th, this);
		}
		
		~Philosopher()
		{
			philosophers--;
			this->thr.join();
		}
	
		void setState(string state) {
			this->state = state;
			this->print(this->state);
		}
		
		void print(string text)
		{
			mut.lock();
			cout<<"Philosopher "<<this->num<<text<<endl;
			mut.unlock();
		}

	private:
		static void th(Philosopher *person) {
			while (true) {
				int time_eat = m_random(1, 10);
				if (forks >= 2) {
					forks -= 2;
					person->setState(" eating " + to_string(time_eat) + " secs");
					this_thread::sleep_for(chrono::seconds(time_eat));
					forks += 2;
					person->setState(" stopped eating");
					person->setState(" thinking");
				}
			}
			return;
		}
};
int Philosopher::philosophers = 0;

int main() 
{
	srand(time(NULL));
	for (int i = 0; i < phils; i++) {
		new Philosopher();
	}
	while (true){}
	return 0;
}

