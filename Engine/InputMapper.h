#pragma once
#include "InputObserver.h"
#include <unordered_map>

class CInputMapper
{
	friend class CEngine;

public:
	static CInputMapper* GetInstance();

	void Update();
	void MapEvent(const CInputObserver::EInputAction aInputEvent, const CInputObserver::EInputEvent aOutputEvent);
	bool AddObserver(const CInputObserver::EInputEvent aEventToListenFor, CInputObserver* aObserver);
	bool RemoveObserver(const CInputObserver::EInputEvent aEventToListenFor, CInputObserver* aObserver);

private:
	static CInputMapper* ourInstance;

	CInputMapper();
	~CInputMapper();

	void RunEvent(const CInputObserver::EInputEvent aOutputEvent, const float aValue = 0);
	void TranslateActionToEvent(const CInputObserver::EInputAction aAction, const float aValue = 0);
	void UpdateKeyboardInput();

	std::unordered_map<CInputObserver::EInputEvent, std::vector<CInputObserver*>> myObservers;
	std::unordered_map<CInputObserver::EInputAction, CInputObserver::EInputEvent> myEvents;
	CommonUtilities::Input* myInput;
};

