#pragma once
#include <vector>
#include <unordered_map>
#include "MessageType.h"
#include "Observer.h"


class CPostMaster
{
public:
	void Subscribe(const EMessageType aMessageType, IObserver* anObserver);
	void Unsubscribe(const EMessageType aMessageType, IObserver* anObserver);
	void Send(const SMessage& aMessage);
	
private:
	std::unordered_map<EMessageType, std::vector<IObserver*>> myObserverMap;

private:
	CPostMaster();
	~CPostMaster();
};
