#pragma once

namespace FMOD {
	class Sound;
}

class CAudio
{
	friend class CFModWrapper;
public:
	~CAudio();// FModWrapper returns a new:ed CAudio, if the destructor is not public they cannot be destroyed properly.

private:
	// CAudio should be created from CFModWrapper
	CAudio(FMOD::Sound* aSoundPointer);

private:
	FMOD::Sound* myFModSound;
};

