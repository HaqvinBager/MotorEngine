#include "stdafx.h"
#include <iostream>
#include <random>
#include "AudioManager.h"

CAudioManager::CAudioManager() {
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::MainMenu, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::GroupLogo, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::IntroStarted, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::OutroStarted, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::IntroEnded, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::OutroEnded, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::UIMenuEnter, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::UIMenuToggle, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::UIMenuClick, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::PlayVoiceline, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::StopVoiceLine, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::ExitDialogue, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::OnPlayerGroundTypeChanged, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::PlayerAnimationStep, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::PlayerAttack_Ice, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::PlayerAttack_Fire, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::PlayerAttack_Sword, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::WeaponHit, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::PlayMapMusic, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::Inventory, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::ItemRoom, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::BlockBreak, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::BlockFrozen, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::BlockPushed, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::BlockSliding, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::BlockStoppingSliding, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::MapTransitionAudio, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::BoundaryTransitionAudio, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::ChestOpened, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::DoorOpened, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::GateOpened, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::FireplaceToggled, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::PressurePlateToggle, this);

	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::FrogManCollidedWithPlayer, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::FrogManAttack, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::FrogManDied, this);

	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::DogManCollidedWithPlayer, this);
	//MainSingleton::get_instance().get_post_master().subscribe(EMessageType::DogManDied, this);


	//TODO implemen _sfx_steps_audio Vector. Filled with all of each type of Stepping-sound.
	//init_sfx_steps();

	//for (int i = 0u; i < EINT(ESFXCollection::Count); ++i) {
	//	_sfx_collection_audio.emplace_back(init_collection(static_cast<ESFXCollection>(i)));
	//}

	//// Init ambiance
	//for (int i = 0u; i < (int)EAmbiance::Count; ++i) {
	//	_ambiance_audio.emplace_back(std::make_unique<Tga2D::CAudio>());
	//	_ambiance_audio.back()->Init(get_path(static_cast<EAmbiance>(i)).c_str(), true);
	//}

	//// Init SFX
	//for (int i = 0u; i < EINT(ESFX::Count); ++i) {
	//	_sfx_audio.emplace_back(std::make_unique<Tga2D::CAudio>());
	//	if (static_cast<ESFX>(i) == ESFX::FirePlace) {
	//		_sfx_audio.back()->Init(get_path(static_cast<ESFX>(i)).c_str(), true);
	//	}
	//	else {
	//		_sfx_audio.back()->Init(get_path(static_cast<ESFX>(i)).c_str());
	//	}
	//}

	//// Init EUI audio
	//for (int i = 0u; i < (int)EUI::Count; ++i) {
	//	_ui_audio.emplace_back(std::make_unique<Tga2D::CAudio>());
	//	_ui_audio.back()->Init(get_path(static_cast<EUI>(i)).c_str());
	//}

	//// Init VoiceLines
	//for (int i = 0u; i < (int)EVoiceLine::Count; ++i) {
	//	_voiceline_audio.emplace_back(std::make_unique<Tga2D::CAudio>());
	//	_voiceline_audio.back()->Init(get_path(static_cast<EVoiceLine>(i)).c_str());
	//}

	//// Init other sounds
	//_intro_audio = std::make_unique<Tga2D::CAudio>();
	//_intro_audio->Init("Audio/Voice/Intro.mp3");
	//_outro_audio = std::make_unique<Tga2D::CAudio>();
	//_outro_audio->Init("Audio/Voice/Outro.mp3");
	//_logo_audio = std::make_unique<Tga2D::CAudio>();
	//_logo_audio->Init("Audio/Voice/DiamondEyesStudio.mp3");

	// Init starting volumes
	//// Init mixer
	//for (int i = 0u; i < (int)SoundChannels::Count; ++i) {
	//	_mixer[static_cast<SoundChannels>(i)] = _starting_volume_mixer.at(static_cast<SoundChannels>(i));
	//}

	//// Set starting volume
	//set_channel_volume(SoundChannels::EAmbiance,  _starting_volume_mixer[SoundChannels::EAmbiance]);
	//set_channel_volume(SoundChannels::EMusic,     _starting_volume_mixer[SoundChannels::EMusic]);
	//set_channel_volume(SoundChannels::ESFX,       _starting_volume_mixer[SoundChannels::ESFX]);
	//set_channel_volume(SoundChannels::EUI,        _starting_volume_mixer[SoundChannels::EUI]);
	//set_channel_volume(SoundChannels::EVoiceLine, _starting_volume_mixer[SoundChannels::EVoiceLine]);
}

