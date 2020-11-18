#include "stdafx.h"
#include "Canvas.h"
#include "MainSingleton.h"
#include "Button.h"

CCanvas::CCanvas(std::vector<EMessageType> someMessageTypes) : myMessageTypes(someMessageTypes)
{
	SubscribeToEvents();
}

CCanvas::~CCanvas()
{
	UnsubscribeToEvents();
	myMessageTypes.clear();
}

//void CCanvas::Init()
//{
//}

void CCanvas::Update(float /*aDeltaTime*/)
{
}

void CCanvas::SubscribeToEvents()
{
	for (auto messageType : myMessageTypes) {
		CMainSingleton::PostMaster().Subscribe(messageType, this);
	}
}

void CCanvas::UnsubscribeToEvents()
{
	for (auto messageType : myMessageTypes) {
		CMainSingleton::PostMaster().Unsubscribe(messageType, this);
	}
}