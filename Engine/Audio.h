#pragma once

namespace FMOD {
	class Sound;
}

class CAudio
{
	friend class CFModWrapper;

private:
	// CAudio should be created from CFModWrapper
	CAudio(FMOD::Sound* aSoundPointer);
	~CAudio();

private:
	FMOD::Sound* myFModSound;
};

