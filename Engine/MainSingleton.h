#pragma once
#include "PostMaster.h"
#include "PopupTextService.h"

class CMainSingleton
{
	friend class CEngine;

public:
	static CPostMaster& PostMaster();
	static CPopupTextService& PopupTextService();

private:
	static CMainSingleton* ourInstance;
	CPostMaster myPostMaster;
	CPopupTextService myPopupTextService;

private:
	CMainSingleton();
	~CMainSingleton();
};

