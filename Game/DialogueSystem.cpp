#include "stdafx.h"
#include "DialogueSystem.h"

#include <sstream>
#include <cstdio>

//#include <tga2d/text/text.h>
#include "TextFactory.h"
#include "TextInstance.h"
//#include <tga2d/sprite/sprite.h>
//#include <tga2d/math/color.h>
#include "SpriteFactory.h"
#include "SpriteInstance.h"
#include "Scene.h"

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

#include "Input.h"
#include <SpriteFont.h>
//#include "MainSingleton.hpp"
//#include "PostMaster.hpp"
//
//#include "SignComponent.hpp"

namespace SM = DirectX::SimpleMath;

CDialogueSystem::CDialogueSystem() {

	CSpriteFactory* factory = CSpriteFactory::GetInstance();
	myDialogueBox = new CSpriteInstance();
	myDialogueBox->Init(factory->GetSprite("Assets/3D/UI/Ingame/tempDialogueBox.dds"));
	myDialogueBox->SetSize({ 0.75f, 0.75f });
	myDialogueBox->SetPosition({ -0.0f, 0.0f });
	CScene::GetInstance()->AddInstance(myDialogueBox);

	mySpeakerPortraits.emplace_back(new CSpriteInstance());
	mySpeakerPortraits.back()->Init(factory->GetSprite("Assets/3D/UI/Ingame/tempSpeakerPortrait.dds"));
	//mySpeakerPortraits.back()->SetSize({ 128.0f / 1000.0f, 128.0f / 1000.0f });
	mySpeakerPortraits.back()->SetSize({ 0.75f, 0.75f });
	mySpeakerPortraits.back()->SetPosition({-0.30f, 0.57f});
	mySpeakerPortraits.back()->SetShouldRender(false);
	CScene::GetInstance()->AddInstance(mySpeakerPortraits.back());
	
	mySpeakerPortraits.emplace_back(new CSpriteInstance());
	mySpeakerPortraits.back()->Init(factory->GetSprite("Assets/3D/UI/Ingame/tempSpeakerPortrait2.dds"));
	mySpeakerPortraits.back()->SetSize({ 0.75f, 0.75f });
	mySpeakerPortraits.back()->SetPosition({ -0.30f, 0.57f });
	mySpeakerPortraits.back()->SetShouldRender(false);
	CScene::GetInstance()->AddInstance(mySpeakerPortraits.back());

	CTextFactory* textFactory = CTextFactory::GetInstance();
	myDialogueLine = new CTextInstance();
	myDialogueLine->Init(textFactory->GetText("baskerville16"));
	myDialogueLine->SetPosition({ 500.0f, 500.0f });

	myAnimatedDialogue = new CTextInstance();
	myAnimatedDialogue->Init(textFactory->GetText("baskerville16"));
	myAnimatedDialogue->SetPosition({ 500.0f, 500.0f });
	
	CScene::GetInstance()->AddInstance(myAnimatedDialogue);

	LoadDialogue("Test");
}

CDialogueSystem::~CDialogueSystem() {
	delete myDialogueBox;
	myDialogueBox = nullptr;
}

void CDialogueSystem::LoadDialogue(const char* scene_name) {
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

	if (doc.HasMember(scene_name)) {
		const rapidjson::Value& object = doc[scene_name];
		assert(object.IsArray());
		for (unsigned int i = 0; i < object.Size(); ++i) {
			myDialogueBuffer.emplace_back(object[i]["text"].GetString(), object[i]["speaker"].GetInt(), object[i]["voiceline"].GetInt());
		}
	}

	myIsActive = !myDialogueBuffer.empty();
	myDialogueBox->SetShouldRender(true);
}

void CDialogueSystem::LoadInfo(const char* info) {
	ExitDialogue();
	myDialogueBuffer.clear();
	myCurrentVoiceLine = -2;

	FILE* fp;
	fopen_s(&fp, "Json/info_text_lines.json", "rb");
	char read_buffer[200];
	rapidjson::FileReadStream is(fp, read_buffer, sizeof(read_buffer));

	rapidjson::Document doc;
	doc.ParseStream(is);

	fclose(fp);

	if (doc.HasMember(info)) {
		const rapidjson::Value& object = doc[info];
		assert(object.IsArray());
		for (unsigned int i = 0; i < object.Size(); ++i) {
			myDialogueBuffer.emplace_back(object[i]["text"].GetString(), object[i]["speaker"].GetInt(), object[i]["voiceline"].GetInt());
		}
	}

	myIsActive = !myDialogueBuffer.empty();
	myIsInfo = true;
}

void CDialogueSystem::LoadScroll(const char* scroll) {
	ExitDialogue();
	myDialogueBuffer.clear();
	myCurrentVoiceLine = -2;

	FILE* fp;
	fopen_s(&fp, "Json/scroll_text_lines.json", "rb");
	char read_buffer[200];
	rapidjson::FileReadStream is(fp, read_buffer, sizeof(read_buffer));

	rapidjson::Document doc;
	doc.ParseStream(is);

	fclose(fp);

	if (doc.HasMember(scroll)) {
		const rapidjson::Value& object = doc[scroll];
		assert(object.IsArray());
		for (unsigned int i = 0; i < object.Size(); ++i) {
			myDialogueBuffer.emplace_back(object[i]["text"].GetString(), object[i]["speaker"].GetInt(), object[i]["voiceline"].GetInt());
		}
	}

	myIsActive = !myDialogueBuffer.empty();
	myIsInventory = true;
}

