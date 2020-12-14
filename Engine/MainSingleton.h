#pragma once
#include "PostMaster.h"
#include "CollisionManager.h"
#include "PopupTextService.h"
#include "DialogueSystem.h"
#include "../Game/PlayerGlobalState.h"

class CMainSingleton
{
	friend class CEngine;

public:
	static CPostMaster& PostMaster();
	static CCollisionManager& CollisionManager();
	static CPopupTextService& PopupTextService();
	static CDialogueSystem& DialogueSystem();
	static CPlayerGlobalState& PlayerGlobalState();

private:
	static CMainSingleton* ourInstance;

	CPostMaster			myPostMaster;
	CCollisionManager	myCollisionManager;
	CPopupTextService	myPopupTextService;
	CDialogueSystem		myDialogueSystem;
	CPlayerGlobalState  myPlayerGlobalState;

private:
	CMainSingleton();
	~CMainSingleton();
};

