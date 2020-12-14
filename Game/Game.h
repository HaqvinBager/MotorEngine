#pragma once
#include "StateStack.h"

class CGame
{
public:
	CGame();
	~CGame();

	void Init();
	bool Update();

private:

	void InitDev();
	void InitRealGame();

	CStateStack myStateStack;
};

