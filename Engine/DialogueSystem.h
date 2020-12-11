#pragma once
#include <vector>
#include <memory>
#include <string>
#include "SimpleMath.h"
#include "PostMaster.h"

class CSpriteInstance;
class CTextInstance;
class CScene;

struct SSpeakerLine {
	SSpeakerLine(std::string aString, int aSpeaker, std::string aSpeakerName, int aLine)
		: myText(aString)
		, mySpeakerNumber(aSpeaker)
		, mySpeakerName(aSpeakerName)
		, myVoiceLine(aLine) {}

	std::string myText;
	std::string mySpeakerName;
	int mySpeakerNumber;
	int myVoiceLine;
};

class CDialogueSystem : public IObserver, public IStringObserver {
public:
	CDialogueSystem();
	~CDialogueSystem();

	bool Init();
	void Receive(const SMessage& aMessage) override;
	void Receive(const SStringMessage& aMessage) override;
	void Update();

public:
	void EmplaceSprites(std::vector<CSpriteInstance*>& someSprites) const;
	void EmplaceTexts(std::vector<CTextInstance*>& someText) const;
	
	bool Active() {
		return myIsActive;
	}
	void Active(bool isActive) {
		myIsActive = isActive;
	}

	void ExitDialogue();
private:
	void LoadDialogue(int aSceneIndex);
	void LoadNarration();

	void HandleInput();
	void SetDialogueSpeed(float aSpeed, int aLineLength);
	void ProcessLineBreaks();

	

private:
	CSpriteInstance* myDialogueBox;
	CSpriteInstance* myCurrentSpeakerPortrait;
	std::vector<CSpriteInstance*> mySpeakerPortraits;

	CTextInstance* myCurrentSpeakerName;
	std::vector<CTextInstance*> mySpeakerNames;

	CTextInstance* myAnimatedDialogue;
	CTextInstance* myAnimatedNarration;

	std::vector<SSpeakerLine> myDialogueBuffer;
	std::string myCurrentLine = "";

	bool myIsActive = false;
	bool myIsNarration = false;
	bool myEnabled = true;
	int myCurrentDialogueIndex = 0;
	int myLastDialogueIndex = -1;
	int myLastSpeakerIndex = -1;
	int myCurrentVoiceLine = -2;
	int myLineBreakCounter = 0;
	int myLineBreakDialogue = 0;
	int myLineBreakNarration = 0;
	float myHeldButtonTimer = 0.0f;
	float myDialogueTimer = 0.0f;
	float myDialogueSpeed = 0.05f;
	float myDialogueSlowSpeed = 0.05f;
	float myDialogueFastSpeed = 0.01f;
};

