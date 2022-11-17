#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <thread>

using namespace std;

int data = 0;

void th_1()
{
	while (data != 1000) {
		data++;
		cout<<"Thread 1:"<<data<<endl;
	}
	terminate();
	return;
}

void th_2()
{
	while (data != -1000) {
		data--;
		cout<<"Thread 2: "<<data<<endl;
	}
	terminate();
	return;
}

int main()
{
	thread th1(th_1);
	thread th2(th_2);
	th1.join();
	th2.join();
	return 0;
}

