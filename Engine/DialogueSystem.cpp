#include "stdafx.h"
#include "DialogueSystem.h"

#include <sstream>
#include <cstdio>

#include "TextFactory.h"
#include "TextInstance.h"
#include "SpriteFactory.h"
#include "SpriteInstance.h"
#include "Scene.h"

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

#include "Input.h"
#include "JsonReader.h"
#include "MainSingleton.h"

namespace SM = DirectX::SimpleMath;

CDialogueSystem::CDialogueSystem()
{
	myCurrentSpeakerName = nullptr;
	myAnimatedDialogue = nullptr;
	myAnimatedNarration = nullptr;
	myDialogueBox = nullptr;
	myCurrentSpeakerPortrait = nullptr;
}

CDialogueSystem::~CDialogueSystem() {
	delete myCurrentSpeakerName;
	myCurrentSpeakerName = nullptr;
	delete myAnimatedDialogue;
	myAnimatedDialogue = nullptr;
	delete myDialogueBox;
	myDialogueBox = nullptr;
	delete myCurrentSpeakerPortrait;
	myCurrentSpeakerPortrait = nullptr;

	// For some reason we always seem to destroy the postmaster before the dialogue system
	//CMainSingleton::PostMaster().Unsubscribe(EMessageType::LoadDialogue, this);
}

bool CDialogueSystem::Init()
{
	CMainSingleton::PostMaster().Subscribe(EMessageType::LoadDialogue, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::IntroStarted, this);
	CMainSingleton::PostMaster().Subscribe("DELevel1", this);
	CMainSingleton::PostMaster().Subscribe("DELevel2", this);
	CMainSingleton::PostMaster().Subscribe("DELevel3", this);

	rapidjson::Document document = CJsonReader::LoadDocument("Json/DialogueSystemInit.json");
	ENGINE_BOOL_POPUP(!document.HasParseError(), "Could not load 'Json/DialogueSystemInit.json'!");

	CSpriteFactory* factory = CSpriteFactory::GetInstance();
	CTextFactory* textFactory = CTextFactory::GetInstance();

	myDialogueBox = new CSpriteInstance();
	myDialogueBox->Init(factory->GetSprite(document["Dialogue Box Path"].GetString()));
	myDialogueBox->SetPosition({0.0f, 0.4f});

	auto iconArray = document["Speaker Icon Paths"].GetArray();
	for (unsigned int i = 0; i < iconArray.Size(); ++i)
	{
		mySpeakerPortraits.emplace_back(new CSpriteInstance());
		mySpeakerPortraits.back()->Init(factory->GetSprite(iconArray[i]["Path"].GetString()));
		mySpeakerPortraits.back()->SetPosition({document["Speaker Icon Position X"].GetFloat(), document["Speaker Icon Position Y"].GetFloat()});

		mySpeakerNames.emplace_back(new CTextInstance());
		mySpeakerNames.back()->Init(textFactory->GetText(document["Speaker Names Font and Size"].GetString()));
		mySpeakerNames.back()->SetPivot({0.0f, 0.5f});
		mySpeakerNames.back()->SetPosition({document["Speaker Name Position X"].GetFloat(), document["Speaker Name Position Y"].GetFloat()});
		mySpeakerNames.back()->SetColor({document["Speaker Name Color R"].GetFloat(), document["Speaker Name Color G"].GetFloat(), document["Speaker Name Color B"].GetFloat(), 1.0f});
	}

	myAnimatedDialogue = new CTextInstance();
	myAnimatedDialogue->Init(textFactory->GetText(document["Dialogue Text Font and Size"].GetString()));
	myAnimatedDialogue->SetPivot({0.0f, 0.5f});
	myAnimatedDialogue->SetPosition({document["Dialogue Text Position X"].GetFloat(), document["Dialogue Text Position Y"].GetFloat()});
	myAnimatedDialogue->SetColor({document["Dialogue Text Color R"].GetFloat(), document["Dialogue Text Color G"].GetFloat(), document["Dialogue Text Color B"].GetFloat(), 1.0f});

	myAnimatedNarration = new CTextInstance();
	myAnimatedNarration->Init(textFactory->GetText(document["Narration Font and Size"].GetString()));
	myAnimatedNarration->SetPivot({ 0.0f, 0.5f });
	myAnimatedNarration->SetPosition({ document["Narration Text Position X"].GetFloat(), document["Narration Text Position Y"].GetFloat() });
	myAnimatedNarration->SetColor({ document["Narration Text Color R"].GetFloat(), document["Narration Text Color G"].GetFloat(), document["Narration Text Color B"].GetFloat(), 1.0f });

	myLineBreakDialogue = document["Dialogue Line Break After Characters"].GetInt();
	myLineBreakNarration = document["Narration Line Break After Characters"].GetInt();

	return true;
}

