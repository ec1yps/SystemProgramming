#include <iostream>
#include <thread>
using std::cin;
using std::cout;
using std::endl;

bool finish = false;

void Plus();
void Minus();

void main()
{
	setlocale(LC_ALL, "");
	//Plus();
	//Minus();
	std::thread plus_thread(Plus);
	std::thread minus_thread(Minus);

	cin.get();
	finish = true;

	if(minus_thread.joinable())minus_thread.join();
	if(plus_thread.joinable())plus_thread.join();
	if(plus_thread.joinable())plus_thread.join();
	if(plus_thread.joinable())plus_thread.join();
	if(plus_thread.joinable())plus_thread.join();
	if(plus_thread.joinable())plus_thread.join();

}

void Plus()
{
	while (!finish)
	{
		cout << "+ ";
	}
}

void Minus()
{
	while (!finish)
	{
		cout << "- ";
	}
}