CAudioManager::~CAudioManager() {
	// Subscriptions
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::IntroStarted);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::OutroStarted);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::IntroEnded);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::OutroEnded);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::GroupLogo);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::UIMenuEnter);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::UIMenuToggle);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::UIMenuClick);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::PlayVoiceline);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::StopVoiceLine);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::ExitDialogue);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::OnPlayerGroundTypeChanged);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::PlayerAnimationStep);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::PlayerAttack_Ice);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::PlayerAttack_Fire);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::PlayerAttack_Sword);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::WeaponHit);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::MainMenu);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::PlayMapMusic);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::Inventory);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::ItemRoom);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::BlockBreak);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::BlockFrozen);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::BlockPushed);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::BlockSliding);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::BlockStoppingSliding);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::MapTransitionAudio);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::BoundaryTransitionAudio);

	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::DoorOpened);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::GateOpened);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::FireplaceToggled);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::PressurePlateToggle);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::FrogManAttack);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::FrogManDied);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::DogManDied);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::FrogManCollidedWithPlayer);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::DogManCollidedWithPlayer);
	//MainSingleton::get_instance().get_post_master().unsubscribe(this, EMessageType::ChestOpened);
}

//Tga2D::CAudio* CAudioManager::get_audio_source(ESFX type) {
//	return _sfx_audio[EINT(type)].get();
//}

float audio_lerp(float a, float b, float f) {
	return (a * (1.0f - f)) + (b * f);
}

void CAudioManager::update(float /*delta_time*/) {
	//if (duck_channel(SoundChannels::EAmbiance, SoundChannels::EVoiceLine, max(_mixer[SoundChannels::EVoiceLine] - 0.9f, 0.24f * _mixer[SoundChannels::EVoiceLine]))) {
	//}
	//else if (channel_volume(SoundChannels::EAmbiance) != _starting_volume_mixer[SoundChannels::EAmbiance]) {
	//	float new_volume = audio_lerp(channel_volume(SoundChannels::EAmbiance), _starting_volume_mixer[SoundChannels::EAmbiance], delta_time);
	//	set_channel_volume(SoundChannels::EAmbiance, new_volume);
	//}
}

