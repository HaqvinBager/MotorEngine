#pragma once
#include "MessageType.h"

struct SMessage
{
	EMessageType myMessageType;
	void* data;
};

struct SStringMessage
{
	const char* myMessageType;
	void* data;
};