void CDialogueSystem::ExitDialogue() {
	myIsActive = false;
	myCurrentDialogueIndex = 0;
	myLastSpeakerIndex = -1;
	myHeldButtonTimer = 0.0f;
	myDialogueTimer = 0.0f;
	myLineBreakCounter = 0;
	myLettersLastFrame = 0;
	if (myIsInventory) {
		myIsInventory = false;
	} else if (myIsInfo) {
		myIsInfo = false;
	}

	myDialogueBox->SetShouldRender(false);
	for (unsigned int i = 0; i < mySpeakerPortraits.size(); ++i) {
		mySpeakerPortraits[i]->SetShouldRender(false);
	}
}

void CDialogueSystem::Update(float delta_time) {
	if (!myIsActive) {
		return;
	}

	// The load_scene() function should be called before using the CDialogueSystem!
	//assert(!myDialogueBuffer.empty());
	if (myCurrentDialogueIndex < myDialogueBuffer.size()) {
		// Speaker change message
		if (myDialogueBuffer[myCurrentDialogueIndex].mySpeakerNumber != myLastSpeakerIndex) {
			if (myLastSpeakerIndex >= 0) {
				mySpeakerPortraits[myLastSpeakerIndex]->SetShouldRender(false);
			}
			myLastSpeakerIndex = myDialogueBuffer[myCurrentDialogueIndex].mySpeakerNumber;
			mySpeakerPortraits[myLastSpeakerIndex]->SetShouldRender(true);
		}
	}

	HandleInput(delta_time);

	if (myCurrentDialogueIndex < myDialogueBuffer.size()) {

		myDialogueTimer += delta_time;
		int length = static_cast<int>(myDialogueBuffer[myCurrentDialogueIndex].myText.length());
		float percentage = myDialogueTimer / (myDialogueSpeed * length);
		int number_of_letters = static_cast<int>(percentage * length);
		myDialogueLine->SetText(myDialogueBuffer[myCurrentDialogueIndex].myText);

#pragma region Line breaks
		if (myLettersLastFrame != number_of_letters) {
			myLineBreakCounter++;
		}

		int line_break = (myIsInfo ? myLineBreakInfo : myLineBreakDialogue);
		if (myCurrentDialogueIndex < myDialogueBuffer.size()) {
			if (myLineBreakCounter >= line_break) {
				if (number_of_letters < myDialogueBuffer[myCurrentDialogueIndex].myText.length()) {
					char current_char = myDialogueBuffer[myCurrentDialogueIndex].myText.at(number_of_letters);
					if (current_char == ' ') {
						myDialogueBuffer[myCurrentDialogueIndex].myText.insert((size_t) number_of_letters + (size_t) 1, "\n");
					} else {
						int backwards_counter = 0;
						while (current_char != ' ') {
							current_char = myDialogueBuffer[myCurrentDialogueIndex].myText.at((size_t) number_of_letters - (size_t)++backwards_counter);
						}
						myDialogueBuffer[myCurrentDialogueIndex].myText.insert((size_t) number_of_letters - (size_t) backwards_counter + (size_t) 1, "\n");
					}
				}
				myLineBreakCounter = 0;
			}
		}
#pragma endregion

		myCurrentLine.assign(myDialogueBuffer[myCurrentDialogueIndex].myText, 0, number_of_letters);

		myAnimatedDialogue->SetText(myCurrentLine);


#pragma region Vertical offset
		myTextPosOffset = (myIsInfo ? myOffsetInfo : myOffsetDialogue) * (length / (myIsInfo ? myLineBreakInfo : myLineBreakDialogue));
#pragma endregion

		myLettersLastFrame = number_of_letters;
	}
}

const int CDialogueSystem::CurrentSpeaker() const {
	return myDialogueBuffer[myCurrentDialogueIndex].mySpeakerNumber;
}

void CDialogueSystem::HandleInput(float delta_time) {
	if (!myIsActive) {
		return;
	}

	int length = static_cast<int>(myDialogueBuffer[myCurrentDialogueIndex].myText.length());
	if (Input::GetInstance()->IsKeyPressed(VK_SPACE) && myDialogueTimer >= (myDialogueSpeed * length)) {
		if (!myIsInventory) {
			myCurrentDialogueIndex = static_cast<size_t>(myCurrentDialogueIndex + 1) % myDialogueBuffer.size();
			myDialogueTimer = 0.0f;
			myLineBreakCounter = 0;
			myLettersLastFrame = 0;
			myCurrentLine = "";

			if (myCurrentDialogueIndex == 0) {
				ExitDialogue();
			}
		}
	}

	if (Input::GetInstance()->IsKeyDown(VK_SPACE)) {
		myHeldButtonTimer += delta_time;

		if (myHeldButtonTimer >= 0.3f) {
			SetDialogueSpeed(0.01f, length);
		}
	}

	if (Input::GetInstance()->IsKeyReleased(VK_SPACE)) {
		myHeldButtonTimer = 0.0f;
		SetDialogueSpeed(0.05f, length);
	}

	if (Input::GetInstance()->IsKeyPressed(VK_RETURN)) {
		if (!myIsInventory) {
			ExitDialogue();
		}
	}
}

void CDialogueSystem::SetDialogueSpeed(float aSpeed, int aLineLength)
{
	float percentage = myDialogueTimer / (myDialogueSpeed * aLineLength);
	myDialogueSpeed = aSpeed;
	myDialogueTimer = percentage * (myDialogueSpeed * aLineLength);
}
