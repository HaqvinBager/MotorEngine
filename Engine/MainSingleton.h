#pragma once
#include "PostMaster.h"
#include "PopupTextService.h"
#include "DialogueSystem.h"

class CMainSingleton
{
	friend class CEngine;

public:
	static CPostMaster& PostMaster();
	static CPopupTextService& PopupTextService();
	static CDialogueSystem& DialogueSystem();

private:
	static CMainSingleton* ourInstance;
	CPostMaster myPostMaster;
	CPopupTextService myPopupTextService;
	CDialogueSystem myDialogueSystem;

private:
	CMainSingleton();
	~CMainSingleton();
};

