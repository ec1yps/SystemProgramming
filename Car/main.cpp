#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <thread>
using std::cin;
using std::cout;
using std::endl;
using namespace std::chrono_literals;

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
 

	double get_consumption_per_second(int speed = 0)const
	{
		
		if (speed == 0) return consumption_per_second;
		else if (speed <= 60) return .0020 * (CONSUMPTION / 10);
		else if (speed <= 100) return .0014 * (CONSUMPTION / 10);
		else if (speed <= 140) return .0020 * (CONSUMPTION / 10);
		else if (speed <= 200) return .0025 * (CONSUMPTION / 10);
		else if (speed <= 250) return .0030 * (CONSUMPTION / 10);
		else return .0035 * (CONSUMPTION / 10);
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
	int acceleration;
	const int MAX_SPEED;
	bool driver_inside;

	struct
	{
		std::thread panel_thread;
		std::thread engine_idle_thread;
		std::thread free_wheeling_thread;
	}threads_container;

public:
	Car(double consumption, int capacity, int max_speed = 250, int acceleration = 10) :
		MAX_SPEED
		(
			max_speed < MAX_SPEED_LOWER_LIMIT ? MAX_SPEED_LOWER_LIMIT :
			max_speed > MAX_SPEED_HIGHER_LIMIT ? MAX_SPEED_HIGHER_LIMIT :
			max_speed
		),
		engine(consumption),
		tank(capacity),
		speed(0),
		acceleration(acceleration)
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

	void start()
	{
		if (tank.get_fuel_level())
		{
			engine.start();
			threads_container.engine_idle_thread = std::thread(&Car::engine_idle, this);
		}
	}
	void stop()
	{
			engine.stop();
			if (threads_container.engine_idle_thread.joinable())
				threads_container.engine_idle_thread.join();
	}
	
	void accelerate()
	{
		if (driver_inside && engine.started())
		{
			speed += acceleration;
			if (!threads_container.free_wheeling_thread.joinable())
				threads_container.free_wheeling_thread = std::thread(&Car::free_wheeling, this);
			if (speed > MAX_SPEED) speed = MAX_SPEED;
			std::this_thread::sleep_for(1s);
		}
	}
	void slow_down()
	{
		if (driver_inside)
		{
			speed -= acceleration;
			if (speed < 0) speed = 0;
			std::this_thread::sleep_for(1s);
		}
	}

	void control()
	{
		char key = 0;
		do
		{
			key = 0;
			if (_kbhit()) key = _getch();
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
			case 'I': case 'i':
				if (driver_inside) !engine.started() ? start() : stop();
				break;
			case 'W':case 'w':
				accelerate();
				break;
			case 'S':case 's':
				slow_down();
				break;
			case Escape:
				stop();
				get_out();
			}
			if (tank.get_fuel_level() <= 0) stop();
			if (speed <= 0 && threads_container.free_wheeling_thread.joinable())
				threads_container.free_wheeling_thread.join();
		} while (key != Escape);
	}

	void free_wheeling()
	{
		while (--speed > 0)
		{
			if (speed < 0) speed = 0;
			std::this_thread::sleep_for(1s);
		}
	}

	void engine_idle()
	{
		while (
			engine.started() && 
			tank.give_fuel(engine.get_consumption_per_second(speed))
			)
		{
			std::this_thread::sleep_for(1s);
		}
	}

	void panel()
	{
		while (driver_inside)
		{
			system("cls");
			cout << "Fuel level: " << tank.get_fuel_level() << " liters";
			if (tank.get_fuel_level() < 5)
			{
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(hConsole, 0xCF);
				cout << " LOW FUEL ";
				SetConsoleTextAttribute(hConsole, 0x07);
			}
			cout << endl;
			cout << "Engine is " << (engine.started() ? "started" : "stopped") << endl;
			cout << "Speed:\t" << speed << " km/h\n";
			cout << "Consumption: " << engine.get_consumption_per_second(speed) << " l/s\n";
			Sleep(100);
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