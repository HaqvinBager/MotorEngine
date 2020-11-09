#pragma once
#include "Message.h"

class IObserver
{
public:
	virtual ~IObserver() { }
	virtual void receive(const SMessage& message) = 0;
};