#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdlib.h>
#include <vector>
#include "../glut.h"

enum TEXTURE_ID 
{
	TITLE_BACKGROUND,
	FIELD,
	WALL,
	TEXTURE_MAX,
};


class Texture
{
public:
	Texture() {}
	~Texture(){}

	void read(const char* _texture);//読み込み
	void read_alpha(const char* _texture);//読み込み（アルファ

protected:
	//色のRGBの値
	typedef struct { unsigned char b, g, r; }RGB;

	std::vector<unsigned char>alpha; //アルファ値

	std::vector<float>vertex;		//位置
	std::vector<unsigned int>index;	//番号
	std::vector<float>normal;		//法線
	std::vector<float>tex;			//テクスチャ番号

};

extern GLuint textures[TEXTURE_ID::TEXTURE_MAX];

#endif