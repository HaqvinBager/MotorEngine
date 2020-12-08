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


	/*{ "Path": "castle1.mp3" },
	{ "Path": "cave1.mp3" },
	{ "Path": "cave2.mp3" },
	{ "Path": "dungeon1.mp3" },
	{ "Path": "gardenday1.mp3" },
	{ "Path": "swampnight1.mp3" },
	{ "Path": "swampnight2.mp3" }*/

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

	//// Init Music
	//for (unsigned int i = 0; i < static_cast<unsigned int>(EMusic::Count); ++i) {
	//	myMusicAudio.emplace_back(myWrapper.RequestSound(GetPath(static_cast<EMusic>(i))));
	//}	

	// Set starting volume
	for (auto& channel : myChannels) {
		channel->SetVolume(0.1f);
	}

	// SEND MESSAGE TO START PLAYING MUSIC

	//CMainSingleton::PostMaster().Send({ EMessageType::MainMenu, NULL });

	//CMainSingleton::PostMaster().Send({ EMessageType::EnemyHealthChanged, NULL });

	//CMainSingleton::PostMaster().Send({ EMessageType::PlayAmbience, NULL });


}

CAudioManager::~CAudioManager()
{
	UnsubscribeToMessages();
}




void CAudioManager::Receive(const SMessage& aMessage) {
	switch (aMessage.myMessageType)
	{
	case EMessageType::Resume:
	{
		myWrapper.Play(myAmbianceAudio[CAST(EAmbiance::Castle)], myChannels[CAST(EChannels::Ambiance)]);
	}
	break;
	case EMessageType::MainMenu:
	{
		myWrapper.Play(myMusicAudio[CAST(EMusic::MainMenu)], myChannels[CAST(EChannels::Music)]);
	}
	break;
	case EMessageType::EnemyHealthChanged:
	{
		myWrapper.Play(mySFXAudio[CAST(ESFX::EnemyPain)], myChannels[CAST(EChannels::SFX)]);
	}
	break;


	default:
		break;
	}
}


//void CAudioManager::SubscribeToMusic()
//{
//	for (auto messageType : myMusics)
//	{
//		CMainSingleton::PostMaster().Subscribe(messageType, this);
//	}
//
//
//	CMainSingleton::PostMaster().Subscribe(EMessageType::MainMenu, this);
//}



void CAudioManager::SubscribeToMessages()
{
	//CMainSingleton::PostMaster().Subscribe(EMessageType::PlayAmbience, this);

	CMainSingleton::PostMaster().Subscribe(EMessageType::MainMenu, this);
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
