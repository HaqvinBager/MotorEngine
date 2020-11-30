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
	CStateStack myStateStack;
};

