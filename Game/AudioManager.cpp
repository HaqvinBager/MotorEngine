#include "stdafx.h"
#include <iostream>
#include <random>
#include "AudioManager.h"
#include "Audio.h"
#include "AudioChannel.h"
#include "MainSingleton.h"
#include "PostMaster.h"

#define CAST(type) { static_cast<unsigned int>(type) }

CAudioManager::CAudioManager() : myWrapper() {
	SubscribeToMessages();

	// Init Channels
	for (unsigned int i = 0; i < static_cast<unsigned int>(EChannels::Count); ++i) {
		myChannels.emplace_back(myWrapper.RequestChannel(TranslateChannels(static_cast<EChannels>(i))));
	}

	// Init Music
	for (unsigned int i = 0; i < static_cast<unsigned int>(EMusic::Count); ++i) {
		myMusicAudio.emplace_back(myWrapper.RequestSound(GetPath(static_cast<EMusic>(i))));
	}	

	// Set starting volume
	for (auto& channel : myChannels) {
		channel->SetVolume(0.1f);
	}

	// SEND MESSAGE TO START PLAYING MUSIC
	//CMainSingleton::PostMaster().Send({ EMessageType::MainMenu, NULL });
}

CAudioManager::~CAudioManager() 
{
	UnsubscribeToMessages();
}

void CAudioManager::Receive(const SMessage& aMessage) {
	switch (aMessage.myMessageType)
	{
	case EMessageType::MainMenu:
	{
		myWrapper.Play(myMusicAudio[CAST(EMusic::MainMenu)], myChannels[CAST(EChannels::Music)]);
	}
		break;
	default:
		break;
	}
}

void CAudioManager::SubscribeToMessages()
{
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
	case EAmbiance::Count:
		return "";
	default:
		return "";
	}
}
std::string CAudioManager::TranslateSFX(ESFX enumerator) const {
	switch (enumerator) {
	case ESFX::Count:
		return "";
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
