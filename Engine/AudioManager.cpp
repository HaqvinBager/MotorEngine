#include "stdafx.h"
#include <iostream>
#include <random>
#include "AudioManager.h"
#include "Audio.h"
#include "AudioChannel.h"
#include "MainSingleton.h"
#include "PostMaster.h"
#include "rapidjson\document.h"
#include "rapidjson\istreamwrapper.h"



using namespace rapidjson;

#define CAST(type) { static_cast<unsigned int>(type) }

CAudioManager::CAudioManager() : myWrapper() {
	SubscribeToMessages();

	std::ifstream inputStream("Json/AudioPaths.json");
	IStreamWrapper inputWrapper(inputStream);
	Document document;
	document.ParseStream(inputWrapper);

	if (document.HasParseError()) { return; }

	// Init Channels
	for (unsigned int i = 0; i < static_cast<unsigned int>(EChannels::Count); ++i) {
		myChannels.emplace_back(myWrapper.RequestChannel(TranslateChannels(static_cast<EChannels>(i))));
	}


#pragma region AudioLoading


	if (document.HasMember("Ambience"))
	{
		auto audioDataArray = document["Ambience"].GetArray();
		for (unsigned int i = 0; i < audioDataArray.Size(); ++i)
		{
			auto audioData = audioDataArray[i].GetObjectW();
			myAmbianceAudio.emplace_back(myWrapper.RequestSound(myAmbiancePath + audioData["Path"].GetString()));

		}

	}

	if (document.HasMember("Music"))
	{
		auto audioDataArray = document["Music"].GetArray();
		for (unsigned int i = 0; i < audioDataArray.Size(); ++i)
		{
			auto audioData = audioDataArray[i].GetObjectW();
			myMusicAudio.emplace_back(myWrapper.RequestLoopingSound(myMusicPath + audioData["Path"].GetString()));

		}

	}

	if (document.HasMember("SFX"))
	{
		auto audioDataArray = document["SFX"].GetArray();
		for (unsigned int i = 0; i < audioDataArray.Size(); ++i)
		{
			auto audioData = audioDataArray[i].GetObjectW();
			mySFXAudio.emplace_back(myWrapper.RequestSound(mySFXPath + audioData["Path"].GetString()));

		}

	}

	if (document.HasMember("UI"))
	{
		auto audioDataArray = document["UI"].GetArray();
		for (unsigned int i = 0; i < audioDataArray.Size(); ++i)
		{
			auto audioData = audioDataArray[i].GetObjectW();
			myUIAudio.emplace_back(myWrapper.RequestSound(myUIPath + audioData["Path"].GetString()));

		}

	}

	if (document.HasMember("VoiceLine"))
	{
		auto audioDataArray = document["VoiceLine"].GetArray();
		for (unsigned int i = 0; i < audioDataArray.Size(); ++i)
		{
			auto audioData = audioDataArray[i].GetObjectW();
			myVoicelineAudio.emplace_back(myWrapper.RequestSound(myVoxPath + audioData["Path"].GetString()));

		}

	}

#pragma endregion

	// Set starting volume
	//for (auto& channel : myChannels) 
	//{
	//	channel->SetVolume(0.1f);
	//}

	// Add JSON reading?
	// Group 4 
	myChannels[CAST(EChannels::Music)]->SetVolume(0.3f);
	myChannels[CAST(EChannels::Ambiance)]->SetVolume(0.5f);
	myChannels[CAST(EChannels::SFX)]->SetVolume(0.15f);
	myChannels[CAST(EChannels::UI)]->SetVolume(0.4f);
	myChannels[CAST(EChannels::VOX)]->SetVolume(0.5f);

	// Unused?
	// SEND MESSAGE TO START PLAYING MUSIC
	//CMainSingleton::PostMaster().Send({ EMessageType::MainMenu, NULL });
	//CMainSingleton::PostMaster().Send({ EMessageType::EnemyHealthChanged, NULL });
	//CMainSingleton::PostMaster().Send({ EMessageType::PlayAmbienceCastle, NULL });
	//CMainSingleton::PostMaster().Send({ EMessageType::BossDied, NULL });
	//CMainSingleton::PostMaster().Send({ EMessageType::UIButtonPress, NULL });
}

	CAudioManager::~CAudioManager()
	{
		UnsubscribeToMessages();
		// 2020 12 06 - CAudio attempts to delete myFModSound, seems to be shared. 
		//for (auto& channel : myChannels)
		//{
		//	delete channel;
		//	channel = nullptr;
		//}
		//for (auto& music : myMusicAudio)
		//{
		//	delete music;
		//	music = nullptr;
		//}
		//for (auto& ambience : myAmbianceAudio)
		//{
		//	delete ambience;
		//	ambience = nullptr;
		//}
		//for (auto& sfx : mySFXAudio)
		//{
		//	delete sfx;
		//	sfx = nullptr;
		//}
		//for (auto& ui : myUIAudio)
		//{
		//	delete ui;
		//	ui = nullptr;
		//}
		//for (auto& voice : myVoicelineAudio)
		//{
		//	delete voice;
		//	voice = nullptr;
		//}
	}


