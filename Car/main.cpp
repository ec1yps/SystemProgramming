#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <thread>
using std::cin;
using std::cout;
using std::endl;

#define Enter	13
#define Escape	27

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

#define MIN_ENGINE_CONSUMPTION	3
#define MAX_ENGINE_CONSUMPTION	25


class Engine
{
	const double CONSUMPTION;
	const double DEFAULT_CONSUMPTION_PER_SECOND;
	double consumption_per_second;
	bool is_started;

public:
 
	double get_consumption_per_second()const
	{
		return consumption_per_second;
	}

	Engine(double consumption):CONSUMPTION
		(
			consumption < MIN_ENGINE_CONSUMPTION ? MIN_ENGINE_CONSUMPTION :
			consumption > MAX_ENGINE_CONSUMPTION ? MAX_ENGINE_CONSUMPTION :
			consumption
		),
		DEFAULT_CONSUMPTION_PER_SECOND(CONSUMPTION*3e-5),
		consumption_per_second(DEFAULT_CONSUMPTION_PER_SECOND)
	{
		is_started = false;
		cout << "Engine is ready" << endl;
	}
	~Engine()
	{
		cout << "Engine is over" << endl;
	}

	void start()
	{
		is_started = true;
	}
	void stop()
	{
		is_started = false;
	}
	bool started()const
	{
		return is_started;
	}
	void info()const
	{
		cout << "Consumption: " << CONSUMPTION << " liters/100km\n";
		cout << "Default consumption: " << DEFAULT_CONSUMPTION_PER_SECOND << " liters/s\n";
		cout << "Consumption: " << consumption_per_second << " liters/s\n";
	}
};

#define MAX_SPEED_LOWER_LIMIT	130
#define MAX_SPEED_HIGHER_LIMIT	408

class Car
{
	Engine engine;
	Tank tank;
	int speed;
	const int MAX_SPEED;
	bool driver_inside;

	struct
	{
		std::thread panel_thread;
	}threads_container;

public:
	Car(double consumption, int capacity, int max_speed = 250) :
		MAX_SPEED
		(
			max_speed < MAX_SPEED_LOWER_LIMIT ? MAX_SPEED_LOWER_LIMIT :
			max_speed > MAX_SPEED_HIGHER_LIMIT ? MAX_SPEED_HIGHER_LIMIT :
			max_speed
		),
		engine(consumption),
		tank(capacity),
		speed(0)
	{
		driver_inside = false;
		cout << "Your car is ready, press 'Enter' to get in" << endl;
	}
	~Car()
	{
		cout << "The car is over" << endl;
	}

	void get_in()
	{
		driver_inside = true;
		//panel();
		threads_container.panel_thread = std::thread(&Car::panel, this);
	}
	void get_out()
	{
		driver_inside = false;
		if (threads_container.panel_thread.joinable()) threads_container.panel_thread.join();
		system("cls");
		cout << "You are out of the Car" << endl;
	}

	void control()
	{
		char key = 0;
		do
		{
			key = _getch();
			switch (key)
			{
			case Enter:
				driver_inside ? get_out() : get_in(); 
				break;
			case 'F':case 'f':
				double fuel;
				cout << "Введите объем топлива: "; cin >> fuel;
				tank.fill(fuel);
				break;
			case 'P':case 'p':
				tank.get_fuel_level() != 0 && !engine.started() ? engine.start() : engine.stop();
				break;
			case Escape:
				get_out();
			}
			
		} while (key != Escape);
	}

	void panel()
	{
		while (driver_inside)
		{
			system("cls");
			cout << "Fuel level: " << (engine.started() ? tank.give_fuel(engine.get_consumption_per_second()) : tank.get_fuel_level()) << " liters\n";
			cout << "Engine is " << (engine.started() ? "started" : "stopped") << endl;
			cout << "Speed:\t" << speed << " km/h\n";
			Sleep(1000);
		}
	}

	void info()const
	{
		engine.info();
		tank.info();
		cout << "Speed: " << speed << "km/h\n";
		cout << "Max speed: " << MAX_SPEED << "km/h\n";
	}
};

//#define TANK_CHECK
//#define ENGINE_CHECK

void main()
{
	setlocale(LC_ALL, "");

#ifdef TANK_CHECK
	Tank tank(80);
	tank.info();
	double fuel;
	do
	{
		cout << "На сколько заправимся? "; cin >> fuel;
		tank.fill(fuel);
		tank.info();
	} while (true);
#endif // TANK_CHECK

#ifdef ENGINE_CHECK
	Engine engine(10);
	engine.info();
#endif // ENGINE_CHECK

	Car car(10, 80, 270);
	//car.info();
	car.control();
}