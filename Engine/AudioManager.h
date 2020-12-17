#pragma once
#pragma once
#include <vector>
#include <random>
#include <memory>
#include "FModWrapper.h"
#include "Observer.h"

class CAudio;
class CAudioChannel;

enum class EMusic 
{ 
	BossEncounter, 
	Castle,
	Dungeon,
	Garden,
	MainMenu, 
	Count 
};
enum class EAmbiance { Castle, Cave1, Cave2,  Dungeon, Garden, Swamp1, Swamp2, Count };
enum class ESFX 
{ 
	PlayerHitSound,
	KingBossDeathSound, 
	BossMeleeAtk, 
	BossExplosion, 
	DemonIdle1, 
	DemonIdle2, 
	Explosion,
	HitDestructible, 
	HealingAura,  
	PlayerLightAtk, 
	PlayerHeavyAtk, 
	ShieldSpell,
	Count 
};
enum class EUI { ButtonClick, Count };
enum class EVoiceLine { Count };

enum class EChannels { Music, Ambiance, SFX, UI, VOX, Count };

enum class GroundType;
enum class ESFXCollection;
class SFXCollection;

enum class SoundChannels {
	EAmbiance,
	ESFX,
	EUI,
	EVoiceLine,
	Count
};

class CAudioManager : public IObserver, public IStringObserver {
public:
	CAudioManager();
	~CAudioManager();

	// Listen to PostMaster
	void Receive(const SMessage& aMessage);
	void Receive(const SStringMessage& aMessage);


	void Update();


private:
	
	void SubscribeToMessages();
	void UnsubscribeToMessages();

	std::string GetPath(EMusic type) const;
	std::string GetPath(EAmbiance type) const;
	std::string GetPath(ESFX type) const;
	std::string GetPath(EUI type) const;
	std::string GetPath(EVoiceLine type) const;

	std::string TranslateChannels(EChannels enumerator) const;
	std::string TranslateMusic(EMusic enumerator) const;
	std::string TranslateAmbiance(EAmbiance enumerator) const;
	std::string TranslateSFX(ESFX enumerator) const;
	std::string TranslateUI(EUI enumerator) const;
	std::string TranslateVoiceLine(EVoiceLine enumerator) const;

private:
	std::string myAmbiancePath = "Audio/Ambience/";
	std::string myMusicPath = "Audio/Music/";
	std::string mySFXPath = "Audio/SFX/";
	std::string myUIPath = "Audio/UI/";
	std::string myVoxPath = "Audio/Voice/";
	GroundType myCurrentGroundType;

	CFModWrapper myWrapper;

	std::vector<CAudio*> myAmbianceAudio;
	std::vector<CAudio*> myMusicAudio;
	std::vector<CAudio*> mySFXAudio;
	std::vector<CAudio*> myUIAudio;
	std::vector<CAudio*> myVoicelineAudio;

	std::vector<CAudioChannel*> myChannels;

	CAudio* mySound;
	CAudioChannel* myChannel;

	std::vector<EMusic> myMusics;

};

