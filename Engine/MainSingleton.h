#pragma once
#include "PostMaster.h"
#include "CollisionManager.h"

class CMainSingleton
{

	friend class CEngine;

public:
	static CPostMaster& PostMaster();
	static CCollisionManager& CollisionManager();

private:
	static CMainSingleton* ourInstance;
	CPostMaster myPostMaster;
	CCollisionManager myCollisionManager;

private:
	CMainSingleton();
	~CMainSingleton();
};