void CDialogueSystem::Receive(const SMessage& aMessage)
{
	switch (aMessage.myMessageType)
	{
	case EMessageType::LoadDialogue:
		LoadDialogue(*static_cast<int*>(aMessage.data));
		break;
	case EMessageType::IntroStarted:
		LoadNarration();
		break;
	default:
		break;
	}
}

void CDialogueSystem::Receive(const SStringMessage& aMessage)
{
	std::array<std::string, 3> dialogueScenes = { "DELevel1", "DELevel2", "DELevel3" };
	for (size_t i = 0; i < dialogueScenes.size(); ++i)
	{
		if (dialogueScenes[i] == std::string(aMessage.myMessageType))
		{
			LoadDialogue(static_cast<int>(i));
			break;
		}
	}
}

void CDialogueSystem::LoadDialogue(int aSceneIndex) {
	ExitDialogue();
	myDialogueBuffer.clear();
	myCurrentVoiceLine = -2;

	FILE* fp;
	fopen_s(&fp, "Json/DialogueTextLines.json", "rb");
	char read_buffer[200];
	rapidjson::FileReadStream is(fp, read_buffer, sizeof(read_buffer));

	rapidjson::Document doc;
	doc.ParseStream(is);

	fclose(fp);

	if (doc.HasMember("Scenes")) {
		const rapidjson::Value& object = doc["Scenes"][aSceneIndex]["Lines"].GetArray();
		assert(object.IsArray());
		for (unsigned int i = 0; i < object.Size(); ++i) {
			myDialogueBuffer.emplace_back(object[i]["Text"].GetString(), object[i]["Speaker Index"].GetInt(), object[i]["Speaker Name"].GetString(), object[i]["Voiceline Index"].GetInt());
		}
	}

	myIsActive = !myDialogueBuffer.empty();
}

void CDialogueSystem::LoadNarration()
{
	ExitDialogue();
	myDialogueBuffer.clear();
	myCurrentVoiceLine = -2;

	FILE* fp;
	fopen_s(&fp, "Json/DialogueTextLines.json", "rb");
	char read_buffer[200];
	rapidjson::FileReadStream is(fp, read_buffer, sizeof(read_buffer));

	rapidjson::Document doc;
	doc.ParseStream(is);

	fclose(fp);

	if (doc.HasMember("Narration")) {
		const rapidjson::Value& object = doc["Narration"].GetArray();
		assert(object.IsArray());
		for (unsigned int i = 0; i < object.Size(); ++i) {
			myDialogueBuffer.emplace_back(object[i]["Text"].GetString(), 0, "", object[i]["Voiceline Index"].GetInt());
		}
	}

	myIsActive = !myDialogueBuffer.empty();
	myIsNarration = myIsActive;
}

void CDialogueSystem::ExitDialogue() {
	myIsActive = false;
	myCurrentDialogueIndex = 0;
	myLastSpeakerIndex = -1;
	myLastDialogueIndex = -1;
	myHeldButtonTimer = 0.0f;
	myDialogueTimer = 0.0f;
	myLineBreakCounter = 0;

	myAnimatedDialogue->SetText("");
	myCurrentSpeakerName = nullptr;
	myCurrentSpeakerPortrait = nullptr;

	myEnabled = true;

	if (myIsNarration) {
		myIsNarration = false;
	}
}

void CDialogueSystem::Update() {
	//if (!myEnabled) {
	//	return;
	//}
	
	if (!myIsActive) {
		return;
	}

	if (myCurrentDialogueIndex < myDialogueBuffer.size()) {
		if (myDialogueBuffer[myCurrentDialogueIndex].mySpeakerNumber != myLastSpeakerIndex) {
			myLastSpeakerIndex = myDialogueBuffer[myCurrentDialogueIndex].mySpeakerNumber;
			myCurrentSpeakerPortrait = mySpeakerPortraits[myLastSpeakerIndex];
			myCurrentSpeakerName = mySpeakerNames[myLastSpeakerIndex];
			myCurrentSpeakerName->SetText(myDialogueBuffer[myCurrentDialogueIndex].mySpeakerName);
		}
	}

	HandleInput();
	
	if (myIsActive && (myCurrentDialogueIndex != myLastDialogueIndex))
	{
		CMainSingleton::PostMaster().Send({ EMessageType::PlayVoiceLine, &myDialogueBuffer[myCurrentDialogueIndex].myVoiceLine });
		ProcessLineBreaks();
	}

	if (myIsActive && (myCurrentDialogueIndex < myDialogueBuffer.size())) {

		myDialogueTimer += CTimer::Dt();
		int length = static_cast<int>(myDialogueBuffer[myCurrentDialogueIndex].myText.length());
		float percentage = myDialogueTimer / (myDialogueSpeed * length);
		int numberOfLetters = static_cast<int>(percentage * length);

		myCurrentLine.assign(myDialogueBuffer[myCurrentDialogueIndex].myText, 0, numberOfLetters);

		if (myIsNarration)
		{
			myAnimatedNarration->SetText(myCurrentLine);
		}
		else {
			myAnimatedDialogue->SetText(myCurrentLine);
		}

		myLastDialogueIndex = myCurrentDialogueIndex;
	}
}

