#include "stdafx.h"
#include "InputMapper.h"
#include "Input.h"

CInputMapper* CInputMapper::ourInstance = nullptr;

CInputMapper* CInputMapper::GetInstance()
{
	return ourInstance;
}

CInputMapper::CInputMapper() : myInput(CommonUtilities::Input::GetInstance())
{
	ourInstance = this;
}

CInputMapper::~CInputMapper()
{
}

void CInputMapper::RunEvent(const IInputObserver::EInputEvent aOutputEvent, const float aValue)
{
	for (int i = 0; i < myObservers[aOutputEvent].size(); ++i)
	{
		myObservers[aOutputEvent][i]->RecieveEvent(aOutputEvent, aValue);
	}
}

void CInputMapper::TranslateActionToEvent(const IInputObserver::EInputAction aAction, const float aValue)
{
	const auto eventIterator = myEvents.find(aAction);
	if (eventIterator != myEvents.end())
	{
		RunEvent(myEvents[aAction], aValue);
	}
}

void CInputMapper::UpdateKeyboardInput()
{
	if (myInput->IsKeyPressed(VK_ESCAPE))
	{
		TranslateActionToEvent(IInputObserver::EInputAction::KeyEscape, 1.0f);
	} else if(myInput->IsKeyDown(VK_ESCAPE))
	{
		TranslateActionToEvent(IInputObserver::EInputAction::KeyEscape, 1.0f);
	} else if (myInput->IsKeyReleased(VK_ESCAPE))
	{
		TranslateActionToEvent(IInputObserver::EInputAction::KeyEscape, 1.0f);
	}
}

void CInputMapper::UpdateMouseInput()
{

	if (myInput->IsMousePressed(CommonUtilities::Input::MouseButton::Left))
	{
		TranslateActionToEvent(IInputObserver::EInputAction::MouseLeft, 1.0f);
	}
	else if (myInput->IsMouseDown(CommonUtilities::Input::MouseButton::Left))
	{
		TranslateActionToEvent(IInputObserver::EInputAction::MouseLeft, 1.0f);
	}
	else if (myInput->IsMouseReleased(CommonUtilities::Input::MouseButton::Left))
	{
		TranslateActionToEvent(IInputObserver::EInputAction::MouseLeft, 1.0f);
	}
}

void CInputMapper::Update()
{
	myInput->update();
	UpdateKeyboardInput();
	UpdateMouseInput();
}

void CInputMapper::MapEvent(const IInputObserver::EInputAction aInputEvent, const IInputObserver::EInputEvent aOutputEvent)
{
	myEvents[aInputEvent] = aOutputEvent;
}

bool CInputMapper::AddObserver(const IInputObserver::EInputEvent aEventToListenFor, IInputObserver* aObserver)
{
	ENGINE_ERROR_BOOL_MESSAGE(aObserver, "InputObsever is nullptr!");
	myObservers[aEventToListenFor].emplace_back(aObserver);
	return true;
}

bool CInputMapper::RemoveObserver(const IInputObserver::EInputEvent aEventToListenFor, IInputObserver* aObserver)
{
	ENGINE_ERROR_BOOL_MESSAGE(aObserver, "InputObsever is nullptr!");
	auto it = std::find(myObservers[aEventToListenFor].begin(), myObservers[aEventToListenFor].end(), aObserver);
	myObservers[aEventToListenFor].erase(it);
	return true;
}