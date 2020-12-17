#pragma once
#include <string>
#include <fmod/fmod_common.h>

namespace FMOD {
	namespace Studio {
		class System;
	}
	class System;
	class Sound;
	class ChannelGroup;
}

class CFMod
{
public:
	CFMod();
	~CFMod();

	const FMOD::Studio::System* GetStudioSystem() const;
	const FMOD::System* GetCoreSystem() const;

	FMOD::Sound* CreateSound(std::string aFilePath);
	FMOD::Sound* CreateLoopingSound(std::string aFilePath);
	FMOD::ChannelGroup* CreateChannel(std::string aChannelName);

	void Play(FMOD::Sound* aSound, FMOD::ChannelGroup* aChannelGroup);

private:
	static void CheckException(FMOD_RESULT aResult);

private:
	FMOD::Studio::System* myStudioSystem;
	FMOD::System* myCoreSystem;

};
