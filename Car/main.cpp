#include <iostream>
using namespace std;

#define MIN_TANK_CAPACITY	20
#define MAX_TANK_CAPACITY	120

class Tank
{
	double fuel_level;

public:
	const int CAPACITY;
	double get_fuel_level()const
	{
		return fuel_level;
	}
	Tank(int capacity) :CAPACITY
	(
		capacity < MIN_TANK_CAPACITY ? MIN_TANK_CAPACITY :
		capacity > MAX_TANK_CAPACITY ? MAX_TANK_CAPACITY :
		capacity
	),
		fuel_level(0)
	{
		cout << "Tank:" << endl;
		if (capacity < MIN_TANK_CAPACITY)cout << "Min capacity was applied" << endl;
		if (capacity > MAX_TANK_CAPACITY)cout << "Max capacity was applied" << endl;
		cout << "Tank is ready" << endl;
	}
	~Tank()
	{
		cout << "Tank is over" << endl;
	}

	double fill(double amount)
	{
		if (amount < 0) return fuel_level;
		fuel_level += amount;
		if (fuel_level > CAPACITY)
		{
			fuel_level = CAPACITY;
			cout << "Full tank" << endl;
		}
		return fuel_level;
	}

	double give_fuel(double amount)
	{
		fuel_level -= amount;
		if (fuel_level < 0) fuel_level = 0;
		return fuel_level;
	}

	void info()const
	{
		cout << "Capacity:\t" << CAPACITY << " liters" << endl;
		cout << "Fuel level:\t" << fuel_level << " liters" << endl;
	}
};

class Engine
{
	int fuel_consumption_per_hundred;
public:
	Engine(int fuel_cons_per_hundred)
	{
		cout << "Engine:" << endl;
		this->fuel_consumption_per_hundred = fuel_cons_per_hundred;
		cout << "Engine is ready" << endl;
	}
	~Engine()
	{
		cout << "Engine is over" << endl;
	}

	double fuel_cons_per_sec(int speed)
	{
		if (speed == 0) return 0.0003;
		else
		{
			return((double)(fuel_consumption_per_hundred * speed) / 360000);
		}
	}
};

void main()
{
	setlocale(LC_ALL, "");

	Engine engine(10);
	int speed;
	cout << "С какой скоростью едем? "; cin >> speed;
	cout << "Расход топлива - " << engine.fuel_cons_per_sec(speed) << " л/с" << endl;

	Tank tank(80);
	tank.info();
	double fuel;
	do
	{
		cout << "На сколько заправимся? "; cin >> fuel;
		tank.fill(fuel);
		tank.info();
	} while (true);


}