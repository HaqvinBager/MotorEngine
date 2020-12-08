#pragma once

namespace FMOD {
	class ChannelGroup;
}

class CAudioChannel
{
	friend class CFModWrapper;
public:
	const bool IsPlaying() const;

	void SetVolume(float aVolumeLevel);
	void Stop();
	void Mute(bool aDoMute);
	
	//Pitch
public:
	~CAudioChannel();// 2020 12 04  - Destruction of CAudioChannel is not taken care of by FMOD
private:
	// CAudioChannel should be created from CFModWrapper
	CAudioChannel(FMOD::ChannelGroup* aChannelPointer);

private:
	FMOD::ChannelGroup* myFModChannel;
};

