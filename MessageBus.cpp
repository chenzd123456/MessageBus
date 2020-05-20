#include "MessageBus.h"
#include "_MessageBus.h"
#include "ThreadPool.h"

MessageBus::MessageBus()
{
	m_msg_bus = std::unique_ptr<IMessageBus>(new _MessageBus());
}

MessageBus::~MessageBus()
{
}

std::shared_ptr<MessageBus> MessageBus::GetInstance()
{
	return Singleton<MessageBus>::GetInstance();
}

void MessageBus::DesInstance()
{
	Singleton<MessageBus>::DesInstance();
}

void MessageBus::Register(_in_ std::string subscriber, _in_ std::function<void(_in_ std::string topic, _in_ std::vector<std::any>)> callback)
{
	m_msg_bus->Register(subscriber, callback);
}

void MessageBus::Subscribe(_in_ std::string topic, _in_ std::string subscriber)
{
	m_msg_bus->Subscribe(topic, subscriber);
}

bool MessageBus::Publish(_in_ std::string topic, _in_ std::vector<std::any> args)
{
	return m_msg_bus->Publish(topic, args);
}
