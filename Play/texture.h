#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdlib.h>
#include <vector>
#include "../glut.h"

enum TEXTURE_ID 
{//読み込みテクスチャ番号
	TITLE_BACKGROUND,	//タイトル拝啓
	TITLE_SPACE,		//タイトル操作文字
	TITLE_NAME,			//タイトル名

	FIELD,				//フィールド
	WALL,				//壁
	CONTROLLER,			//操作方法
	SMOKEEFFECT,		//煙エフェクト用

	VICTORYorDEFEAT,	//勝敗
	RESULT_WORD,		//リザルト操作文字

	TEXTURE_MAX,		//テクスチャ総枚数
};

class Texture
{
public:
	Texture() {}
	~Texture(){}

	void read(const char* _texture);		//読み込み
	void read_alpha(const char* _texture);	//読み込み（アルファ変換

protected:

	typedef struct { unsigned char b, g, r; }RGB;	//色のRGBの値
	std::vector<unsigned char>alpha;//アルファ値

	std::vector<float>vertex;		//位置
	std::vector<unsigned int>index;	//番号
	std::vector<float>normal;		//法線
	std::vector<float>tex;			//テクスチャ番号

};

extern GLuint textures[TEXTURE_ID::TEXTURE_MAX];
extern Texture *smokeTex;

#endif