void CAudioManager::Receive(const SMessage& aMessage) {
	switch (aMessage.myMessageType)
	{
	// MUSIC

	case EMessageType::PlayDungeonMusic:
	{
		if (myMusicAudio.size() >= static_cast<unsigned int>(EMusic::Dungeon))
			myWrapper.Play(myMusicAudio[CAST(EMusic::Dungeon)], myChannels[CAST(EChannels::Music)]);
	}break;

	case EMessageType::PlayGardensMusic:
	{
		if (myMusicAudio.size() >= static_cast<unsigned int>(EMusic::Garden))
			myWrapper.Play(myMusicAudio[CAST(EMusic::Garden)], myChannels[CAST(EChannels::Music)]);
	}break;

	case EMessageType::PlayCastleMusic:
	{
		if (myMusicAudio.size() >= static_cast<unsigned int>(EMusic::Castle))
			myWrapper.Play(myMusicAudio[CAST(EMusic::Castle)], myChannels[CAST(EChannels::Music)]);
	}break;

	case EMessageType::BossFightStart:
	{
		if (myMusicAudio.size() >= static_cast<unsigned int>(EMusic::BossEncounter))
			myWrapper.Play(myMusicAudio[CAST(EMusic::BossEncounter)], myChannels[CAST(EChannels::Music)]);
	}break;

	case EMessageType::PlayMainMenuMusic:
	{
		if (myMusicAudio.size() >= static_cast<unsigned int>(EMusic::MainMenu))
			myWrapper.Play(myMusicAudio[CAST(EMusic::MainMenu)], myChannels[CAST(EChannels::Music)]);
	}break;

	case EMessageType::StopMusic:
	{
		if (!myMusicAudio.empty()) {
			myChannels[CAST(EChannels::Music)]->Stop();
		}

		if (!myAmbianceAudio.empty()) {
			myChannels[CAST(EChannels::Ambiance)]->Stop();
		}
	}break;

	case EMessageType::PlayAmbienceCastle:
	{
		myWrapper.Play(myAmbianceAudio[0], myChannels[CAST(EChannels::Ambiance)]);
		//if (myAmbianceAudio.size() >= static_cast<unsigned int>(EAmbiance::Castle)) 
		//{
		//}
	}break;

	case EMessageType::PlayAmbienceCave1:
	{
		if (myAmbianceAudio.size() >= static_cast<unsigned int>(EAmbiance::Cave1))
		{
			myWrapper.Play(myAmbianceAudio[CAST(EAmbiance::Cave1)], myChannels[CAST(EChannels::Ambiance)]);
		}
		//myWrapper.Play(myAmbianceAudio[CAST(EAmbiance::Cave1)], myChannels[CAST(EChannels::Ambiance)])
	}break;

	case EMessageType::PlayAmbienceDungeon:
	{
		myWrapper.Play(myAmbianceAudio[1], myChannels[CAST(EChannels::Ambiance)]);
		//if (myAmbianceAudio.size() >= static_cast<unsigned int>(EAmbiance::Dungeon))
		//{
		//}
	}break;

	case EMessageType::PlayAmbienceGarden:
	{
		myWrapper.Play(myAmbianceAudio[2], myChannels[CAST(EChannels::Ambiance)]);
		//if (myAmbianceAudio.size() >= static_cast<unsigned int>(EAmbiance::Garden))
		//{
		//}
	}break;

	case EMessageType::PlayAmbienceSwamp1:
	{
		if (myAmbianceAudio.size() >= static_cast<unsigned int>(EAmbiance::Swamp1))
		{
			myWrapper.Play(myAmbianceAudio[CAST(EAmbiance::Swamp1)], myChannels[CAST(EChannels::Ambiance)]);
		}
	}break;
	
	// SFX
	case EMessageType::AttackHits:
	{
		if (mySFXAudio.size() >= static_cast<unsigned int>(ESFX::PlayerHitSound))
		{
			myWrapper.Play(mySFXAudio[CAST(ESFX::PlayerHitSound)], myChannels[CAST(EChannels::SFX)]);
		}
	}break;

	case EMessageType::PlayBossDeathSFX:
	{
		if (mySFXAudio.size() >= static_cast<unsigned int>(ESFX::KingBossDeathSound))
		{
			myWrapper.Play(mySFXAudio[CAST(ESFX::KingBossDeathSound)], myChannels[CAST(EChannels::SFX)]);
		}
	}break;

	case EMessageType::PlayBossExplosionSFX:
	{
		if (aMessage.data != nullptr)
		{
			SDelayedSFX sfx = { ESFX::BossExplosion, *static_cast<float*>(aMessage.data) };
			myDelayedSFX.emplace_back(sfx);
		}else if (mySFXAudio.size() >= static_cast<unsigned int>(ESFX::BossExplosion))
		{
			myWrapper.Play(mySFXAudio[CAST(ESFX::BossExplosion)], myChannels[CAST(EChannels::SFX)]);
		}
	}break;

	case EMessageType::DemonIdle1:
	{
		if (mySFXAudio.size() >= static_cast<unsigned int>(ESFX::DemonIdle1))
		{
			myWrapper.Play(mySFXAudio[CAST(ESFX::DemonIdle1)], myChannels[CAST(EChannels::SFX)]);
		}
	}break;

	case EMessageType::DemonIdle2:
	{
		if (mySFXAudio.size() >= static_cast<unsigned int>(ESFX::DemonIdle2))
		{
			myWrapper.Play(mySFXAudio[CAST(ESFX::DemonIdle2)], myChannels[CAST(EChannels::SFX)]);
		}
	}break;

	case EMessageType::HitDestructible:
	{
		if (mySFXAudio.size() >= static_cast<unsigned int>(ESFX::HitDestructible))
		{
			myWrapper.Play(mySFXAudio[CAST(ESFX::HitDestructible)], myChannels[CAST(EChannels::SFX)]);
		}
	}break;

	case EMessageType::PlayLevelUpSFX:
	{
		if (mySFXAudio.size() >= static_cast<unsigned int>(ESFX::LevelUp))
		{
			myWrapper.Play(mySFXAudio[CAST(ESFX::LevelUp)], myChannels[CAST(EChannels::SFX)]);
		}
	}break;

	case EMessageType::HealingAura:
	{
		if (mySFXAudio.size() >= static_cast<unsigned int>(ESFX::HealingAura))
		{
			myWrapper.Play(mySFXAudio[CAST(ESFX::HealingAura)], myChannels[CAST(EChannels::SFX)]);
		}
	}break;

	case EMessageType::LightAttack:
	{
		if (aMessage.data != nullptr)
		{
			SDelayedSFX sfx = { ESFX::PlayerLightAtk, *static_cast<float*>(aMessage.data) };
			myDelayedSFX.emplace_back(sfx);
		}else if (mySFXAudio.size() >= static_cast<unsigned int>(ESFX::PlayerLightAtk))
		{
			myWrapper.Play(mySFXAudio[CAST(ESFX::PlayerLightAtk)], myChannels[CAST(EChannels::SFX)]);
		}
	}break;

	case EMessageType::HeavyAttack:
	{
		if (aMessage.data != nullptr)
		{
			SDelayedSFX sfx = { ESFX::PlayerHeavyAtk, *static_cast<float*>(aMessage.data) };
			myDelayedSFX.emplace_back(sfx);
		}else if (mySFXAudio.size() >= static_cast<unsigned int>(ESFX::PlayerHeavyAtk))
		{
			myWrapper.Play(mySFXAudio[CAST(ESFX::PlayerHeavyAtk)], myChannels[CAST(EChannels::SFX)]);
		}
	}break;

	case EMessageType::ShieldSpell:
	{
		if (aMessage.data != nullptr)
		{
			SDelayedSFX sfx = { ESFX::ShieldSpell, *static_cast<float*>(aMessage.data) };
			myDelayedSFX.emplace_back(sfx);
		}else if (mySFXAudio.size() >= static_cast<unsigned int>(ESFX::ShieldSpell))
		{
			myWrapper.Play(mySFXAudio[CAST(ESFX::ShieldSpell)], myChannels[CAST(EChannels::SFX)]);
		}
	}break;

	case EMessageType::PlayExplosionSFX:
	{
		if (static_cast<float*>(aMessage.data) != nullptr)
		{
			SDelayedSFX sfx = { ESFX::Explosion, *static_cast<float*>(aMessage.data) };
			myDelayedSFX.emplace_back(sfx);
		}else if (mySFXAudio.size() >= static_cast<unsigned int>(ESFX::Explosion))
		{
			myWrapper.Play(mySFXAudio[CAST(ESFX::Explosion)], myChannels[CAST(EChannels::SFX)]);
		}
	}break;

	// UI
	case EMessageType::UIButtonPress:
	{

		if (myUIAudio.size() >= static_cast<unsigned int>(EUI::ButtonClick))
		{
			myWrapper.Play(myUIAudio[CAST(EUI::ButtonClick)], myChannels[CAST(EChannels::UI)]);
		}
	}break;

	// VOICELINES
	case EMessageType::PlayVoiceLine:
	{
		if (!myVoicelineAudio.empty()) {
			int index = *static_cast<int*>(aMessage.data);
			myChannels[CAST(EChannels::VOX)]->Stop();
			myWrapper.Play(myVoicelineAudio[index], myChannels[CAST(EChannels::VOX)]);
		}
	}break;

	case EMessageType::StopDialogue:
	{
		if (!myVoicelineAudio.empty()) {
			myChannels[CAST(EChannels::VOX)]->Stop();
		}
	}break;

	default: break;
	}
}

