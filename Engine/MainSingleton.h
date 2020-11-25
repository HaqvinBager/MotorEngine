#pragma once
#include "PostMaster.h"

class CMainSingleton
{

	friend class CEngine;

public:

	static CPostMaster& PostMaster();

private:
	static CMainSingleton* ourInstance;
	CPostMaster myPostMaster;

private:
	CMainSingleton();
	~CMainSingleton();
};

