#include "AudioPlayer.h"

bool isAble;
bool isEnabled;

Mix_Music* music;

Mix_Chunk* shot;
Mix_Chunk* hit;

AudioPlayer::AudioPlayer()
{

	isEnabled = true;
	isAble = true;

	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
        isAble = false;  

	music = Mix_LoadMUS( "C:/SDL/wuman.wav" );

	shot = Mix_LoadWAV( "shot.wav" );
	hit = Mix_LoadWAV( "hit.wav" );

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

	Mix_FreeMusic( music );
	Mix_FreeChunk( shot );
	Mix_FreeChunk( hit );

	Mix_CloseAudio();

}//closeAudio

void AudioPlayer::playEffect( int which )
{

	if( !audioReady() )
		return;

	if( which == 0 )
	{

		if( shot == NULL )
			return;

		Mix_PlayChannel( -1 , shot , 0 );

	}//if
	else if( which == 1 )
	{

		if( hit == NULL )
			return;

		Mix_PlayChannel( -1 , hit , 0 );

	}//if

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