void CAudioManager::Receive(const SStringMessage& /*aMessage*/)
{
	//if(aMessage.myMessageType == "")
}


void CAudioManager::Update()
{
	if (myDelayedSFX.size() > 0)
	{
		const float dt = CTimer::Dt();

		for (auto it = myDelayedSFX.begin(); it != myDelayedSFX.end();)
		{
			it->myTimer -= dt;
			if (it->myTimer <= 0.0f)
			{
				myWrapper.Play(mySFXAudio[CAST(it->mySFX)], myChannels[CAST(EChannels::SFX)]);
				it = myDelayedSFX.erase(it);
				continue;
			}
			++it;
		}
	}
}

void CAudioManager::SubscribeToMessages()
{
	CMainSingleton::PostMaster().Subscribe(EMessageType::PlayMainMenuMusic, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::PlayDungeonMusic, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::PlayGardensMusic, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::PlayCastleMusic, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::BossFightStart, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::StopMusic, this);

	CMainSingleton::PostMaster().Subscribe(EMessageType::PlayAmbienceCastle, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::PlayAmbienceCave1, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::PlayAmbienceDungeon, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::PlayAmbienceGarden, this);
	
	CMainSingleton::PostMaster().Subscribe(EMessageType::AttackHits, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::PlayBossDeathSFX, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::BossMeleeAttack, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::PlayBossExplosionSFX, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::DemonIdle1, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::DemonIdle2, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::HitDestructible, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::PlayLevelUpSFX, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::HealingAura, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::LightAttack, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::HeavyAttack, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::ShieldSpell, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::PlayExplosionSFX, this);

	CMainSingleton::PostMaster().Subscribe(EMessageType::UIButtonPress, this);

	CMainSingleton::PostMaster().Subscribe(EMessageType::PlayVoiceLine, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::StopDialogue, this);
}