void CDialogueSystem::EmplaceSprites(std::vector<CSpriteInstance*>& someSprites) const
{
	if (!myIsActive) {
		return;
	}

	if (!myIsNarration)
	{
		if (myDialogueBox)
			someSprites.emplace_back(myDialogueBox);

		if (myCurrentSpeakerPortrait)
			someSprites.emplace_back(myCurrentSpeakerPortrait);
	}
}

void CDialogueSystem::EmplaceTexts(std::vector<CTextInstance*>& someText) const
{
	if (!myIsActive) {
		return;
	}

	if (myIsNarration) {
		if (myAnimatedNarration)
			someText.emplace_back(myAnimatedNarration);
	}
	else {
		if (myAnimatedDialogue)
			someText.emplace_back(myAnimatedDialogue);

		if (myCurrentSpeakerName)
			someText.emplace_back(myCurrentSpeakerName);
	}
}

void CDialogueSystem::HandleInput() {
	if (!myIsActive) {
		return;
	}

	int length = static_cast<int>(myDialogueBuffer[myCurrentDialogueIndex].myText.length());
	if (Input::GetInstance()->IsKeyPressed(VK_SPACE) && myDialogueTimer >= (myDialogueSpeed * length)) {
		myCurrentDialogueIndex = static_cast<size_t>(myCurrentDialogueIndex + 1) % myDialogueBuffer.size();
		myDialogueTimer = 0.0f;
		myLineBreakCounter = 0;
		myCurrentLine = "";

		if (myCurrentDialogueIndex == 0) {
			CMainSingleton::PostMaster().Send({ EMessageType::StopDialogue, NULL });
			ExitDialogue();
		}
	}

	if (Input::GetInstance()->IsKeyDown(VK_SPACE)) {
		myHeldButtonTimer += CTimer::Dt();

		if (myHeldButtonTimer >= 0.3f) {
			SetDialogueSpeed(myDialogueFastSpeed, length);
		}
	}

	if (Input::GetInstance()->IsKeyReleased(VK_SPACE)) {
		myHeldButtonTimer = 0.0f;
		SetDialogueSpeed(myDialogueSlowSpeed, length);
	}

	// Needs to be controller by other states
	//if (Input::GetInstance()->IsKeyPressed(VK_ESCAPE)) {
	//	CMainSingleton::PostMaster().Send({ EMessageType::StopDialogue, NULL });
	//	ExitDialogue();
	//}
}

void CDialogueSystem::SetDialogueSpeed(float aSpeed, int aLineLength)
{
	float percentage = myDialogueTimer / (myDialogueSpeed * aLineLength);
	myDialogueSpeed = aSpeed;
	myDialogueTimer = percentage * (myDialogueSpeed * aLineLength);
}

void CDialogueSystem::ProcessLineBreaks()
{
	int lineBreak = (myIsNarration ? myLineBreakNarration : myLineBreakDialogue);
	myLineBreakCounter = 0;
	for (unsigned int i = 0; i < myDialogueBuffer[myCurrentDialogueIndex].myText.size(); ++i)
	{
		if (++myLineBreakCounter >= lineBreak)
		{
			char currentChar = myDialogueBuffer[myCurrentDialogueIndex].myText.at(i);
			if (currentChar == ' ') {
				myDialogueBuffer[myCurrentDialogueIndex].myText.insert((size_t)i + (size_t)1, "\n");
			} else {
				int backwardsCounter = 0;
				while (currentChar != ' ') {
					currentChar = myDialogueBuffer[myCurrentDialogueIndex].myText.at((size_t)i - (size_t)++backwardsCounter);
				}
				myDialogueBuffer[myCurrentDialogueIndex].myText.insert((size_t)i - (size_t)backwardsCounter + (size_t)1, "\n");
			}
			myLineBreakCounter = 0;
			i--;
		}
	}
}
