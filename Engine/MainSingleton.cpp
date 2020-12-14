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

CCollisionManager & CMainSingleton::CollisionManager()
{
	return ourInstance->myCollisionManager;
}

CPopupTextService& CMainSingleton::PopupTextService()
{
	return ourInstance->myPopupTextService;
}

CDialogueSystem& CMainSingleton::DialogueSystem()
{
	return ourInstance->myDialogueSystem;
}

CPlayerGlobalState& CMainSingleton::PlayerGlobalState()
{
	return ourInstance->myPlayerGlobalState;
}