void CAudioManager::UnsubscribeToMessages()
{
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::PlayMainMenuMusic, this);
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::PlayDungeonMusic, this);
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::PlayGardensMusic, this);
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::PlayCastleMusic, this);
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::BossFightStart, this);
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::StopMusic, this);

	CMainSingleton::PostMaster().Unsubscribe(EMessageType::PlayAmbienceCastle, this);
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::PlayAmbienceCave1, this);
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::PlayAmbienceDungeon, this);
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::PlayAmbienceGarden, this);

	CMainSingleton::PostMaster().Unsubscribe(EMessageType::AttackHits, this);
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::PlayBossDeathSFX, this);
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::BossMeleeAttack, this);
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::PlayBossExplosionSFX, this);
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::DemonIdle1, this);
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::DemonIdle2, this);
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::HitDestructible, this);
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::PlayLevelUpSFX, this);
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::HealingAura, this);
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::LightAttack, this);
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::HeavyAttack, this);
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::ShieldSpell, this);
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::PlayExplosionSFX, this);

	CMainSingleton::PostMaster().Unsubscribe(EMessageType::UIButtonPress, this);

	CMainSingleton::PostMaster().Unsubscribe(EMessageType::PlayVoiceLine, this);
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::StopDialogue, this);
}

std::string CAudioManager::GetPath(EMusic type) const
{
	std::string path = myMusicPath;
	path.append(TranslateMusic(type));
	path.append(".mp3");
	return path;
}

