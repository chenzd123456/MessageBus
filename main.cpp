#include <iostream>
#include <string>
#include <atomic>

#include <chrono>
#include <exception>

#include "MessageBus.h"

#ifdef _WIN32 && _WIN64
#include <windows.h>
#elif __linux__
#include <unistd.h>
#else
#error
#endif

#define loop_times 10

static std::mutex g_mutex;
int count = 0;



void callback1(std::string topic, std::vector<std::any> args)
{
	//throw std::exception();
	std::cout << "callback1:" << topic << std::endl;
}

void callback2(std::string topic, std::vector<std::any> args)
{
	std::cout << "callback2:" << topic << std::endl;
}

void callback3(std::string topic, std::vector<std::any> args)
{
	//throw std::exception();
	std::cout << "callback3:" << topic << std::endl;
}

void callback4(std::string topic, std::vector<std::any> args)
{
	std::cout << "callback4:" << topic << std::endl;
}

void thread_func1()
{
	auto msg_bus = MessageBus::GetInstance();
	std::vector<std::any> list = { 1,2.5,3,5.6 };
	while(1)
	{
		msg_bus->Publish("topic1", list);
		Sleep(1*1000);
	}
}

void thread_func2()
{
	auto msg_bus = MessageBus::GetInstance();
	std::vector<std::any> list = { 1,2.5,3,5.6 };
	while (1)
	{
		msg_bus->Publish("topic2", list);
		Sleep(1 * 1000);
	}
}

//void thread_func3()
//{
//	auto msg_bus = MessageBus::GetInstance();
//	std::vector<std::any> list = { 1,2.5,3,5.6 };
//	while (1)
//	{
//		msg_bus->Publish("topic3", list);
//		Sleep(1);
//	}
//}
//
//void thread_func4()
//{
//	auto msg_bus = MessageBus::GetInstance();
//	std::vector<std::any> list = { 1,2.5,3,5.6 };
//	while (1)
//	{
//		msg_bus->Publish("topic4", list);
//		Sleep(1);
//	}
//}

int main()
{
	auto msg_bus = MessageBus::GetInstance();
	msg_bus->Register("Subscriber1", callback1);
	msg_bus->Subscribe("topic1", "Subscriber1");
	msg_bus->Register("Subscriber2", callback2);
	msg_bus->Subscribe("topic1", "Subscriber2");
	msg_bus->Register("Subscriber3", callback3);
	msg_bus->Subscribe("topic2", "Subscriber3");
	msg_bus->Register("Subscriber4", callback4);
	msg_bus->Subscribe("topic2", "Subscriber4");

	std::thread t1(thread_func1);
	std::thread t2(thread_func2);
	//std::thread t3(thread_func3);
	//std::thread t4(thread_func4);

	while (1)
	{
		Sleep(10000);
	}
}
