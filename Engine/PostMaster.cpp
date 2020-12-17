#include "stdafx.h"
#include "PostMaster.h"
#include "Message.h"

void CPostMaster::Subscribe(const EMessageType aMessageType, IObserver* anObserver)
{
	if (myObserverMap.find(aMessageType) != myObserverMap.end())
	{
		if (std::find(myObserverMap[aMessageType].begin(), myObserverMap[aMessageType].end(), anObserver) == myObserverMap[aMessageType].end())
		{
			myObserverMap[aMessageType].push_back(anObserver);
		}
	}
}

void CPostMaster::Unsubscribe(const EMessageType aMessageType, IObserver* anObserver)
{
	for (int i = 0; i < myObserverMap[aMessageType].size(); ++i)
	{
		if (myObserverMap[aMessageType][i] == anObserver)
		{
			myObserverMap[aMessageType][i] = myObserverMap[aMessageType].back();
			myObserverMap[aMessageType].pop_back();
			return;
		}
	}
}

void CPostMaster::SendLate(const SMessage& aMessage)
{
	if (aMessage.myMessageType == EMessageType::Count)
		return;

	myMessageQueue.push(SMessage(aMessage));
}

void CPostMaster::Send(const SMessage& aMessage)
{
	for (int i = 0; i < myObserverMap[aMessage.myMessageType].size(); ++i)
	{
		myObserverMap[aMessage.myMessageType][i]->Receive(aMessage);
	}
}

void CPostMaster::Subscribe(const char* aMessageType, IStringObserver* anObserver)
{
	if (myStringObserverMap.find(aMessageType) != myStringObserverMap.end())
		if (std::find(myStringObserverMap[aMessageType].begin(), myStringObserverMap[aMessageType].end(), anObserver) != myStringObserverMap[aMessageType].end())
			return;

	myStringObserverMap[aMessageType].push_back(anObserver);
}


void CPostMaster::Unsubscribe(const std::string aMessageType, IStringObserver* anObserver)
{
	for (int i = 0; i < myStringObserverMap[aMessageType].size(); ++i)
	{
		if (myStringObserverMap[aMessageType][i] == anObserver)
		{
			myStringObserverMap[aMessageType][i] = myStringObserverMap[aMessageType].back();
			myStringObserverMap[aMessageType].pop_back();
			return;
		}
	}
}

void CPostMaster::Send(const SStringMessage& aMessage)
{
	for (int i = 0; i < myStringObserverMap[aMessage.myMessageType].size(); ++i)
	{
		myStringObserverMap[aMessage.myMessageType][i]->Receive(aMessage);
	}

	//if (strlen(aMessage.myMessageType) == 0)
	//	return;

	//myStringMessageQueue.push(SStringMessage(aMessage));
}

void CPostMaster::FlushEvents()
{
	while (!myMessageQueue.empty())
	{
		SMessage message = myMessageQueue.front();
		for (int i = 0; i < myObserverMap[message.myMessageType].size(); ++i)
		{
			myObserverMap[message.myMessageType][i]->Receive(message);
		}
		myMessageQueue.pop();
	}

	while (!myStringMessageQueue.empty())
	{
		SStringMessage message = myStringMessageQueue.front();
		for (int i = 0; i < myStringObserverMap[message.myMessageType].size(); ++i)
		{
			myStringObserverMap[message.myMessageType][i]->Receive(message);
		}
		myStringMessageQueue.pop();
	}
}

CPostMaster::CPostMaster()
{
	for (unsigned int i = 0; i < static_cast<unsigned int>(EMessageType::Count); ++i)
	{
		myObserverMap[static_cast<EMessageType>(i)] = std::vector<IObserver*>();
	}
}

CPostMaster::~CPostMaster()
{
	for (unsigned int i = 0; i < static_cast<unsigned int>(EMessageType::Count); ++i)
	{
		for (unsigned int index = 0; index < myObserverMap[static_cast<EMessageType>(i)].size(); ++i)
		{
			//Should we Delete the Observers? Or will that cause a crash because we also delete the object that inherits from observer?
			myObserverMap[static_cast<EMessageType>(i)][index] = nullptr;
		}

		myObserverMap[static_cast<EMessageType>(i)].clear();
	}

	myObserverMap.clear();
}
