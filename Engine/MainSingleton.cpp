#include "stdafx.h"
#include "MainSingleton.h"

CMainSingleton* CMainSingleton::ourInstance = nullptr;

CMainSingleton::CMainSingleton() {
	ourInstance = this;
}

CMainSingleton::~CMainSingleton()
{
	ourInstance = nullptr;
}

CPostMaster& CMainSingleton::PostMaster()
{
	return ourInstance->myPostMaster;
}