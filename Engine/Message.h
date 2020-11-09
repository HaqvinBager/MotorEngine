#pragma once
#include "MessageType.h"

struct SMessage
{
	EMessageType myMessageType;
	void* data;
};