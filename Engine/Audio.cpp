#include "stdafx.h"
#include "Audio.h"
#include <fmod/fmod.hpp>

CAudio::CAudio(FMOD::Sound* aSoundPointer)
{
	myFModSound = aSoundPointer;
}

CAudio::~CAudio()
{
	//delete myFModSound;
	//myFModSound = nullptr;
}
