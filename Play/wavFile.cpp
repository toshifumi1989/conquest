#define _CRT_SECURE_NO_WARNINGS

#include "wavFile.h"
#include<assert.h>
#include<Windows.h>
#include<stdlib.h>
#include<stdio.h>

ALCdevice *device;
ALCcontext *context;
ALCuint bid[SOUND::SOUND_MAX];
ALuint sid[SOUND::SOUND_MAX];

WavFile *bgm;

void initMusic()
{
	device = alcOpenDevice(NULL);//const ALCchar *devicename

	assert(alcGetError(device) == ALC_NO_ERROR);

	context =
		alcCreateContext(
			device,//ALCdevice *device,
			NULL); //const ALCint* attrlist

	assert(alcGetError(device) == ALC_NO_ERROR);

	alcMakeContextCurrent(context);//ALCcontext *context
	assert(alcGetError(device) == ALC_NO_ERROR);

	alGenBuffers(SOUND::SOUND_MAX, bid);
	assert(alGetError() == AL_NO_ERROR);

	alGenSources(SOUND::SOUND_MAX, sid);
	assert(alGetError() == AL_NO_ERROR);

	bgm = new WavFile();
	//bgm->readSound("title.wav", SOUND::TITLE_BGM);
	bgm->readSound("title.wav", SOUND::TITLE_BGM);
	bgm->readSound("play.wav", SOUND::PLAY_BGM);
	//bgm->readSound("battle.wav", BGM::BATTLE_BGM);

}

void WavFile::readSound(const char* music, unsigned char sound)
{

	FILE *pFile = fopen(music, "rb");

	WavFile wavfile;

	fread(&wavfile, sizeof(WavFile), 1, pFile);

	unsigned int dataSize = wavfile.dataChunkSize * sizeof(short);

	short *data = (short*)malloc(dataSize);

	fread(data, dataSize, 1, pFile);

	fclose(pFile);

	if (wavfile.channel == 1)
	{//モノラル	
		if (wavfile.bitType == 8)
		{//8ビット		
			alBufferData(bid[sound], AL_FORMAT_MONO8, data, dataSize, wavfile.frequency);
		}
		else
		{//16ビット		
			alBufferData(bid[sound], AL_FORMAT_MONO16, data, dataSize, wavfile.frequency);
		}
	}
	else
	{//ステレオ
		if (wavfile.bitType == 8)
		{//8ビット		
			alBufferData(bid[sound], AL_FORMAT_STEREO8, data, dataSize, wavfile.frequency);
		}
		else
		{//16ビット		
			alBufferData(bid[sound], AL_FORMAT_STEREO16, data, dataSize, wavfile.frequency);
		}
	}

	assert(alGetError() == AL_NO_ERROR);

	alSourcei(sid[sound], AL_BUFFER, bid[sound]);
	assert(alGetError() == AL_NO_ERROR);

	alSourcei(sid[sound], AL_LOOPING, AL_TRUE);
	assert(alGetError() == AL_NO_ERROR);

}

///////////////////////////
//再生
///////////////////////////
void WavFile::playMusic(unsigned char sound)
{
	alSourcePlay(sid[sound]);//ALuint source
	assert(alGetError() == AL_NO_ERROR);

}

////////////////////////////
//データ削除
////////////////////////////
void WavFile::deleteMusic()
{
	alDeleteBuffers(
		SOUND::SOUND_MAX,      // ALsizei n
		bid);  // const ALuint* buffers

	alDeleteSources(
		SOUND::SOUND_MAX,          // ALsizei n
		sid);   // const ALuint* sources
}


/////////////////////////////////
//再生時間
/////////////////////////////////
const float WavFile::timeMusic(unsigned char sound)
{
	float time;

	alGetSourcef(sid[sound], AL_SEC_OFFSET, &time);

	return time;
}

/////////////////////////////////
//停止
/////////////////////////////////
void WavFile::stopMusic(unsigned char sound)
{

	alSourceStop(sid[sound]);	// ALuint source

}