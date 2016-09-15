#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdlib.h>
#include <vector>
#include "../glut.h"

enum TEXTURE_ID 
{//読み込みテクスチャ番号
	TITLE_BACKGROUND,
	TITLE_SPACE,
	TITLE_NAME,

	FIELD,
	WALL,
	CONTROLLER,
	SMOKEEFFECT,

	VICTORYorDEFEAT,
	RESULT_WORD,

	TEXTURE_MAX,
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

#endif