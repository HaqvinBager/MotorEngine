#include "stdafx.h"
#include "PlayerGlobalState.h"

#include "StatsComponent.h"

CPlayerGlobalState::CPlayerGlobalState()
{}

CPlayerGlobalState::~CPlayerGlobalState()
{}

void CPlayerGlobalState::SetStatsToSave(const SStats & someStats)
{
	myStatsToSave.myPlayerExperience = someStats.myExperience;
	myStatsToSave.myPlayerLevel = someStats.myLevel;
}

const float CPlayerGlobalState::GetSavedExperience()
{
	return myStatsToSave.myPlayerExperience;
}

const int CPlayerGlobalState::GetSavedPlayerLevel()
{
	return myStatsToSave.myPlayerLevel;
}
