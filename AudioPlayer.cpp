#include "AudioPlayer.h"

bool isAble;
bool isEnabled;

Mix_Music* music;

Mix_Chunk* sfx[8];

AudioPlayer::AudioPlayer()
{

	isEnabled = true;
	isAble = true;

	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
        isAble = false;  

	music = Mix_LoadMUS( "C:/SDL/wuman.wav" );

	sfx[0] = Mix_LoadWAV( "shot.wav" );
	sfx[1] = Mix_LoadWAV( "hit.wav" );
	sfx[2] = Mix_LoadWAV( "death.wav" );
	sfx[3] = Mix_LoadWAV( "minedrop.wav" );
	sfx[4] = Mix_LoadWAV( "mineblast.wav" );
	sfx[5] = Mix_LoadWAV( "bosses.wav" );
	sfx[6] = Mix_LoadWAV( "win.wav" );
	sfx[7] = Mix_LoadWAV( "minepick.wav" );

}//constructor

bool AudioPlayer::audioReady()
{

	return (isAble && isEnabled);

}//audioReady

void AudioPlayer::enableAudio()
{

	isEnabled = true;

}//enableAudio

void AudioPlayer::disableAudio()
{

	isEnabled = false;

}//disableAudio

void AudioPlayer::closeAudio()
{

	Mix_FreeMusic( music);
	Mix_FreeChunk(sfx[0]);
	Mix_FreeChunk(sfx[1]);
	Mix_FreeChunk(sfx[2]);
	Mix_FreeChunk(sfx[3]);
	Mix_FreeChunk(sfx[4]);
	Mix_FreeChunk(sfx[5]);
	Mix_FreeChunk(sfx[6]);
	Mix_FreeChunk(sfx[7]);

	Mix_CloseAudio();

}//closeAudio

void AudioPlayer::playEffect( int which )
{
	if( !audioReady() )
		return;

	if(sfx[which] == NULL)
		return;

	Mix_PlayChannel( -1 , sfx[which] , 0 );

}//playEffect

void AudioPlayer::playMusic()
{

	if( !audioReady() )
		return;

	if( music == NULL )
		return;

	if( Mix_PlayingMusic() )
		return;

	Mix_PlayMusic( music , -1 );

}//playMusic

void AudioPlayer::stopMusic()
{

	Mix_HaltMusic();

}//stopMusic