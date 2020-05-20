#pragma once

#include "MessageBusHeader.h"
#include "Singleton.h"
#include "IMessageBus.h"

class MessageBus
{
	friend class Singleton<MessageBus>;
private:
	std::unique_ptr<IMessageBus> m_msg_bus;
	MessageBus();
public:
	~MessageBus();
	static std::shared_ptr<MessageBus> GetInstance();
	static void DesInstance();
	void Register(_in_ std::string subscriber, _in_ std::function<void(_in_ std::string topic, _in_ std::vector<std::any>)> callback);
	void Subscribe(_in_ std::string topic, _in_ std::string subscriber);
	bool Publish(_in_ std::string topic, _in_ std::vector<std::any> args);
};
