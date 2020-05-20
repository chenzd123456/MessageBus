#include "_MessageBus.h"

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <unistd.h>
#else
#error
#endif

#define __USE_THREAD_POOL 1
#define MAX_THREAD 8
#define MAX_MSG_QUEUE_SIZE 1000

_MessageBus::_MessageBus()
{
	m_is_message_bus_quit = false;
	m_callback_map = std::make_unique<std::map<std::string, std::function<void(_in_ std::string topic, _in_ std::vector<std::any>)>>>();
	m_topic_map = std::make_unique<std::map<std::string, std::unique_ptr<std::vector<std::string>>>>();
	m_message_queue = std::make_unique<std::queue<std::pair<std::string, std::vector<std::any>>>>();

#if __USE_THREAD_POOL
	m_thread_pool = std::make_unique<ThreadPool>(MAX_THREAD);
#endif

	std::mutex mutex;
	std::unique_lock<std::mutex> lk(mutex);
	m_message_process_thread = std::make_unique<std::thread>([this]() {this->Invoker(); });
	m_message_process_thread_cv.wait(lk);
}

_MessageBus::~_MessageBus()
{
	m_is_message_bus_quit = true;
	m_message_process_thread->join();
}

void _MessageBus::Subscribe(_in_ std::string topic, _in_ std::string subscriber)
{
	std::lock_guard<std::mutex> lock(m_topic_map_mutex);

	if (m_topic_map->find(topic) == m_topic_map->end())
	{
		m_topic_map->insert({ topic, std::make_unique<std::vector<std::string>>() });
	}

	m_topic_map->at(topic)->push_back(subscriber);
}

void _MessageBus::Register(_in_ std::string subscriber, _in_ std::function<void(_in_ std::string topic, _in_ std::vector<std::any>)> callback)
{
	std::lock_guard<std::mutex> lock(m_callback_map_mutex);
	m_callback_map->insert({ subscriber, callback });
}

bool _MessageBus::Publish(_in_ std::string topic, _in_ std::vector<std::any> args)
{
	auto size = m_message_queue->size();

	if (m_message_queue->size() > MAX_MSG_QUEUE_SIZE)
	{
		return false;
	}
	else
	{
		std::lock_guard<std::mutex> lock(m_message_queue_mutex);
		m_message_queue->push({ topic , args });
		m_message_queue_semaphore.Signal();
		return true;
	}
}

void _MessageBus::Invoker()
{
	std::pair<std::string, std::vector<std::any>> event;
	m_message_process_thread_cv.notify_all();
	while (!m_is_message_bus_quit)
	{

		if (m_message_queue->empty())
		{
			m_message_queue_semaphore.Wait();
			continue;
		}

		{
			std::lock_guard<std::mutex> lock(m_message_queue_mutex);
			event = m_message_queue->front();
			m_message_queue->pop();
		}

		if (m_topic_map->find(event.first) != m_topic_map->end())
		{
			for (int i = 0; i < m_topic_map->at(event.first)->size(); i++)
			{
				auto subscriber = m_topic_map->at(event.first)->at(i);
				auto callback = m_callback_map->at(subscriber);

#if __USE_THREAD_POOL
				m_thread_pool->enqueue([callback, event](){callback(event.first, event.second); });
#else
				try
				{
					callback(event.first, event.second);
				}
				catch (...)
				{
					continue;
				}
#endif
			}
		}
	}
}
