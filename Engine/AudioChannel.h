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

private:
	// CAudioChannel should be created from CFModWrapper
	CAudioChannel(FMOD::ChannelGroup* aChannelPointer);
	~CAudioChannel();

private:
	FMOD::ChannelGroup* myFModChannel;
};

