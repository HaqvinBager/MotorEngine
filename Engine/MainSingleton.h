#pragma once
#include "PostMaster.h"
#include "StateStack.h"

class CMainSingleton
{

	friend class CEngine;

public:

	static CPostMaster& PostMaster();
	static CStateStack& StateStack();

private:
	static CMainSingleton* ourInstance;
	CPostMaster myPostMaster;
	CStateStack myStateStack;

private:
	CMainSingleton();
	~CMainSingleton();
};

