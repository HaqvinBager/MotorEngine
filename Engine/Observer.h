#pragma once
#include "Message.h"

class IObserver
{
public:
	virtual ~IObserver() { }
	virtual void Receive(const SMessage& aMessage) = 0;
};