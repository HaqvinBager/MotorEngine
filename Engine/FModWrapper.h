#pragma once
#include <fmod/fmod_common.h>

namespace FMOD {
	namespace Studio {
		class System;
	}
	class System;
}

class CFModWrapper
{
public:
	CFModWrapper();
	~CFModWrapper();

private:
	static void CheckException(FMOD_RESULT aResult);

private:
	FMOD::Studio::System* myStudioSystem;
	FMOD::System* myCoreSystem;

};

