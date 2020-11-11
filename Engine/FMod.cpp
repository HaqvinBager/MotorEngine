#include "stdafx.h"
#include "FMod.h"
#include <fmod/fmod_studio.h>
#include <fmod/fmod_studio.hpp>
#include <fmod/fmod_studio_common.h>
#include <fmod/fmod_errors.h>

#pragma comment(lib, "fmod_vc.lib")
#pragma comment(lib, "fmodstudio_vc.lib")

#define FMOD_EXCEPTION(result)	{ FMOD_RESULT fresult = result; if (fresult != FMOD_OK) {ENGINE_ERROR_MESSAGE(FMOD_ErrorString(fresult)); exit(-1);} }

CFMod::CFMod()
{
    myStudioSystem = NULL;

    // Create the Studio System object.
    FMOD_EXCEPTION(FMOD::Studio::System::create(&myStudioSystem));

    // Initialize FMOD Studio, which will also initialize FMOD Core
    FMOD_EXCEPTION(myStudioSystem->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));

    // Get core system, initialized when studio system is initialized
    FMOD_EXCEPTION(myStudioSystem->getCoreSystem(&myCoreSystem));
}

CFMod::~CFMod()
{
    myCoreSystem->release();
    myStudioSystem->release();
}

const FMOD::Studio::System* CFMod::GetStudioSystem() const
{
    return myStudioSystem;
}

const FMOD::System* CFMod::GetCoreSystem() const
{
    return myCoreSystem;
}

FMOD::Sound* CFMod::CreateSound(std::string aFilePath)
{
    FMOD::Sound* soundPtr;
    FMOD_EXCEPTION(myCoreSystem->createSound(aFilePath.c_str(), FMOD_DEFAULT, nullptr, &soundPtr));
    return soundPtr;
}

FMOD::ChannelGroup* CFMod::CreateChannel(std::string aChannelName)
{
    FMOD::ChannelGroup* channelPtr;
    FMOD_EXCEPTION(myCoreSystem->createChannelGroup(aChannelName.c_str(), &channelPtr));
    return channelPtr;
}

void CFMod::Play(FMOD::Sound* aSound, FMOD::ChannelGroup* aChannelGroup)
{
    FMOD_EXCEPTION(myCoreSystem->playSound(aSound, aChannelGroup, false, NULL));
}

void CFMod::CheckException(FMOD_RESULT aResult)
{
    if (aResult != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", aResult, FMOD_ErrorString(aResult));
        exit(-1);
    }
}