//void CAudioManager::receive(const Message& message) {
//	std::string level = "";
//	bool open = false;
//	int line;
//
//	ESFXCollection col = ESFXCollection::Count;
//	switch (message.message_type) {
//
//	case EMessageType::GroupLogo:
//		_logo_audio->Play();
//		break;
//
//	case EMessageType::IntroStarted:
//		mute_channel(SoundChannels::EAmbiance);
//		_intro_audio->Play();
//		break;
//
//	case EMessageType::OutroStarted:
//		mute_channel(SoundChannels::EAmbiance);
//		_outro_audio->Play();
//		break;
//
//	case EMessageType::IntroEnded:
//		_intro_audio->Stop();
//		break;
//
//	case EMessageType::OutroEnded:
//		_outro_audio->Stop();
//		break;
//
//	case EMessageType::MainMenu:
//		_intro_audio->Stop();
//		_outro_audio->Stop();
//		play_ambiance(EAmbiance::Overworld3);
//		break;
//
//	case EMessageType::UIMenuEnter:
//		play_ui(EUI::UIMenuEnter);
//		break;
//
//	case EMessageType::UIMenuToggle:
//		play_ui(EUI::UIMenuToggle);
//		break;
//
//	case EMessageType::UIMenuClick:
//		play_ui(EUI::UIMenuClick);
//		break;
//
//	case EMessageType::PlayVoiceline:
//		line = *static_cast<int*>(message.data);
//		play_voice_line(line);
//		if (line < EINT(EVoiceLine::Scroll1) && _mixer[SoundChannels::EAmbiance] != 0.5f) {
//			set_channel_volume(SoundChannels::EAmbiance, 0.5f);
//		}
//		break;
//
//	case EMessageType::StopVoiceLine:
//		mute_channel(SoundChannels::EVoiceLine);
//		break;
//
//	case EMessageType::ExitDialogue:
//		if (_mixer[SoundChannels::EAmbiance] != 1.0) {
//			set_channel_volume(SoundChannels::EAmbiance, 1.0f);
//		}
//		break;
//
//	case EMessageType::PlayerAttack_Sword:
//		play_sfx(ESFX::StandardWeapon);
//		break;
//
//	case EMessageType::PlayerAttack_Ice:
//		play_sfx(ESFX::IceWeapon);
//		break;
//
//	case EMessageType::PlayerAttack_Fire:
//		play_sfx(ESFX::FireWeapon);
//		break;
//
//	case EMessageType::WeaponHit:
//		play_sfx(ESFX::WeaponHit);
//		break;
//
//
//
//	case EMessageType::PlayMapMusic:
//	{
//		level = *static_cast<std::string*>(message.data);
//		if (level.find("overworld") != std::string::npos) {
//			const bool* scrolls = MainSingleton::get_instance().get_global_state().scroll_number();
//			if (scrolls[1] == false)
//				play_ambiance(EAmbiance::Overworld1);
//			if (scrolls[1] == true)
//				play_ambiance(EAmbiance::Overworld2);
//			if (scrolls[3] == true)
//				play_ambiance(EAmbiance::Overworld3);
//			if (scrolls[6] == true)
//				play_ambiance(EAmbiance::Overworld4);
//		}
//		else if (level.find("maindungeon") != std::string::npos) {
//			play_ambiance(EAmbiance::Dungeon4);
//		}
//		else if (level.find("dungeon_1") != std::string::npos) {
//			play_ambiance(EAmbiance::Dungeon1);
//		}
//		else if (level.find("dungeon_2") != std::string::npos) {
//			play_ambiance(EAmbiance::Dungeon2);
//		}
//		else if (level.find("dungeon_3") != std::string::npos) {
//			play_ambiance(EAmbiance::Dungeon3);
//		}
//	}
//	break;
//
//	case EMessageType::Inventory:
//		open = *static_cast<bool*>(message.data);
//		if (open) {
//			play_ambiance(EAmbiance::Scrolls);
//		}
//		else {
//			level = MainSingleton::get_instance().get_global_state().current_map();
//			MainSingleton::get_instance().get_post_master().send_message({ EMessageType::PlayMapMusic, &level });
//		}
//		break;
//
//	case EMessageType::ItemRoom:
//		play_ambiance(EAmbiance::ItemRoom);
//		break;
//
//	case EMessageType::BlockBreak:
//		play_sfx(ESFX::BlockBreaking);
//		break;
//
//	case EMessageType::BlockFrozen:
//		play_sfx(ESFX::BlockFrozen);
//		break;
//
//	case EMessageType::BlockPushed:
//		play_sfx(ESFX::BlockPushed);
//		break;
//
//	case EMessageType::BlockSliding:
//		if (*static_cast<bool*>(message.data)) {
//			play_sfx(ESFX::BlockSliding);
//		}
//		else {
//			_sfx_audio[EINT(ESFX::BlockSliding)]->Stop();
//		}
//		break;
//
//	case EMessageType::BlockStoppingSliding:
//		play_sfx(ESFX::BlockStoppingSliding);
//		break;
//
//	case EMessageType::MapTransitionAudio:
//		play_sfx(ESFX::MapTransition);
//		break;
//
//	case EMessageType::BoundaryTransitionAudio:
//		play_sfx(ESFX::BoundaryTransition);
//		break;
//
//	case EMessageType::ChestOpened:
//		play_sfx(ESFX::ChestOpening);
//		if (*static_cast<bool*>(message.data)) {
//			play_sfx(ESFX::ScrollPickup);
//		}
//		else {
//			play_sfx(ESFX::WeaponPickup);
//		}
//		break;
//
//	case EMessageType::DoorOpened:
//		play_sfx(ESFX::DoorUnlocked);
//		play_sfx(ESFX::PuzzleSolved);
//		break;
//
//	case EMessageType::GateOpened:
//		play_sfx(ESFX::GateUnlocked);
//		play_sfx(ESFX::PuzzleSolved);
//		break;
//
//	case EMessageType::FireplaceToggled:
//		//if (*static_cast<bool*>(message.data)) {
//		//	//play_sfx(ESFX::FireCrackling);
//		//	if (_sfx_audio[EINT(ESFX::FirePlace)]->IsPlaying() == false) {
//		//		play_sfx(ESFX::FirePlace);
//		//	}
//		//	//INFO_PRINT("Fire Activte");
//		//} else {
//		//	_sfx_audio[EINT(ESFX::FirePlace)]->Stop();
//		//	//_sfx_audio[EINT(ESFX::FireCrackling)]->Stop();
//		//	//INFO_PRINT("Fire De-Activated");
//		//}
//		break;
//
//
//	case EMessageType::PressurePlateToggle:
//		if (*static_cast<bool*>(message.data)) {
//			play_sfx(ESFX::PressurePlateDown);
//		}
//		else {
//			play_sfx(ESFX::PressurePlateUp);
//		}
//		break;
//	case EMessageType::FrogManDied:
//		play_sfx(ESFX::FrogManDeath);
//		break;
//
//	case EMessageType::DogManDied:
//		play_sfx(ESFX::DogManDeath);
//		break;
//
//	case EMessageType::FrogManCollidedWithPlayer:
//		play_sfx(ESFXCollection::FrogManGrowl);
//		break;
//
//	case EMessageType::FrogManHit:
//		play_sfx(ESFXCollection::FrogManGrowl);
//		break;
//
//	case EMessageType::DogManHit:
//		play_sfx(ESFXCollection::DogManGrowl);
//		break;
//
//	case EMessageType::FrogManAttack:
//		play_sfx(ESFXCollection::FrogManGrowl);
//		break;
//
//	case EMessageType::DogManCollidedWithPlayer:
//		play_sfx(ESFXCollection::DogManGrowl);
//		break;
//
//	case EMessageType::OnPlayerGroundTypeChanged:
//		_current_player_ground_type = *static_cast<GroundType*>(message.data);
//		break;
//
//	case EMessageType::PlayerAnimationStep:
//		col = SFXCollectionUtility::esfx_collection_to_type(_current_player_ground_type);
//		play_sfx(col);
//		break;
//
//	default:
//		break;
//	}
//}

