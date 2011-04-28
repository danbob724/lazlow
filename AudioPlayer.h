#pragma once

#include "SDL_mixer.h"

class AudioPlayer
{
public:
	AudioPlayer();

	bool audioReady();

	void playMusic();
	void stopMusic();

	void playEffect(int which);

	void enableAudio();
	void disableAudio();

	void closeAudio();

};
