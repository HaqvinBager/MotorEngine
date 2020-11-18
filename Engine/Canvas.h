#pragma once
#include "Observer.h"

class CButton;

class CCanvas : public IObserver
{
public:
	CCanvas(std::vector<EMessageType> someMessageTypes);
	~CCanvas();

	//void Init();

	void Update(float aDeltaTime);

	void SubscribeToEvents();
	void UnsubscribeToEvents();
private:
	std::vector<CButton*> myButtons;

	std::vector<EMessageType> myMessageTypes;
};