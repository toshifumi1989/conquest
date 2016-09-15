#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdlib.h>
#include <vector>
#include "../glut.h"

enum TEXTURE_ID 
{//�ǂݍ��݃e�N�X�`���ԍ�
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

	void read(const char* _texture);		//�ǂݍ���
	void read_alpha(const char* _texture);	//�ǂݍ��݁i�A���t�@�ϊ�

protected:

	typedef struct { unsigned char b, g, r; }RGB;	//�F��RGB�̒l
	std::vector<unsigned char>alpha;//�A���t�@�l

	std::vector<float>vertex;		//�ʒu
	std::vector<unsigned int>index;	//�ԍ�
	std::vector<float>normal;		//�@��
	std::vector<float>tex;			//�e�N�X�`���ԍ�

};

extern GLuint textures[TEXTURE_ID::TEXTURE_MAX];

#endif