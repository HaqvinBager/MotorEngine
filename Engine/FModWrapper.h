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

	CAudio* RequestSound(std::string aFilePath);
	CAudioChannel* RequestChannel(std::string aChannelName);

	void Play(CAudio* aSound, CAudioChannel* aChannel);

private:
	
private:
	CFMod* myFModInstance;
};