void CAudioManager::play_ambiance(EAmbiance /*to_play*/) {
	//mute_channel(SoundChannels::EAmbiance);
	//_ambiance_audio[EINT(to_play)]->Play();
}

void CAudioManager::play_sfx(ESFXCollection /*to_play*/) {
	//_sfx_collection_audio[EINT(to_play)]->play();
}

void CAudioManager::play_sfx(ESFX /*to_play*/) {
	//_sfx_audio[EINT(to_play)]->Play();
}

void CAudioManager::play_ui(EUI /*to_play*/) {
	//_ui_audio[EINT(to_play)]->Play();
}

void CAudioManager::play_voice_line(int /*to_play*/) {
	//mute_channel(SoundChannels::EVoiceLine);
	//_voiceline_audio[to_play]->Play();
}

void CAudioManager::mute_channel(SoundChannels /*channel*/) {
	//switch (channel) {
	//case SoundChannels::EAmbiance:

	//	for (int i = 0u; i < (int)EAmbiance::Count; ++i) {
	//		_ambiance_audio[i]->Stop();
	//	}

	//	break;
	//case SoundChannels::ESFX:

	//	for (int i = 0u; i < (int)ESFX::Count; ++i) {
	//		_sfx_audio[i]->Stop();
	//	}

	//	break;
	//case SoundChannels::EUI:

	//	for (int i = 0u; i < (int)EUI::Count; ++i) {
	//		_ui_audio[i]->Stop();
	//	}

	//	break;
	//case SoundChannels::EVoiceLine:

	//	for (int i = 0u; i < (int)EVoiceLine::Count; ++i) {
	//		_voiceline_audio[i]->Stop();
	//	}

	//	break;
	//default:
	//	break;
	//}
}

