#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#ifdef WIN32
    #include <SDL_mixer.h>
#else
    #include <SDL/SDL_mixer.h>
#endif

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

#endif