std::string CAudioManager::GetPath(EAmbiance type) const
{
	std::string path = myAmbiancePath;
	path.append(TranslateAmbiance(type));
	path.append(".mp3");
	return path;
}

std::string CAudioManager::GetPath(ESFX type) const
{
	std::string path = mySFXPath;
	path.append(TranslateSFX(type));
	path.append(".wav");
	return path;
}

std::string CAudioManager::GetPath(EUI type) const
{
	std::string path = myUIPath;
	path.append(TranslateUI(type));
	path.append(".wav");
	return path;
}

std::string CAudioManager::GetPath(EVoiceLine type) const
{
	std::string path = myVoxPath;
	path.append(TranslateVoiceLine(type));
	path.append(".mp3");
	return path;
}

std::string CAudioManager::TranslateChannels(EChannels enumerator) const
{
	switch (enumerator)
	{
	case EChannels::Music:
		return "Music";
	case EChannels::Ambiance:
		return "Ambiance";
	case EChannels::SFX:
		return "SFX";
	case EChannels::UI:
		return "UI";
	case EChannels::VOX:
		return "VOX";
	default:
		return "";
	}
}

std::string CAudioManager::TranslateMusic(EMusic enumerator) const
{
	switch (enumerator) {
	case EMusic::MainMenu:
		return "MainMenu";
	default:
		return "";
	}
}

std::string CAudioManager::TranslateAmbiance(EAmbiance enumerator) const {
	switch (enumerator) {
	case EAmbiance::Castle:
		return "Castle";
		break;
	case EAmbiance::Cave1:
		return "Cave1";
	default:
		return "";
	}
}
std::string CAudioManager::TranslateSFX(ESFX enumerator) const {
	switch (enumerator) {
	
	case ESFX::PlayerHitSound:
		return "PlayerHitSound";
		break;
	case ESFX::KingBossDeathSound:
		return "KingBossDeathSound";
		break;
	case ESFX::BossMeleeAtk:
		return "BossMeleeAtk";
		break;
	case ESFX::DemonIdle1:
		return "DemonIdle1";
		break;
	case ESFX::DemonIdle2:
		return "DemonIdle2";
		break;
	case ESFX::HitDestructible:
		return "HitDestructible";
		break;
	case ESFX::HealingAura:
		return "HealingAura";
		break;
	case ESFX::PlayerLightAtk:
		return "PlayerLightAtk";
		break;
	case ESFX::PlayerHeavyAtk:
		return "PlayerHeavyAtk";
		break;
	case ESFX::ShieldSpell:
		return "ShieldSpell";
		break;

	default:
		return "";
	}
}
std::string CAudioManager::TranslateUI(EUI enumerator) const {
	switch (enumerator) {
	case EUI::ButtonClick:
		return "ButtonClick";
	default:
		return "";
	}
}
std::string CAudioManager::TranslateVoiceLine(EVoiceLine enumerator) const {
	switch (enumerator) {
	case EVoiceLine::Count:
		return "";
	default:
		return "";
	}
}