bool CAudioManager::channel_playing(SoundChannels /*channel*/) {
	//switch (channel) {
	//case SoundChannels::EAmbiance:

	//	for (int i = 0u; i < (int)EAmbiance::Count; ++i) {
	//		if (_ambiance_audio[i]->IsPlaying()) {
	//			return true;
	//		}
	//	}

	//	break;
	//case SoundChannels::ESFX:

	//	for (int i = 0u; i < (int)ESFX::Count; ++i) {
	//		if (_sfx_audio[i]->IsPlaying()) {
	//			return true;
	//		}
	//	}

	//	break;
	//case SoundChannels::EUI:

	//	for (int i = 0u; i < (int)EUI::Count; ++i) {
	//		if (_ui_audio[i]->IsPlaying()) {
	//			return true;
	//		}
	//	}

	//	break;
	//case SoundChannels::EVoiceLine:

	//	for (int i = 0u; i < (int)EVoiceLine::Count; ++i) {
	//		if (_voiceline_audio[i]->IsPlaying()) {
	//			return true;
	//		}
	//	}

	//	break;
	//default:
	//	break;
	//}

	return false;
}

void CAudioManager::set_channel_volume(SoundChannels /*channel*/, float /*new_volume*/) {
	//_mixer[channel] = new_volume;

	//switch (channel) {
	//case SoundChannels::EAmbiance:

	//	for (int i = 0u; i < (int)EAmbiance::Count; ++i) {
	//		_ambiance_audio[i]->SetVolume(new_volume);
	//	}

	//	break;

	//case SoundChannels::ESFX:

	//	for (int i = 0u; i < (int)ESFX::Count; ++i) {
	//		_sfx_audio[i]->SetVolume(new_volume);
	//	}

	//	break;
	//case SoundChannels::EUI:

	//	for (int i = 0u; i < (int)EUI::Count; ++i) {
	//		_ui_audio[i]->SetVolume(new_volume);
	//	}

	//	break;
	//case SoundChannels::EVoiceLine:

	//	for (int i = 0u; i < (int)EVoiceLine::Count; ++i) {
	//		_voiceline_audio[i]->SetVolume(new_volume);
	//	}

	//	break;
	//default:
	//	break;
	//}
}

bool CAudioManager::duck_channel(SoundChannels change, SoundChannels listen_to, float new_volume) {
	if (channel_playing(listen_to)) {
		set_channel_volume(change, new_volume);
		return true;
	}
	return false;
}

void CAudioManager::set_volume(ESFX /*sfx*/, float /*volume*/) {
	//for (int i = 0u; i < (int)ESFX::Count; ++i) {
	//	if (static_cast<ESFX>(i) == sfx) {
	//		_sfx_audio[i]->SetVolume(volume);
	//		break;
	//	}
	//}
}

