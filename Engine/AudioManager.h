#pragma once
#pragma once
#include <vector>
#include <random>
#include <memory>
//#include <tga2d/audio/audio.h>
//#include <tga2d/audio/audio_out.h>
//#include "PostMaster.hpp"

enum class EAmbiance { Dungeon1, Dungeon2, Dungeon3, Dungeon4, ItemRoom, Overworld1, Overworld2, Overworld3, Overworld4, Scrolls, Count };
enum class ESFX { BlockBreaking, BlockFrozen, BlockPushed, BlockSliding, BlockStoppingSliding, BoundaryTransition, ChestOpening, DogManDeath, /*DogManGrowl1, DogManGrowl2, DogManGrowl3, DogManGrowl4,*/ DoorUnlocked, FireCrackling, FireWeapon, FrogManDeath, /*FrogManGrowl1, FrogManGrowl2, FrogManGrowl3, FrogManGrowl4,*/ GateUnlocked, IceWeapon, MapTransition, PressurePlateDown, PressurePlateUp, PuzzleSolved, ScrollPickup, StandardWeapon, WeaponHit, WeaponPickup, FirePlace, Count };
enum class EUI { UIMenuClick, UIMenuEnter, UIMenuToggle, Count };
enum class EVoiceLine { Dialogue1a, Dialogue1b, Dialogue1c, Dialogue1d, Dialogue1e, Dialogue1f, Dialogue1g, Dialogue1h, Dialogue1i, Dialogue1j, Dialogue1k, Dialogue2a, Dialogue2b, Dialogue2c, Dialogue2d, Dialogue2e, Dialogue2f, Dialogue2g, Dialogue2h, Dialogue3a, Dialogue3b, Dialogue3c, Dialogue3d, Dialogue3e, Dialogue3f, Dialogue4a, Dialogue4b, Dialogue4c, Dialogue4d, Dialogue4e, Dialogue4f, Dialogue5a, Dialogue5b, Dialogue5c, Fluff1, Fluff2a, Fluff2b, Fluff3, Fluff4, Fluff5, Fluff6a, Fluff6b, Fluff7, Scroll1, Scroll2, Scroll3, Scroll4, Scroll5, Scroll6, Scroll7, Scroll8, Scroll9, Count };

//enum class PlayerStep { StepSand, StepGrass, StepDungeonStone, StepDungeonSand, StepStonePath };

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

class CAudioManager/* : public Observer*/ {
public:
	CAudioManager();
	~CAudioManager();

	//Tga2D::CAudio* get_audio_source(ESFX type);

	void update(float delta_time);

	// Listen to PostMaster
	//void receive(const Message& message);

	void play_ambiance(EAmbiance to_play);

	void play_sfx(ESFXCollection to_play);
	void play_sfx(ESFX to_play);
	void play_ui(EUI to_play);
	void play_voice_line(int to_play);

	// Channels
	void mute_channel(SoundChannels channel);
	bool channel_playing(SoundChannels channel);
	void set_channel_volume(SoundChannels channel, float new_volume);
	bool duck_channel(SoundChannels change, SoundChannels listen_to, float new_volume);
	const float channel_volume(SoundChannels /*channel*/) const { return /*_mixer.at(channel)*/0; }


	void set_volume(ESFX sfx, float volume);


	//void play_step();

private:

	//void init_sfx_steps();
	//std::unique_ptr<SFXCollection> init_collection(const ESFXCollection collection);

	std::string get_path(EAmbiance type) const;
	std::string get_path(ESFX type) const;
	std::string get_path(EUI type) const;
	std::string get_path(EVoiceLine type) const;

	//std::vector<std::unique_ptr<Tga2D::CAudio>> _ambiance_audio;
	//std::vector<std::unique_ptr<Tga2D::CAudio>> _music_audio;
	//std::vector<std::unique_ptr<Tga2D::CAudio>> _sfx_audio;
	////std::vector<StepSFX>                        _sfx_steps_audio;
	//std::vector<std::unique_ptr<SFXCollection>>	_sfx_collection_audio;
	//std::vector<std::unique_ptr<Tga2D::CAudio>> _ui_audio;
	//std::vector<std::unique_ptr<Tga2D::CAudio>> _voiceline_audio;
	//std::unique_ptr<Tga2D::CAudio>              _intro_audio;
	//std::unique_ptr<Tga2D::CAudio>              _outro_audio;
	//std::unique_ptr<Tga2D::CAudio>              _logo_audio;
	//std::unordered_map<SoundChannels, float>    _mixer;
	//std::unordered_map<SoundChannels, float>    _starting_volume_mixer;

	std::string translate_ambiance(EAmbiance enumerator) const;
	std::string translate_sfx(ESFX enumerator) const;
	std::string translate_ui(EUI enumerator) const;
	std::string translate_voice_line(EVoiceLine enumerator) const;

	//std::string to_string(ESFXCollection type) const;
	//ESFXCollection esfx_collection_to_type(std::string prefix) const;

	std::string _ambiance_path = "Audio/Ambiance/";
	std::string _sfx_path = "Audio/SFX/";
	std::string _ui_path = "Audio/UI/";
	std::string _vox_path = "Audio/Voice/";
	GroundType _current_player_ground_type;



};

