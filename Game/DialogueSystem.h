#pragma once
#include <vector>
#include <memory>
#include <string>
#include "SimpleMath.h"
//#include "PostMaster.hpp"

class CSpriteInstance;
class CTextInstance;
class CScene;

struct SSpeakerLine {
	SSpeakerLine(std::string aString, int aSpeaker, int aLine)
		: myText(aString)
		, mySpeakerNumber(aSpeaker)
		, myVoiceLine(aLine) {}

	std::string myText;
	int mySpeakerNumber;
	int myVoiceLine;
};

class CDialogueSystem {
public:
	CDialogueSystem(CScene& aScene);
	~CDialogueSystem();

	void LoadDialogue(const char* aSceneName);
	void LoadInfo(const char* someInfo);
	void LoadScroll(const char* aScroll);
	void ExitDialogue();

	void Update(float dt);

	const bool GetActive() const { return myIsActive; }
	void SetActive(bool isActive) { myIsActive = isActive; }
	const int CurrentSpeaker() const;

private:
	void HandleInput(float delta_time);
	void SetDialogueSpeed(float aSpeed, int aLineLength);

	CSpriteInstance* myDialogueBox;
	std::vector<CSpriteInstance*> mySpeakerPortraits;

	CTextInstance* myDialogueLine;
	CTextInstance* myAnimatedDialogue;

	std::vector<SSpeakerLine> myDialogueBuffer;
	std::string myCurrentLine = "";

	bool myIsActive = false;
	bool myIsInfo = false;
	bool myIsInventory = false;
	int myCurrentDialogueIndex = 0;
	int myLastSpeakerIndex = -1;
	int myCurrentVoiceLine = -2;
	int myLineBreakCounter = 0;
	int myLineBreakDialogue = 35;
	int myLineBreakInfo = 35;
	int myLettersLastFrame = 0;
	float myHeldButtonTimer = 0.0f;
	float myDialogueTimer = 0.0f;
	float myLineFinished = 2.5f;
	float myDialogueSpeed = 0.05f;

	float myDialogueBoxHeight = 0.75f;
	float myDialogueBoxWidth = 0.29f;
	float myInfoBoxHeight = 0.25f;
	float myInfoBoxWidth = 0.345f;
	float myInventoryBoxHeight = 0.2f;
	float myInventoryBoxWidth = 0.305f;
	float myOffsetDialogue = 0.015f;
	float myOffsetInfo = 0.01f;
	float myTextPosOffset = 0.0f;
};

