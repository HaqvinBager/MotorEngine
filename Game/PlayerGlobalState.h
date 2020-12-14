#pragma once

struct SStats;
class CPlayerGlobalState
{
public:
	struct SStatsToSave
	{
		float myPlayerExperience = 0.f;//taken from CUnityFactory create player
		int myPlayerLevel = 0;
	};
	
public:
	CPlayerGlobalState();
	~CPlayerGlobalState();

	void SetStatsToSave(const SStats& someStats);
	const float GetSavedExperience();
	const int GetSavedPlayerLevel();

private:
	SStatsToSave myStatsToSave;
};

