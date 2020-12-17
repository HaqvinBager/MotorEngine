#include "stdafx.h"
#include "FModWrapper.h"
#include "FMod.h"
#include "Audio.h"
#include "AudioChannel.h"

CFModWrapper::CFModWrapper()
{
	myFModInstance = new CFMod();
}

CFModWrapper::~CFModWrapper()
{
	delete myFModInstance;
	myFModInstance = nullptr;
}

CAudio* CFModWrapper::RequestSound(std::string aFilePath)
{
	return new CAudio(myFModInstance->CreateSound(aFilePath));
}

CAudio* CFModWrapper::RequestLoopingSound(std::string aFilePath)
{
	return new CAudio(myFModInstance->CreateLoopingSound(aFilePath));
}

CAudioChannel* CFModWrapper::RequestChannel(std::string aChannelName)
{
	return new CAudioChannel(myFModInstance->CreateChannel(aChannelName));
}

void CFModWrapper::Play(CAudio* aSound, CAudioChannel* aChannel)
{
	myFModInstance->Play(aSound->myFModSound, aChannel->myFModChannel);
}
