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
			myMusicAudio.emplace_back(myWrapper.RequestSound(myMusicPath + audioData["Path"].GetString()));

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

	// Set starting volume
	for (auto& channel : myChannels) {
		channel->SetVolume(0.1f);
	}

	// SEND MESSAGE TO START PLAYING MUSIC

	//CMainSingleton::PostMaster().Send({ EMessageType::MainMenu, NULL });

	//CMainSingleton::PostMaster().Send({ EMessageType::EnemyHealthChanged, NULL });

	CMainSingleton::PostMaster().Send({ EMessageType::PlayAmbienceCave1, NULL });


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
	case EMessageType::MainMenu:
	{
	    //myWrapper.Play(myMusicAudio[CAST(EMusic::MainMenu)], myChannels[CAST(EChannels::Music)]);
	}
		break;
	case EMessageType::PlayAmbienceCastle:
	{
		//myWrapper.Play(myAmbianceAudio[CAST(EAmbiance::Castle)], myChannels[CAST(EChannels::Ambiance)]);
	}
		break;
	case EMessageType::PlayAmbienceCave1:
	{
		//myWrapper.Play(myAmbianceAudio[CAST(EAmbiance::Cave1)], myChannels[CAST(EChannels::Ambiance)]);
	}
		break;

	case EMessageType::PlayAmbienceDungeon:
	{
		//myWrapper.Play(myAmbianceAudio[CAST(EAmbiance::Castle)], myChannels[CAST(EChannels::Ambiance)]);
	}
		break;

	case EMessageType::PlayAmbienceSwamp1:
	{
		//myWrapper.Play(myAmbianceAudio[CAST(EAmbiance::Castle)], myChannels[CAST(EChannels::Ambiance)]);
	}
		break;
	
	case EMessageType::EnemyHealthChanged:
	{
		//myWrapper.Play(mySFXAudio[CAST(ESFX::EnemyPain)], myChannels[CAST(EChannels::SFX)]);
	}
	break;


	default:
		break;
	}
}


void CAudioManager::Update()
{

}

void CAudioManager::SubscribeToMessages()
{

	CMainSingleton::PostMaster().Subscribe(EMessageType::MainMenu, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::PlayAmbienceCastle, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::PlayAmbienceCave1, this);

	CMainSingleton::PostMaster().Subscribe(EMessageType::EnemyHealthChanged, this);

}

void CAudioManager::UnsubscribeToMessages()
{
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::MainMenu, this);
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
	case ESFX::EnemyPain:
		return "EnemyPain";
	default:
		return "";
	}
}
std::string CAudioManager::TranslateUI(EUI enumerator) const {
	switch (enumerator) {
	case EUI::Count:
		return "";
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
