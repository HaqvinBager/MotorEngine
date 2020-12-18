#pragma once
#include <string>

class CFMod;
class CAudio;
class CAudioChannel;

class CFModWrapper
{
public:
	CFModWrapper();
	~CFModWrapper();

	CAudio* RequestSound(std::string aFilePath);//Who takes care of the new CAudio* this returns? 2020 12 04 Adding destruction of it to CAudioManager
	CAudio* RequestLoopingSound(std::string aFilePath);//Who takes care of the new CAudio* this returns? 2020 12 04 Adding destruction of it to CAudioManager
	CAudioChannel* RequestChannel(std::string aChannelName);//Who takes care of the new CAudio* this returns? 2020 12 04 Adding destruction of it to CAudioManager

	void Play(CAudio* aSound, CAudioChannel* aChannel);

private:
	
private:
	CFMod* myFModInstance;
};

