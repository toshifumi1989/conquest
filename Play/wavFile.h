#ifndef WAVFILE_H
#define WAVFILE_H

#include "../AL/al.h"
#include "../AL\alc.h"

#pragma comment(lib,"OpenAL32.lib") 

enum SOUND
{
	TITLE_BGM,
	CLICK,

	PLAY_BGM,
	SHOOT,
	ISDEAD,

	RESULT_BGM,

	SOUND_MAX
};

extern ALCdevice *device;
extern ALCcontext *context;
extern ALCuint bid[SOUND::SOUND_MAX];
extern ALuint sid[SOUND::SOUND_MAX];

class WavFile 
{
public:
	WavFile(){}
	~WavFile() {}

	void readSound(const char* music ,unsigned char sound);//wavFile読み込み
	void playMusic(unsigned char sound);			//音再生
	void deleteMusic();								//データ削除
	const float timeMusic(unsigned char sound);		//再生時間を渡す
	void stopMusic(unsigned char sound);			//音停止
	void playSceneStopMusic();						//playシーン時の音停止管理

private:
	char fileID[4];//ファイル識別子
	int fileSize;//ファイルのサイズ
	char RIFFID[4];//RIFF識別子

	char fmtChunkID[4];//チャンク識別子
	int fmtChunkSize;//チャンクサイズ
	short formatType;//フォーマットの種類
	short channel;//チャンネル数
	int frequency;//周波数
	int dataSizePerSec;//１秒あたりのデータサイズ
	short blockSize;//ブロックサイズ
	short bitType;//ビット数

	char dataChunkID[4];//チャンク識別子
	int dataChunkSize;//チャンクサイズ

};

extern WavFile *sound;
extern void initMusic();

#endif