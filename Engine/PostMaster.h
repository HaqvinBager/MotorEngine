#pragma once
#include <vector>
#include <unordered_map>
#include "MessageType.h"
#include "Observer.h"
#include <string>


class CPostMaster
{	

	friend class CMainSingleton;

public:
	void Subscribe(const EMessageType aMessageType, IObserver* anObserver);
	void Unsubscribe(const EMessageType aMessageType, IObserver* anObserver);
	void Send(const SMessage& aMessage);

	void Subscribe(const char*, IStringObserver* anObserver);
	void Unsubscribe(const std::string aMessageType, IStringObserver* anObserver);
	void Send(const SStringMessage& aMessage);
	
private:
	std::unordered_map<EMessageType, std::vector<IObserver*>> myObserverMap;
	std::unordered_map<std::string, std::vector<IStringObserver*>> myStringObserverMap;
private:
	CPostMaster();
	~CPostMaster();
};
