#pragma once

#include "MessageBusHeader.h"

class IMessageBus
{
public:
	IMessageBus() {};
	virtual ~IMessageBus() {};
	virtual void Register(_in_ std::string subscriber, _in_ std::function<void(_in_ std::string topic, _in_ std::vector<std::any>)> callback) = 0;
	virtual void Subscribe(_in_ std::string topic, _in_ std::string subscriber) = 0;
	virtual bool Publish(_in_ std::string topic, _in_ std::vector<std::any> args) = 0;
};