#pragma once
#include "InputObserver.h"
#include <unordered_map>

class CInputMapper
{
	friend class CEngine;
public:
	static CInputMapper* GetInstance();

	void Update();
	void MapEvent(const IInputObserver::EInputAction aInputEvent, const IInputObserver::EInputEvent aOutputEvent);
	bool AddObserver(const IInputObserver::EInputEvent aEventToListenFor, IInputObserver* aObserver);
	bool RemoveObserver(const IInputObserver::EInputEvent aEventToListenFor, IInputObserver* aObserver);

private:
	static CInputMapper* ourInstance;

	CInputMapper();
	~CInputMapper();

	void RunEvent(const IInputObserver::EInputEvent aOutputEvent);
	void TranslateActionToEvent(const IInputObserver::EInputAction aAction);
	void UpdateKeyboardInput();
	void UpdateMouseInput();

	std::unordered_map<IInputObserver::EInputEvent, std::vector<IInputObserver*>> myObservers;
	std::unordered_map<IInputObserver::EInputAction, IInputObserver::EInputEvent> myEvents;
	CommonUtilities::Input* myInput;
};