//void AudioManager::play_step() {
//	//INFO_PRINT("Play Step");
//	for (auto i = 0; i < _sfx_steps_audio.size(); ++i) {

//		if (_sfx_steps_audio[i]._type == _current_player_ground_type) {
//			//_sfx_steps_audio[i]._clips[_sfx_steps_audio[i]._last_played_index]
//			//if (!_sfx_steps_audio[i]._clips[_sfx_steps_audio[i]._last_played_index]->IsPlaying()) {

//			if (_sfx_steps_audio[i]._clips.size() > 0) {
//				int rand = RandomNumber::random(0, static_cast<int>(_sfx_steps_audio[i]._clips.size()) - 1, _sfx_steps_audio[i]._last_played_index);
//				_sfx_steps_audio[i]._clips[rand]->Play();
//				_sfx_steps_audio[i]._last_played_index = rand;
//			}
//			//}
//		}
//	}
//}

//void AudioManager::init_sfx_steps() {
//	_sfx_steps_audio.reserve(EINT(GroundType::Count));
//	for (int i = 0; i < EINT(GroundType::Count); ++i) {
//		_sfx_steps_audio.emplace_back(StepSFX());
//		_sfx_steps_audio.back()._type = static_cast<GroundType>(i);
//	}

//	std::string root_path = "Audio/SFX/";
//	std::filesystem::path p("Audio/SFX");
//	std::filesystem::directory_iterator start(p);
//	std::filesystem::directory_iterator end;
//	for (auto file = start; file != end; ++file) {
//		std::string audio_clip_path = "Audio/SFX/";
//		audio_clip_path.append(file->path().filename().string());

//		for (std::size_t i = 0; i < EINT(GroundType::Count); ++i) {
//			std::string ground_type_name = GroundTypeUtility::to_string(static_cast<GroundType>(i));
//			if (audio_clip_path.find(ground_type_name) != std::string::npos) {
//				_sfx_steps_audio[i]._clips.emplace_back(std::make_unique<Tga2D::CAudio>());
//				_sfx_steps_audio[i]._clips.back()->Init(audio_clip_path.c_str());
//				_sfx_steps_audio[i]._last_played_index = 0;
//			}
//		}
//	}
//}

//std::unique_ptr<SFXCollection> CAudioManager::init_collection(const ESFXCollection /*collection_type*/) {
//
//	//std::unique_ptr<SFXCollection> collection = std::make_unique<SFXCollection>(collection_type);
//	//std::string prefix = SFXCollectionUtility::to_string(collection_type);
//
//	//std::string root = "Audio/SFX";
//	//std::filesystem::path p(root);
//	//std::filesystem::directory_iterator start(p);
//	//std::filesystem::directory_iterator end;
//	//for (auto file = start; file != end; ++file) {
//
//	//	std::string file_name = file->path().filename().string();
//	//	if (file_name.find(prefix) != std::string::npos) {
//	//		collection->add_clip((root + "/" + file_name).c_str());
//	//		std::string print = "Collection [ ";
//	//		print.append(prefix);
//	//		print.append(" ] -> Adding [ ");
//	//		print.append((root + "/" + file_name));
//	//		print.append(" ]");
//	//		INFO_PRINT(print.c_str());
//	//	}
//	//}
//
//	//return std::move(collection);
////	return nullptr;
//}

std::string CAudioManager::get_path(EAmbiance type) const {
	std::string path = _ambiance_path;
	path.append(translate_ambiance(type));
	path.append(".mp3");
	return path;
}

std::string CAudioManager::get_path(ESFX type) const {
	std::string path = _sfx_path;
	path.append(translate_sfx(type));
	path.append(".wav");
	return path;
}

std::string CAudioManager::get_path(EUI type) const {
	std::string path = _ui_path;
	path.append(translate_ui(type));
	path.append(".wav");
	return path;
}

