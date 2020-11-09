#include "stdafx.h"
#include "PostMaster.h"
#include "Message.h"

void CPostMaster::Subscribe(const EMessageType aMessageType, IObserver* anObserver)
{
	myObserverMap[aMessageType].push_back(anObserver);
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

void CPostMaster::Send(const SMessage& aMessage)
{
	for (int i = 0; i < myObserverMap[aMessage.myMessageType].size(); ++i)
	{
		myObserverMap[aMessage.myMessageType][i]->Receive(aMessage);
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
