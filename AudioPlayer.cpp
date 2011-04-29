#include "AudioPlayer.h"

bool isAble;
bool isEnabled;

Mix_Music* music;

Mix_Chunk* shot;
Mix_Chunk* hit;
Mix_Chunk* death;
Mix_Chunk* minedrop;
Mix_Chunk* mineblast;


AudioPlayer::AudioPlayer()
{

	isEnabled = true;
	isAble = true;

	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
        isAble = false;  

	music = Mix_LoadMUS( "C:/SDL/wuman.wav" );

	shot = Mix_LoadWAV( "shot.wav" );
	hit = Mix_LoadWAV( "hit.wav" );
	death = Mix_LoadWAV( "death.wav" );
	minedrop = Mix_LoadWAV( "minedrop.wav" );
	mineblast = Mix_LoadWAV( "mineblast.wav" );

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

	switch(which)
	{
	case(0):

		if( shot == NULL )
			return;

		Mix_PlayChannel( -1 , shot , 0 );
		break;
	case(1):

		if( hit == NULL )
			return;

		Mix_PlayChannel( -1 , hit , 0 );
		break;
	case(2):
		if( death == NULL )
			return;

		Mix_PlayChannel( -1 , death , 0 );
		break;
	case(3):
		if( minedrop == NULL )
			return;

		Mix_PlayChannel( -1 , minedrop , 0 );
		break;
	case(4):
		if( mineblast == NULL )
			return;

		Mix_PlayChannel( -1 , mineblast , 0 );
		break;

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