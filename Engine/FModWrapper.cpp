#include "stdafx.h"
#include "FModWrapper.h"
#include <fmod/fmod_studio.h>
#include <fmod/fmod_studio.hpp>
#include <fmod/fmod_studio_common.h>
#include <fmod/fmod_errors.h>

#pragma comment(lib, "fmod_vc.lib")
#pragma comment(lib, "fmodstudio_vc.lib")

#define FMOD_EXCEPTION(result) { CFModWrapper::CheckException(result); }

CFModWrapper::CFModWrapper()
{
    myStudioSystem = NULL;

    // Create the Studio System object.
    FMOD_EXCEPTION(FMOD::Studio::System::create(&myStudioSystem)); 

    // Initialize FMOD Studio, which will also initialize FMOD Core
    FMOD_EXCEPTION(myStudioSystem->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));

    myStudioSystem->getCoreSystem(&myCoreSystem);

    //FMOD::Sound* soundPtr;
    //myCoreSystem->createSound("Strings_test.mp3", FMOD_DEFAULT, nullptr, &soundPtr);
    //
    //FMOD::Channel* channel;
    //myCoreSystem->playSound(soundPtr, nullptr, false, &channel);
    //channel->setVolume(0.1f);
}

CFModWrapper::~CFModWrapper()
{
}

void CFModWrapper::CheckException(FMOD_RESULT aResult)
{
    if (aResult != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", aResult, FMOD_ErrorString(aResult));
        exit(-1);
    }
}
