#pragma once
#include "InputObserver.h"
#include <unordered_map>

class Input;

class CInputMapper
{
	friend class CEngine;
public:
	static CInputMapper* GetInstance();

	void Update();
	void MapEvent(const IInputObserver::EInputAction aInputEvent, const IInputObserver::EInputEvent aOutputEvent);
	bool AddObserver(const IInputObserver::EInputEvent aEventToListenFor, IInputObserver* aObserver);
	bool RemoveObserver(const IInputObserver::EInputEvent aEventToListenFor, IInputObserver* aObserver);
	bool HasObserver(const IInputObserver::EInputEvent aEventToListenFor, IInputObserver* aObserver);

	void ClearObserverList(const IInputObserver::EInputEvent aEventToListenFor);

private:
	static CInputMapper* ourInstance;

	CInputMapper();
	~CInputMapper();

	bool Init();

	void RunEvent(const IInputObserver::EInputEvent aOutputEvent);
	void TranslateActionToEvent(const IInputObserver::EInputAction aAction);
	void UpdateKeyboardInput();
	void UpdateMouseInput();

	std::unordered_map<IInputObserver::EInputEvent, std::vector<IInputObserver*>> myObservers;
	std::unordered_map<IInputObserver::EInputAction, IInputObserver::EInputEvent> myEvents;
	Input* myInput;
};