std::string CAudioManager::get_path(EVoiceLine type) const {
	std::string path = _vox_path;
	path.append(translate_voice_line(type));
	path.append(".mp3");
	return path;
}

std::string CAudioManager::translate_ambiance(EAmbiance enumerator) const {
	switch (enumerator) {
	case EAmbiance::Dungeon1:
		return "Dungeon1";
	case EAmbiance::Dungeon2:
		return "Dungeon2";
	case EAmbiance::Dungeon3:
		return "Dungeon3";
	case EAmbiance::Dungeon4:
		return "Dungeon4";
	case EAmbiance::ItemRoom:
		return "ItemRoom";
	case EAmbiance::Overworld1:
		return "Overworld1";
	case EAmbiance::Overworld2:
		return "Overworld2";
	case EAmbiance::Overworld3:
		return "Overworld3";
	case EAmbiance::Overworld4:
		return "Overworld4";
	case EAmbiance::Scrolls:
		return "Scrolls";
	default:
		return "";
	}
}
std::string CAudioManager::translate_sfx(ESFX enumerator) const {
	switch (enumerator) {
	case ESFX::BlockBreaking:
		return "BlockBreaking";
	case ESFX::BlockFrozen:
		return "BlockFrozen";
	case ESFX::BlockPushed:
		return "BlockPushed";
	case ESFX::BlockSliding:
		return "BlockSliding";
	case ESFX::BlockStoppingSliding:
		return "BlockStoppingSliding";
	case ESFX::BoundaryTransition:
		return "BoundaryTransition";
	case ESFX::ChestOpening:
		return "ChestOpening";
	case ESFX::DogManDeath:
		return "DogManDeath";
		/*	case ESFX::DogManGrowl1:
			return "DogManGrowl1";
			case ESFX::DogManGrowl2:
			return "DogManGrowl2";
			case ESFX::DogManGrowl3:
			return "DogManGrowl3";
			case ESFX::DogManGrowl4:
			return "DogManGrowl4";*/
	case ESFX::DoorUnlocked:
		return "DoorUnlocked";
	case ESFX::FireCrackling:
		return "FireCrackling";
	case ESFX::FireWeapon:
		return "FireWeapon";
	case ESFX::FrogManDeath:
		return "FrogManDeath";
		/*case ESFX::FrogManGrowl1:
		return "FrogManGrowl1";
		case ESFX::FrogManGrowl2:
		return "FrogManGrowl2";
		case ESFX::FrogManGrowl3:
		return "FrogManGrowl3";
		case ESFX::FrogManGrowl4:
		return "FrogManGrowl4";*/
	case ESFX::GateUnlocked:
		return "GateUnlocked";
	case ESFX::IceWeapon:
		return "IceWeapon";
	case ESFX::MapTransition:
		return "MapTransition";
	case ESFX::PressurePlateDown:
		return "PressurePlateDown";
	case ESFX::PressurePlateUp:
		return "PressurePlateUp";
	case ESFX::PuzzleSolved:
		return "PuzzleSolved";
	case ESFX::ScrollPickup:
		return "ScrollPickup";
	case ESFX::StandardWeapon:
		return "StandardWeapon";
	case ESFX::WeaponHit:
		return "WeaponHit";
	case ESFX::WeaponPickup:
		return "WeaponPickup";
	case ESFX::FirePlace:
		return "FirePlace";
	default:
		return "";
	}
}
std::string CAudioManager::translate_ui(EUI enumerator) const {
	switch (enumerator) {
	case EUI::UIMenuClick:
		return "UIMenuClick";
	case EUI::UIMenuEnter:
		return "UIMenuEnter";
	case EUI::UIMenuToggle:
		return "UIMenuToggle";
	default:
		return "";
	}
}
std::string CAudioManager::translate_voice_line(EVoiceLine enumerator) const {
	switch (enumerator) {
	case EVoiceLine::Dialogue1a:
		return "Dialogue1a";
	case EVoiceLine::Dialogue1b:
		return "Dialogue1b";
	case EVoiceLine::Dialogue1c:
		return "Dialogue1c";
	case EVoiceLine::Dialogue1d:
		return "Dialogue1d";
	case EVoiceLine::Dialogue1e:
		return "Dialogue1e";
	case EVoiceLine::Dialogue1f:
		return "Dialogue1f";
	case EVoiceLine::Dialogue1g:
		return "Dialogue1g";
	case EVoiceLine::Dialogue1h:
		return "Dialogue1h";
	case EVoiceLine::Dialogue1i:
		return "Dialogue1i";
	case EVoiceLine::Dialogue1j:
		return "Dialogue1j";
	case EVoiceLine::Dialogue1k:
		return "Dialogue1k";
	case EVoiceLine::Dialogue2a:
		return "Dialogue2a";
	case EVoiceLine::Dialogue2b:
		return "Dialogue2b";
	case EVoiceLine::Dialogue2c:
		return "Dialogue2c";
	case EVoiceLine::Dialogue2d:
		return "Dialogue2d";
	case EVoiceLine::Dialogue2e:
		return "Dialogue2e";
	case EVoiceLine::Dialogue2f:
		return "Dialogue2f";
	case EVoiceLine::Dialogue2g:
		return "Dialogue2g";
	case EVoiceLine::Dialogue2h:
		return "Dialogue2h";
	case EVoiceLine::Dialogue3a:
		return "Dialogue3a";
	case EVoiceLine::Dialogue3b:
		return "Dialogue3b";
	case EVoiceLine::Dialogue3c:
		return "Dialogue3c";
	case EVoiceLine::Dialogue3d:
		return "Dialogue3d";
	case EVoiceLine::Dialogue3e:
		return "Dialogue3e";
	case EVoiceLine::Dialogue3f:
		return "Dialogue3f";
	case EVoiceLine::Dialogue4a:
		return "Dialogue4a";
	case EVoiceLine::Dialogue4b:
		return "Dialogue4b";
	case EVoiceLine::Dialogue4c:
		return "Dialogue4c";
	case EVoiceLine::Dialogue4d:
		return "Dialogue4d";
	case EVoiceLine::Dialogue4e:
		return "Dialogue4e";
	case EVoiceLine::Dialogue4f:
		return "Dialogue4f";
	case EVoiceLine::Dialogue5a:
		return "Dialogue5a";
	case EVoiceLine::Dialogue5b:
		return "Dialogue5b";
	case EVoiceLine::Dialogue5c:
		return "Dialogue5c";
	case EVoiceLine::Fluff1:
		return "Fluff1";
	case EVoiceLine::Fluff2a:
		return "Fluff2a";
	case EVoiceLine::Fluff2b:
		return "Fluff2b";
	case EVoiceLine::Fluff3:
		return "Fluff3";
	case EVoiceLine::Fluff4:
		return "Fluff4";
	case EVoiceLine::Fluff5:
		return "Fluff5";
	case EVoiceLine::Fluff6a:
		return "Fluff6a";
	case EVoiceLine::Fluff6b:
		return "Fluff6b";
	case EVoiceLine::Fluff7:
		return "Fluff7";
	case EVoiceLine::Scroll1:
		return "Scroll1";
	case EVoiceLine::Scroll2:
		return "Scroll2";
	case EVoiceLine::Scroll3:
		return "Scroll3";
	case EVoiceLine::Scroll4:
		return "Scroll4";
	case EVoiceLine::Scroll5:
		return "Scroll5";
	case EVoiceLine::Scroll6:
		return "Scroll6";
	case EVoiceLine::Scroll7:
		return "Scroll7";
	case EVoiceLine::Scroll8:
		return "Scroll8";
	case EVoiceLine::Scroll9:
		return "Scroll9";
	default:
		return "";
	}
}
