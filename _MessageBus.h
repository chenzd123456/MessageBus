#pragma once

#include "IMessageBus.h"

#include <thread>
#include <mutex>
#include <queue>

#include "MessageBusHeader.h"
#include "ThreadPool.h"
#include "Semaphore.h"


class _MessageBus : public IMessageBus
{
private:
	volatile bool m_is_message_bus_quit;
	std::condition_variable m_message_process_thread_cv;
	std::mutex m_callback_map_mutex;
	std::mutex m_topic_map_mutex;
	std::mutex m_message_queue_mutex;
	Semaphore m_message_queue_semaphore;
	std::unique_ptr<std::map<std::string, std::function<void(_in_ std::string topic, _in_ std::vector<std::any>)>>> m_callback_map;  // subscriber, callback
	std::unique_ptr<std::map<std::string, std::unique_ptr<std::vector<std::string>>>> m_topic_map;  // topic, subscriber_list
	std::unique_ptr<std::queue<std::pair<std::string, std::vector<std::any>>>> m_message_queue;
	std::unique_ptr<std::thread> m_message_process_thread;
	std::unique_ptr<ThreadPool> m_thread_pool;

public:
	_MessageBus();
	virtual ~_MessageBus();
	virtual void Register(_in_ std::string subscriber, _in_ std::function<void(_in_ std::string topic, _in_ std::vector<std::any>)> callback) override;
	virtual void Subscribe(_in_ std::string topic, _in_ std::string subscriber) override;
	virtual bool Publish(_in_ std::string topic, _in_ std::vector<std::any> args) override;
	void Invoker();
};