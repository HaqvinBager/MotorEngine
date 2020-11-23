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

	MapEvent(IInputObserver::EInputAction::MouseLeft, IInputObserver::EInputEvent::MoveClick);
}

CInputMapper::~CInputMapper()
{
}

bool CInputMapper::Init()
{
	MapEvent(IInputObserver::EInputAction::Key1, IInputObserver::EInputEvent::Ability1);
	MapEvent(IInputObserver::EInputAction::Key2, IInputObserver::EInputEvent::Ability2);
	MapEvent(IInputObserver::EInputAction::Key3, IInputObserver::EInputEvent::Ability3);
	MapEvent(IInputObserver::EInputAction::KeyA, IInputObserver::EInputEvent::Ability1);
	MapEvent(IInputObserver::EInputAction::KeyS, IInputObserver::EInputEvent::Ability2);
	MapEvent(IInputObserver::EInputAction::KeyD, IInputObserver::EInputEvent::Ability3);

	if (this == nullptr)
		return false;
	else
		return true;
}

void CInputMapper::RunEvent(const IInputObserver::EInputEvent aOutputEvent)
{
	for (int i = 0; i < myObservers[aOutputEvent].size(); ++i)
	{
		myObservers[aOutputEvent][i]->RecieveEvent(aOutputEvent);
	}
}

void CInputMapper::TranslateActionToEvent(const IInputObserver::EInputAction aAction)
{
	const auto eventIterator = myEvents.find(aAction);
	if (eventIterator != myEvents.end())
	{
		RunEvent(myEvents[aAction]);
	}
}

void CInputMapper::UpdateKeyboardInput()
{
	if (myInput->IsKeyPressed(VK_ESCAPE))
	{
		TranslateActionToEvent(IInputObserver::EInputAction::KeyEscape);
	} else if(myInput->IsKeyDown(VK_ESCAPE))
	{
		TranslateActionToEvent(IInputObserver::EInputAction::KeyEscape);
	} else if (myInput->IsKeyReleased(VK_ESCAPE))
	{
		TranslateActionToEvent(IInputObserver::EInputAction::KeyEscape);
	}

	if (myInput->IsKeyPressed('1'))
	{
		TranslateActionToEvent(IInputObserver::EInputAction::Key1);
	}
	if (myInput->IsKeyPressed('2'))
	{
		TranslateActionToEvent(IInputObserver::EInputAction::Key2);
	}
	if (myInput->IsKeyPressed('3'))
	{
		TranslateActionToEvent(IInputObserver::EInputAction::Key3);
	}

	if (myInput->IsKeyPressed('A'))
	{
		TranslateActionToEvent(IInputObserver::EInputAction::KeyA);
	}
	if (myInput->IsKeyPressed('S'))
	{
		TranslateActionToEvent(IInputObserver::EInputAction::KeyS);
	}
	if (myInput->IsKeyPressed('D'))
	{
		TranslateActionToEvent(IInputObserver::EInputAction::KeyD);
	}
}

void CInputMapper::UpdateMouseInput()
{

	if (myInput->IsMousePressed(CommonUtilities::Input::MouseButton::Left))
	{
		TranslateActionToEvent(IInputObserver::EInputAction::MouseLeft);
	}
	//else if (myInput->IsMouseDown(CommonUtilities::Input::MouseButton::Left))
	//{
	//	TranslateActionToEvent(IInputObserver::EInputAction::MouseLeft);
	//}
	//else if (myInput->IsMouseReleased(CommonUtilities::Input::MouseButton::Left))
	//{
	//	TranslateActionToEvent(IInputObserver::EInputAction::MouseLeft);
	//}
}

void CInputMapper::Update()
{
	UpdateKeyboardInput();
	UpdateMouseInput();
	myInput->update();
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